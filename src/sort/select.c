#include "sort.h"
#include "sortprivate.h"
#include <stdlib.h>
#include <string.h>

int 
Select(int *array, int begin, int end, int n)
{
    if (n < 1 || n > end - begin)
        return -1;
    int *a = alloca(sizeof(int)*(end-begin));
    memcpy(a, array+begin, sizeof(int)*(end-begin));

    int target = n-1; // index from 0, so a[n-1] is the n'th smallest element.
    int b = 0;
    int e = end-begin;
    int mid = Partition(a, b, e);
    while (mid != target) {
        if (mid > target) {
            e = mid;
            mid = Partition(a, b, e);
        }
        else {
            b = mid+1;
            mid = Partition(a, b, e);
        }
    }
    return a[mid];
}

