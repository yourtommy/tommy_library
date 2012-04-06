#ifndef SORT_H__
#define SORT_H__

/* 'a[end]' is not part of 'a', but the element
 * right after the last one.
 */
void InsertionSortArray(int *a, int begin, int end);
void MergeSortArray(int *a, int begin, int end);
void HeapSortArray(int *a, int begin, int end);
void QuickSortArray(int *a, int begin, int end);
/* Require min & max specified  */
void CountingSortArray(int *a, int begin, int end, int min, int max);
/* Only support positive integers */
void RadixSortArray(int *a, int begin, int end);

#endif

