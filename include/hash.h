#ifndef HASH_H__
#define HASH_H__

#include <stdbool.h>

typedef enum HashType
{
    HT_DA, // Direct Addressing
    HT_CA, // Closed Addressing
    HT_OA, // Open Addressing
    HT_PH, // Perfect Hashing
} HashType;

typedef struct Hash
{
    HashType type;
    unsigned capacity;
} Hash;

/***********************************
 * Initialize the hash.
 *
 * Direct Addressing: the valid value should in the range
 * of [0, capacity).
 *
 * Closed Addressing: the forth optional argument is the
 * used hashing funciton whose prototype is
 * "int (*func)(int value)".
 *
 * Open Addressing: the forth optional argument is the 
 * used hashing function whose prototype is
 * "int (*hash_func)(int value, int times);
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

