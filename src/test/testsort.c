#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sort.h"
#include "utility.h"

#define TEST_SORT_ARRAY(sort, numbers, length, begin, end) { \
    printf("===%s===\n", #sort); \
    TestSortArray(&sort, numbers, length, begin, end); \
    printf("====================\n\n\n"); \
}

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58000;
static const int TestMaxValue = 58000;

int UnsortedIndex(int *a, int begin, int end)
{
    int i;
    for (i = begin + 1; i < end; i++)
        if (a[i] < a[i-1])
            return i;
    return -1;
}


int FindMissingElement(int *origin, int *sorted, int begin, int end) {
    int i;
    for (i = begin; i < end; i++) {
        bool found = false;
        int left = begin, right = end - 1;
        while (!found && left <= right) {
            int mid = (left + right) / 2;
            if (origin[i] == sorted[mid])
                found = true;
            else if (origin[i] < sorted[mid])
                right = mid - 1;
            else
                left = mid + 1;
        }
        if (!found)
            return i;
    }
    return -1;
}

void 
TestSortArray(void (*sortp)(int *, int, int),
        int *a, unsigned length, int begin, int end)
{
    // Avoid the sorted result affecting the following tests.
    int *numbers = alloca(sizeof(int)*length);
    memcpy(numbers, a, sizeof(int)*length);

#ifdef TRACE
    PrintArrayInt("Before sort", numbers, length);
#endif
    (*sortp)(numbers, begin, end);
#if TRACE
    PrintArrayInt("After sort", numbers, length);
#endif
    int unsorted = -1;
    if ((unsorted = UnsortedIndex(numbers, begin, end) != -1)) {
        printf("Error: Array is not sorted [%d]!!\n", unsorted);
        return;
    }
    
    int missing = -1;
    if ((missing = FindMissingElement(a, numbers, begin, end)) != -1) {
        printf("Error: Element %d (index %d) is missing!!\n", numbers[missing], missing);
        return;
    }
    printf("Sort successful!!\n");
}

void
TestSelectArray(int *a, int begin, int end)
{
    int *sorted = alloca(sizeof(int)*(end - begin));
    memcpy(sorted, a+begin, sizeof(int)*(end-begin));
    QuickSortArray(sorted, 0, end-begin);

    int testk[] = {1, 5, end/2, end-begin-6, end-begin};
    unsigned int i;
    for (i = 0; i < sizeof(testk)/sizeof(testk[0]); i++) {
        int k = testk[i];
        int n = Select(a, begin, end, k); // find the 5th
        if (n == sorted[k-1]) 
            printf("Select %d'th successful!!\n", k);
        else
            printf("Select %d'th failed!!\n", k);
    }    
}

void
TestSort(void)
{
    unsigned length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    const int begin = 2;
    const int end = length - 2;   

    TEST_SORT_ARRAY(InsertionSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(MergeSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(HeapSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(QuickSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(CountingSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(RadixSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(BucketSortArray, numbers, length, begin, end);

    TestSelectArray(numbers, begin, end);
}
