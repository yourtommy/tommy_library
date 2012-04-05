#include "sort.h"

void
Heapfy(int *a, int begin, int cur, int end)
{
    int leftc = begin + (cur-begin)*2 + 1;
    int rightc = begin + (cur-begin)*2 + 2;
    int largest = cur;
    if (leftc < end && a[leftc] > a[largest])
        largest = leftc;
    if (rightc < end && a[rightc] > a[largest])
        largest = rightc;

    if (largest != cur) {
        int tmp = a[largest];
        a[largest] = a[cur];
        a[cur] = tmp;
        Heapfy(a, begin, largest, end);
    }
}

void
BuildHeap(int *a, int begin, int end)
{
    int i;
    for (i = (begin+end)/2; i >= begin; i--)
        Heapfy(a, begin, i, end);
}

void
HeapSortArray(int *a, int begin, int end)
{
    BuildHeap(a, begin, end);

    int i, e;
    for (i = end - 1, e = end; i >= begin; i--) {
        int tmp = a[i];
        a[i] = a[begin];
        a[begin] = tmp;
        e--;
        Heapfy(a, begin, begin, e);
    }
}
