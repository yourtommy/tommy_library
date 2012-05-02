#ifndef UTILITY_H__
#define UTILITY_H__
#include <stdbool.h>

#define INFINITY_INT    ((int)(~(unsigned int)0 >> 1))

// oop
#define SUPER_MEMBER            __super
#define SUPER_PTR(objptr)       (&((objptr)->SUPER_MEMBER))
#define SUPER(obj)              ((obj).SUPER_MEMBER)

#define UNUSED(p) (void)(p)

unsigned MaxPrimeNum(unsigned n);

static inline unsigned int
RoundRightShift(unsigned n, unsigned bits)
{
    return (n >> bits) | (n << (sizeof(int) * 8 - bits));
}

#endif
