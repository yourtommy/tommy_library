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
    unsigned capacity;
    void *impl;
} Hash;

typedef int (*CAHashingPtr)(int value, unsigned slot_num);
typedef int (*OAHashingPtr)(int value, unsigned slot_num, int times);

int CAModHashing(int value, unsigned slot_num);
int CAMultHashing(int value, unsigned slot_num);

/***********************************
 * Initialize the hash.
 *
 * Direct Addressing: the valid value should in the range
 * of [0, capacity).
 *
 * Closed Addressing: the 3rd argument is a pointer of type
 * CAModHashing.
 *
 * Open Addressing: the 3rd argument is a pointer of type
 * OAModHashing.
 *
 * Perfect Hashing:
 ***********************************/
bool HashInit(Hash *hashp, HashType type, unsigned capacity, ...);

/***************************
 * Insert a value.
 ***************************/
bool HashInsert(Hash *hashp, int value);

/***************************
 * Search a value.
 ***************************/
bool HashSearch(Hash *hashp, int value);

/***************************
 * Delete a vlaue.
 ***************************/
bool HashDelete(Hash *hashp, int value);

/*************************
 * Free a hash.
 ************************/
bool HashFree(Hash *hashp);

#endif

