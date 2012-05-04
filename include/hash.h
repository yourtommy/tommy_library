#ifndef HASH_H__
#define HASH_H__

#include <stdbool.h>

typedef enum HashType
{
    HT_DA = 0, // Direct Addressing
    HT_CA, // Closed Addressing
    HT_OA, // Open Addressing
    HT_PH, // Perfect Hashing
} HashType;

typedef struct Hash
{
    HashType type;
    unsigned slot_num;
    unsigned ele_num;
    void *impl;
} Hash;

typedef int (*CAHashingPtr)(int value, unsigned slot_num);

/* Times starts from 0 */
typedef int (*OAHashingPtr)(int value, unsigned slot_num, unsigned times);

int CAHashingMod(int value, unsigned slot_num);
int CAHashingMult(int value, unsigned slot_num);
int OAHashingLinearProbing(int value, unsigned slot_num, unsigned times);

/***********************************
 * Initialize the hash.
 *
 * Direct Addressing: the valid value should in the range
 * of [0, slot_num).
 *
 * Closed Addressing: the 3rd argument is a pointer of type
 * CAModHashing.
 *
 * Open Addressing: the 3rd argument is a pointer of type
 * OAModHashing.
 *
 * Perfect Hashing:
 ***********************************/
bool HashInit(Hash *hashp, HashType type, unsigned slot_num, ...);

/***************************
 * Insert a value.
 * Return false if value already
 * exists.
 ***************************/
bool HashInsert(Hash *hashp, int value);

/***************************
 * Search a value.
 * Return true if found.
 ***************************/
bool HashSearch(Hash *hashp, int value);

/***************************
 * Delete a vlaue.
 * Return false if value doesn't
 * exist.
 ***************************/
bool HashDelete(Hash *hashp, int value);

/*************************
 * Free a hash.
 ************************/
bool HashFree(Hash *hashp);

#endif

