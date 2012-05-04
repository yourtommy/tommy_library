#include "hash.h"
#include "list.h"
#include "utility.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define DELETED_INT	(-INFINITY_INT)

//====================
// Hashing functions
//====================
int
CAHashingMod(int value, unsigned slot_num)
{
    unsigned divisor = MaxPrimeNum(slot_num);
    int ret = value % divisor;
    if (ret < 0)
        ret += divisor;
    return ret;
}

int
CAHashingMult(int value, unsigned slot_num)
{
    if (value < 0)
        value = -value;
    int p = 0;
    while (slot_num >> p != 0) 
        p++;
    static const unsigned int s = 2654435769;
    unsigned decimal = s * value;
    int hashed = decimal >> (sizeof(int) - p);
    return hashed;
}

int
OAHashingLinearProbing(int value, unsigned slot_num, unsigned times)
{
    if (times >= slot_num)
	return -1; // overflow
    unsigned index = CAHashingMod(value, slot_num);
    index += times;
    if (index >= slot_num)
	index -= slot_num;
    return index;
}

//====================
// Direct Addressing
//====================
static bool
DAHashInit(Hash *hashp, va_list a_list)
{
    UNUSED(a_list);
    hashp->impl = calloc(hashp->slot_num, sizeof(int));
    for (unsigned i = 0; i < hashp->slot_num; i++)
	*((int *)hashp->impl + i) = INFINITY_INT;
    return true;
}

static bool 
DAHashInsert(Hash *hashp, int value)
{
    int *a = hashp->impl;
    if (a[value] != INFINITY_INT)
        return false;
    a[value] = value;
    return true;
}

static bool 
DAHashSearch(Hash *hashp, int value)
{
    int *a = hashp->impl;
    return a[value] != INFINITY_INT;
}

static bool 
DAHashDelete(Hash *hashp, int value)
{
    int *a = hashp->impl;
    if (a[value] == INFINITY_INT)
        return false;
    a[value] = INFINITY_INT;
    return true;
}

static bool 
DAHashFree(Hash *hashp)
{
    free(hashp->impl);
    hashp->impl = NULL;
    return true;
}

//====================
// Closed Addressing
//====================
typedef struct CAHash
{
    CAHashingPtr hashing_p;
    List slots[];
} CAHash;

static bool
CAHashInit(Hash *hashp, va_list a_list)
{
    CAHash *cahashp = malloc(sizeof(CAHash) + hashp->slot_num  * sizeof(List));
    cahashp->hashing_p = va_arg(a_list, CAHashingPtr);
    for (unsigned i = 0; i < hashp->slot_num; i++)
        ListInit(&cahashp->slots[i], LT_DLS);
    hashp->impl = cahashp;
    return true;
}

static bool 
CAHashSearch(Hash *hashp, int value)
{
    CAHash *cahashp = hashp->impl;
    if (cahashp == NULL || cahashp->hashing_p == NULL)
        return false;
    int hashed = cahashp->hashing_p(value, hashp->slot_num);
    ListItor itor =  ListHead(&cahashp->slots[hashed]);
    while (!ListItorNull(itor)) {
        if (ListValue(itor) == value)
            return true;
        itor = ListItorNext(itor);
    }
    return false;
}

static bool 
CAHashInsert(Hash *hashp, int value)
{
    CAHash *cahashp = hashp->impl;
    if (cahashp == NULL || cahashp->hashing_p == NULL)
        return false;
    if (CAHashSearch(hashp, value))
        return false;

    int hashed = cahashp->hashing_p(value, hashp->slot_num);
    return ListPrepend(&cahashp->slots[hashed], value);
}

static bool 
CAHashDelete(Hash *hashp, int value)
{
    CAHash *cahashp = hashp->impl;
    if (cahashp == NULL || cahashp->hashing_p == NULL)
        return false;
    if (!CAHashSearch(hashp, value))
        return false;

    int hashed = cahashp->hashing_p(value, hashp->slot_num);
    ListItor itor =  ListHead(&cahashp->slots[hashed]);

    while (!ListItorNull(itor)) {
        if (ListValue(itor) == value) {
            ListDelete(itor);
            return true;
        }
        itor = ListItorNext(itor);
    }
    return false;
}

static bool 
CAHashFree(Hash *hashp)
{
    free(hashp->impl);
    hashp->impl = NULL;
    return true;
}

static bool
CAHashRehash(Hash *hashp)
{
    CAHash *cahashp = hashp->impl;
    Hash new_hash;
    HashInit(&new_hash, hashp->type, hashp->slot_num * 2, cahashp->hashing_p);
    new_hash.ele_num = hashp->ele_num;
    for (unsigned i = 0; i < hashp->slot_num; i++) {
	List list = cahashp->slots[i];
	ListItor itor = ListHead(&list);
	while (!ListItorNull(itor)) {
	    CAHashInsert(&new_hash, ListValue(itor));
	    itor = ListItorNext(itor);
	}
    }

    HashFree(hashp);
    *hashp = new_hash;
    return true;
}

//====================
// Open Addressing
//====================
typedef struct OAHash
{
    OAHashingPtr hashing_p;
    int slots[];
} OAHash;

