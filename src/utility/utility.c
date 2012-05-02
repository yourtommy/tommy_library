#include "utility.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define units_of_block  1024
typedef uint8_t unit_t;
#define bits_of_unit    (sizeof(unit_t) * 8)
#define block_total     1024

static unit_t *prime_bitmap[block_total];
static unsigned block_index_to_calculate = 0;

/* TODO: very wrong here!
static bool
PopulateFirst32Bits(int block_index)
{
    unsigned two 	= 0x55555555; // 0101 0101 0101 0101 0101 0101 0101 0101
    unsigned three 	= 0x6DB6DB6D; // 0110 1101 1011 0110 1101 1011 0110 1101
    unsigned five 	= 0xFBDEF7BD; // 0111 1011 1101 1110 1111 0111 1011 1101
    unsigned seven 	= 0xFDFBF7EF; // 0111 1110 1111 1101 1111 1011 1111 0111
    unsigned eleven 	= 0xFFDFFBFF; // 1111 1111 1101 1111 1111 1011 1111 1111
    unsigned thirteen 	= 0xFFF7FFBF; // 1111 1111 1111 0111 1111 1111 1011 1111

    // TODO: shift correctly according to the block index

    if (block_index == 0) {
	prime_bitmap[block_index][0] &= 0x; // 0011 0101 0001 0100 0101 0001 0000 0100
    }
    for (int i = 1; i < units_of_block; i++) {
	three = RoundRightShift(three, 3);
	five = RoundRightShift(five, 5);
	seven = RoundRightShift(seven, 7);
	eleven = RoundRightShift(eleven, 11);
	thirteen = RoundRightShift(thirteen, 13);
	prime_bitmap[block_index][i] &= (two & three & five & seven & eleven & thirteen);
    }

    return true;
}
*/

static bool
GetBitIndex(unsigned n, unit_t **unit_ptr_p, unsigned *bit_index_in_unit_p)
{
    unsigned block_index = n / (units_of_block * bits_of_unit); 
    unsigned bit_index_in_block = n - block_index * units_of_block * bits_of_unit;
    unsigned unit_index_in_block = bit_index_in_block / bits_of_unit;

    *unit_ptr_p = prime_bitmap[block_index] + unit_index_in_block;
    *bit_index_in_unit_p = bit_index_in_block - unit_index_in_block * bits_of_unit;

    return true;
}

static bool
IsPrime(unsigned n)
{
    unsigned bit_index_in_unit;
    unit_t *unit_ptr;
    GetBitIndex(n, &unit_ptr, &bit_index_in_unit);
    
    return (*unit_ptr >> (bits_of_unit - bit_index_in_unit - 1)) & 0x1;
}

static bool
UnsetPrime(unsigned n)
{
    unsigned bit_index_in_unit;
    unit_t *unit_ptr;
    GetBitIndex(n, &unit_ptr, &bit_index_in_unit);
    
    *unit_ptr &= ~(0x80/*000000*/ >> bit_index_in_unit);

    return true;
}
	
static bool
FillBlock(unsigned block_index)
{
    // TODO: make it more efficient
    unsigned up_limit = (block_index + 1) * units_of_block * bits_of_unit;
    for (unsigned i = 2; i < up_limit; i++) {
	if (IsPrime(i)) {
	    unsigned composite_num = i * 2;
	    while (composite_num < up_limit) {
		UnsetPrime(composite_num);
		assert(!IsPrime(composite_num));
		composite_num += i;
	    }
	}
    }

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
    unsigned index = n / (units_of_block * bits_of_unit); 

    // calculate to index
    while (block_index_to_calculate <= index) {
        prime_bitmap[block_index_to_calculate] = malloc(units_of_block * sizeof(unit_t));
        memset(prime_bitmap[block_index_to_calculate], 0xff, units_of_block * sizeof(unit_t));
	FillBlock(block_index_to_calculate);
        ++block_index_to_calculate;
    }

    return GetMaxPrimNum(n);
}


