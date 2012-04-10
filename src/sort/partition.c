#include "sortprivate.h"

int 
Partition(int *a, int begin, int end)
{
    int i, j;
    for (i = begin, j = begin-1; i < end - 1; i++) {
        if (a[i] < a[end-1]) {
            j++;
            int tmp = a[j];
            a[j] = a[i];
            a[i] = tmp;
        }
    }
    int tmp = a[end-1];
    a[end-1] = a[j+1];
    a[j+1] = tmp;

    return j+1;
    /*
    int key = a[end-1];
    int i = begin, j = end - 2;

    while (i < j) {
        if (a[i] < key)
            i++;
        else {
            int tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            j--;
        }
    }
    int mid;
    if (a[i] < key) 
        mid = i+1;
    else
        mid = i;

    a[end-1] = a[mid];
    a[mid] = key;
    return mid;
    */
}
