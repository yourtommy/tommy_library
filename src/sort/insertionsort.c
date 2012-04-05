#include "sort.h"

void
InsertionSortArray(int *a, int begin, int end)
{
    int *p1, *p2;
    for (p1 = a + begin + 1; p1 < a + end; p1++) {
        int key = *p1;
        p2 = p1 - 1;
        while (p2 >= a + begin && *p2 > key) {
            *(p2+1) = *p2;
            p2--;
        }
        *(p2+1) = key; 
    } 
}
