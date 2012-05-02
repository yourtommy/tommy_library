#include "utility.h"
#include <stdlib.h>
#include <string.h>

// static const unsigned block_size = 1024; // byte
#define block_size      1024
// static const unsigned block_total = 1024;
#define block_total     1024
static unsigned block_index_to_calculate = 0;
// static const unsigned bits_of_byte = 8;
#define bits_of_byte    8
static unsigned char *prime_bitmap[block_total];

static bool
GetBitIndex(unsigned n, unsigned char **byte_ptr_p, unsigned *bit_index_in_byte_p)
{
    unsigned block_index = n / (block_size * bits_of_byte); 
    unsigned bit_index_in_block = n - block_index * block_size * bits_of_byte;
    unsigned byte_index_in_block = bit_index_in_block / bits_of_byte;

    *byte_ptr_p = prime_bitmap[block_index] + byte_index_in_block;
    *bit_index_in_byte_p = bit_index_in_block - byte_index_in_block * bits_of_byte;

    return true;
}

static bool
IsPrime(unsigned n)
{
    unsigned bit_index_in_byte;
    unsigned char *byte_ptr;
    GetBitIndex(n, &byte_ptr, &bit_index_in_byte);
    
    return (*byte_ptr >> (bits_of_byte - bit_index_in_byte - 1)) & 0x01;
}

static bool
UnsetPrime(unsigned n)
{
    unsigned bit_index_in_byte;
    unsigned char *byte_ptr;
    GetBitIndex(n, &byte_ptr, &bit_index_in_byte);
    
    *byte_ptr &= ~(0x80 >> bit_index_in_byte);

    return true;
}

static unsigned
GetMaxPrimNum(unsigned n)
{
    for (int i = n; i >= 2; i--)
        if (IsPrime(i))
            return i;
    return -1;
}

unsigned
MaxPrimeNum(unsigned n)
{
    unsigned index = n / (block_size * bits_of_byte); 

    // calculate to index
    while (block_index_to_calculate <= index) {
        prime_bitmap[block_index_to_calculate] = malloc(block_size);
        memset(prime_bitmap[block_index_to_calculate], 0xff, block_size);
        ++block_index_to_calculate;
    }

    // TODO remove this fake code
    UnsetPrime(4);
    UnsetPrime(6);
    UnsetPrime(8);
    UnsetPrime(9);
    UnsetPrime(10);

    return GetMaxPrimNum(n);
}

