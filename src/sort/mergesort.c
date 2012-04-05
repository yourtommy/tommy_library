#include "sort.h"
#include <stdlib.h>

const int Infinity = ~(unsigned int)0 >> 1;

/*
 * mid belongs to the right part
 */
void
MergeArray(int *a, int begin, int mid, int end)
{
    int leftn = mid - begin;
    int *left = alloca((leftn+1)*sizeof(int));
    int i;
    for (i = 0; i < leftn; i++)
        left[i] = a[begin+i];
    left[leftn] = Infinity;

    int rightn = end - mid;
    int *right = alloca((rightn+1)*sizeof(int));
    for (i = 0; i < rightn; i++)
        right[i] = a[mid+i];
    right[rightn] = Infinity;
    
    int lefti = 0, righti=0;
    for (i = begin; i < end; i++) {
        if (left[lefti] <= right[righti]) {
            a[i] = left[lefti];
            lefti++;
        } else {
            a[i] = right[righti];
            righti++;
        }
    }
}

void 
MergeSortArray(int *a, int begin, int end)
{
    if ((end - begin) <= 1)
        return;
    int mid = (begin + end) / 2;
    MergeSortArray(a, begin, mid);
    MergeSortArray(a, mid, end);
    MergeArray(a, begin, mid, end);
}
