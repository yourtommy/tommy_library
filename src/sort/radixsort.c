#include "sortprivate.h"
#include <stdbool.h>

static unsigned int rshift;
static bool convert_sign;
static int bits = 8; // works even if it's another value lie 7
static int mask;

int
CountingConvert(int value)
{
    if (!convert_sign)
        return (value >> rshift) & mask;
    if (value >= 0)
        return ((value >> rshift) | (1 << (bits-1))) & mask; // Guarantee positive values lager than negative values.
    else 
        return ((-value) >> rshift) & mask;
}

void
RadixSortArray(int *a, int begin, int end)
{
    mask = ~(~(unsigned)0 << bits);
    rshift = 0;
    convert_sign = false;
    while (rshift < sizeof(int) * 8) {
        if (rshift + bits >= sizeof(int) * 8) 
            convert_sign = true;
        CountingSortArrayWithConvert(a, begin, end, 0, mask, &CountingConvert);
        rshift += bits;
    }
}