static bool
OAHashInit(Hash *hashp, va_list a_list)
{
    OAHash *oahashp = malloc(sizeof(OAHash) + hashp->slot_num * sizeof(int));
    oahashp->hashing_p = va_arg(a_list, OAHashingPtr);
    for (unsigned i = 0; i < hashp->slot_num; i++)
	oahashp->slots[i] = INFINITY_INT;
    hashp->impl = oahashp;
    return true;
}

static bool 
OAHashInsert(Hash *hashp, int value)
{
    OAHash *oahashp = (OAHash *)hashp->impl;
    int times = 0;
    int index;
    while ((index = oahashp->hashing_p(value, hashp->slot_num, times++)) != -1) {
	if (oahashp->slots[index] == value)
	    return false;
	if (oahashp->slots[index] == INFINITY_INT || 
		oahashp->slots[index] == DELETED_INT) {
	    oahashp->slots[index] = value;
	    return true;
	}
    }
    return false;
}

static bool 
OAHashSearch(Hash *hashp, int value)
{
    OAHash *oahashp = (OAHash *)hashp->impl;
    int times = 0;
    int index;
    while ((index = oahashp->hashing_p(value, hashp->slot_num, times++)) != -1) {
	if (oahashp->slots[index] == INFINITY_INT)
	    return false;
	if (oahashp->slots[index] == value)
	    return true;
    }
    return false;
}

static bool 
OAHashDelete(Hash *hashp, int value)
{
    OAHash *oahashp = (OAHash *)hashp->impl;
    int times = 0;
    int index;
    while ((index = oahashp->hashing_p(value, hashp->slot_num, times++)) != -1) {
	if (oahashp->slots[index] == INFINITY_INT) 
	    return false;
	if (oahashp->slots[index] == value) {
	    oahashp->slots[index] = DELETED_INT;
	    return true;
	}
    }
    return false;
}

static bool 
OAHashFree(Hash *hashp)
{
    free(hashp->impl);
    hashp->impl = NULL;
    return true;
}

static bool
OAHashRehash(Hash *hashp)
{
    OAHash *oahashp = hashp->impl;
    Hash new_hash;
    HashInit(&new_hash, hashp->type, hashp->slot_num * 2, oahashp->hashing_p);
    new_hash.ele_num = hashp->ele_num;
    for (unsigned i = 0; i < hashp->slot_num; i++) {
	int value = oahashp->slots[i];
	if (value != INFINITY_INT && value != DELETED_INT
	    && !OAHashInsert(&new_hash, value))
		return false;
    }

    HashFree(hashp);
    *hashp = new_hash;
    return true;
}

//===================
// Operation table
//===================
typedef struct HashOperation
{
    bool (*init_ptr)(Hash *hashp, va_list a_list);
    bool (*insert_ptr)(Hash *hashp, int value);
    bool (*search_ptr)(Hash *hashp, int value);
    bool (*delete_ptr)(Hash *hashp, int value);
    bool (*free_ptr)(Hash *hashp);
    bool (*rehash_ptr)(Hash *hashp);
} HashOperation;

static HashOperation hash_operations[] = {
    [HT_DA] = { &DAHashInit, &DAHashInsert, &DAHashSearch, &DAHashDelete, &DAHashFree, NULL },
    [HT_CA] = { &CAHashInit, &CAHashInsert, &CAHashSearch, &CAHashDelete, &CAHashFree, &CAHashRehash },
    [HT_OA] = { &OAHashInit, &OAHashInsert, &OAHashSearch, &OAHashDelete, &OAHashFree, &OAHashRehash },
};

#define VerifyType(type, operation) \
    (type < (sizeof(hash_operations) / sizeof(hash_operations[0])) \
         && *((void**)((unsigned char *)&hash_operations[type] + \
                offsetof(HashOperation, operation))) != NULL) 

//====================
// General 
//====================
static bool
HashReHash(Hash *hashp)
{
    if (hashp == NULL)
	return false;
    // Don't need rehash.
    if (hash_operations[hashp->type].rehash_ptr == NULL)
	return true;
    return hash_operations[hashp->type].rehash_ptr(hashp);
}

bool 
HashInit(Hash *hashp, HashType type, unsigned slot_num, ...)
{
    if (hashp == NULL || !VerifyType(type, init_ptr))
        return false;
    hashp->type = type;
    hashp->ele_num = 0;
    hashp->slot_num = slot_num;
    va_list a_list;
    va_start(a_list, slot_num);
    bool ret = hash_operations[type].init_ptr(hashp, a_list);
    va_end(a_list);
    return ret;
}

bool
HashInsert(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, insert_ptr))
        return false;
    if (hashp->ele_num >= hashp->slot_num * 0.75
	    && !HashReHash(hashp))
	return false;
    if (!hash_operations[hashp->type].insert_ptr(hashp, value))
	return false;
    ++hashp->ele_num;
    return true;
}

bool
HashSearch(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, search_ptr))
        return false;
    return hash_operations[hashp->type].search_ptr(hashp, value);
}

bool
HashDelete(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, delete_ptr))
        return false;
    if (!hash_operations[hashp->type].delete_ptr(hashp, value))
	return false;
    --hashp->ele_num;
    return true;
}

bool 
HashFree(Hash *hashp)
{
    if (hashp == NULL || !VerifyType(hashp->type, free_ptr))
        return false;
    if (!hash_operations[hashp->type].free_ptr(hashp))
	return false;
    hashp->ele_num = 0;
    hashp->slot_num = 0;
    return true;
}

