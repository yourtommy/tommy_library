#include "sort.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

void
CountingSortArray(int *a, int begin, int end)
{
    int countn = (CountingSortMax - CountingSortMin + 1); 
    int *counta = alloca(sizeof(int) * countn);
    memset(counta, 0, sizeof(int) * countn);

    int len = end - begin;
    int *copya = alloca(sizeof(int) * len);
    memcpy(copya, a + begin, sizeof(int) * len);

    int i;
    for (i = 0; i < len; i++)
        counta[copya[i] - CountingSortMin]++;
    for (i = 1; i < countn; i++)
        counta[i] += counta[i-1];
    for (i = len-1; i >= 0; i--) {
        bool valid = copya[i] >= CountingSortMin
            && copya[i] <= CountingSortMax;
        assert(valid);
        if (!valid)
            return;
        a[begin + counta[copya[i] - CountingSortMin] - 1] = copya[i];
        counta[copya[i] - CountingSortMin]--;
    }
}
