#include "hash.h"
#include "list.h"
#include "utility.h"
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

//====================
// Direct Addressing
//====================
static bool
DAHashInit(Hash *hashp, unsigned capacity, ...)
{
    hashp->impl = calloc(capacity, sizeof(int));
    memset(hashp->impl, 0xFF, capacity * sizeof(int));
    return true;
}

static bool 
DAHashInsert(Hash *hashp, int value)
{
    int *a = hashp->impl;
    if (a[value] != ~0)
        return false;
    a[value] = value;
    return true;
}

static bool 
DAHashSearch(Hash *hashp, int value)
{
    int *a = hashp->impl;
    return a[value] != ~0;
}

static bool 
DAHashDelete(Hash *hashp, int value)
{
    int *a = hashp->impl;
    if (a[value] == ~0)
        return false;
    a[value] = ~0;
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
    unsigned slot_num;
    List slots[];
} CAHash;

static bool
CAHashInit(Hash *hashp, unsigned capacity, ...)
{
    CAHash *cahashp = malloc(sizeof(CAHash) + capacity * sizeof(List));
    cahashp->slot_num = capacity;
    for (unsigned i = 0; i < capacity; i++)
        ListInit(&cahashp->slots[i], LT_DLS);
    hashp->impl = cahashp;
    return true;
}

static bool 
CAHashInsert(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
CAHashSearch(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
CAHashDelete(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
CAHashFree(Hash *hashp)
{
    // TODO
    UNUSED(hashp);
    return false;
}

//====================
// Open Addressing
//====================
static bool
OAHashInit(Hash *hashp, unsigned capacity, ...)
{
    // TODO
    UNUSED(hashp);
    UNUSED(capacity);
    return false;
}

static bool 
OAHashInsert(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
OAHashSearch(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
OAHashDelete(Hash *hashp, int value)
{
    // TODO
    UNUSED(hashp);
    UNUSED(value);
    return false;
}

static bool 
OAHashFree(Hash *hashp)
{
    // TODO
    UNUSED(hashp);
    return false;
}

//===================
// Operation table
//===================
typedef struct HashOperation
{
    bool (*init_ptr)(Hash *hashp, unsigned capacity, ...);
    bool (*insert_ptr)(Hash *hashp, int value);
    bool (*search_ptr)(Hash *hashp, int value);
    bool (*delete_ptr)(Hash *hashp, int value);
    bool (*free_ptr)(Hash *hashp);
} HashOperation;

static HashOperation hash_operations[] = {
    [HT_DA] = { &DAHashInit, &DAHashInsert, &DAHashSearch, &DAHashDelete, &DAHashFree },
    [HT_CA] = { &CAHashInit, &CAHashInsert, &CAHashSearch, &CAHashDelete, &CAHashFree },
    [HT_OA] = { &OAHashInit, &OAHashInsert, &OAHashSearch, &OAHashDelete, &OAHashFree },
};

#define VerifyType(type, operation) \
    (type < sizeof(hash_operations) / sizeof(hash_operations[0]) \
        && *((void **)&hash_operations[type] + offsetof(HashOperation, operation)) != NULL) \

//====================
// General 
//====================
bool 
HashInit(Hash *hashp, HashType type, unsigned capacity, ...)
{
    if (hashp == NULL || !VerifyType(type, init_ptr))
        return false;
    hashp->type = type;
    va_list a_list;
    va_start(a_list, capacity);
    bool ret = hash_operations[type].init_ptr(hashp, capacity, a_list);
    va_end(a_list);
    return ret;
}

bool
HashInsert(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, insert_ptr))
        return false;
    return hash_operations[hashp->type].insert_ptr(hashp, value);
}

bool HashSearch(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, search_ptr))
        return false;
    return hash_operations[hashp->type].search_ptr(hashp, value);
}

bool HashDelete(Hash *hashp, int value)
{
    if (hashp == NULL || !VerifyType(hashp->type, delete_ptr))
        return false;
    return hash_operations[hashp->type].delete_ptr(hashp, value);
}

bool HashFree(Hash *hashp)
{
    if (hashp == NULL || !VerifyType(hashp->type, free_ptr))
        return false;
    return hash_operations[hashp->type].free_ptr(hashp);
}
