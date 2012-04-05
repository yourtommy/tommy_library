#ifndef SORT_H__
#define SORT_H__

/* 'a[end]' is not part of 'a', but the element
 * right after the last one.
 */
void InsertionSortArray(int *a, int begin, int end);
void MergeSortArray(int *a, int begin, int end);
void HeapSortArray(int *a, int begin, int end);
void QuickSortArray(int *a, int begin, int end);
/* Only support a range of 
 * [CountingSortMin, CountingSortMax]
 */ 
static const int CountingSortMin = -1000;
static const int CountingSortMax = 1000;
void CountingSortArray(int *a, int begin, int end);

#endif

