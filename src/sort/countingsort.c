#include "sort.h"
#include "sortprivate.h"
#include <stdbool.h>
#include <string.h>
#include <assert.h>


void
CountingSortArray(int *a, int begin, int end)
{
    int min, max;
    min = max = a[begin];
    int i;
    for (i = begin + 1; i < end; i++) {
        if (a[i] < min)
            min = a[i];
        else if (a[i] > max)
            max = a[i];
    }
    CountingSortArrayWithConvert(a, begin, end, min, max, 0);
}

static int
NoConvert(int value)
{
    return value;
}

/* A mask is used to sort part of the value, normally for radix sort. */
void
CountingSortArrayWithConvert(int *a, int begin, int end, 
        int min, int max, int (*convert)(int))
{
    if (!convert)
        convert = &NoConvert;

    int countn = (max - min + 1); 
    int counta[countn];
    memset(counta, 0, sizeof(int) * countn);

    int len = end - begin;
    int copya[len];
    memcpy(copya, a + begin, sizeof(int) * len);

    int i;
    for (i = 0; i < len; i++)
        counta[(*convert)(copya[i]) - min]++;
    for (i = 1; i < countn; i++)
        counta[i] += counta[i-1];
    for (i = len-1; i >= 0; i--) {
        bool valid = (*convert)(copya[i]) >= min
            && (*convert)(copya[i]) <= max;
        assert(valid);
        if (!valid)
            return;
        a[begin + counta[(*convert)(copya[i]) - min] - 1] = copya[i];
        counta[(*convert)(copya[i]) - min]--;
    }
}
