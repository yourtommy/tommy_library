#ifndef HASH_H__
#define HASH_H__

#include <stdbool.h>

typedef enum HashType
{
    HT_DA = 0, // Direct Addressing
    HT_CA_MOD, // Closed Addressing Mod hashing
    HT_CA_MULT, // Closed Addressing Multiply hashing
    HT_OA, // Open Addressing
    HT_PH, // Perfect Hashing
} HashType;

typedef struct Hash
{
    HashType type;
    unsigned capacity;
    void *impl;
} Hash;

/***********************************
 * Initialize the hash.
 *
 * Direct Addressing: the valid value should in the range
 * of [0, capacity).
 *
 * Closed Addressing: 
 *
 * Open Addressing: 
 *
 * Perfect Hashing:
 ***********************************/
bool HashInit(Hash *hashp, HashType type, unsigned capacity/*, ...*/);

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

