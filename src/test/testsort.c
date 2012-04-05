#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sort.h"
#include "utility.h"

#define TEST_SORT_ARRAY(sort, numbers, length, begin, end) { \
    printf("===%s===\n", #sort); \
    TestSortArray(&sort, numbers, length, begin, end); \
    printf("====================\n\n"); \
}

bool IsArraySorted(int *a, int begin, int end)
{
    int *p;
    for (p = a + begin + 1; p < a + end; p++)
        if (*p < *(p-1))
            return false;
    return true;
}


bool ArrayEqaulSortInsensitive(int *sorted, int *origin, int begin, int end) {
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
            return false;
    }
    return true;
}

void 
TestSortArray(void (*sortp)(int *, int, int),
        int *a, unsigned length, int begin, int end)
{
    // Avoid the sorted result affecting the following tests.
    int *numbers = alloca(sizeof(int)*length);
    memcpy(numbers, a, sizeof(int)*length);

    PrintArrayInt("Before sort", numbers, length);
    (*sortp)(numbers, begin, end);
    PrintArrayInt("After sort", numbers, length);
    if (!IsArraySorted(numbers, begin, end))
        printf("Error: Array is not sorted!!\n");
    if (!ArrayEqaulSortInsensitive(numbers, a, begin, end))
        printf("Error: Elements are not consistent!!\n");
}


int
main(void)
{
    unsigned length = GenerateRandomArrayLength(15, 20);
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, 0, 100);

    const int begin = 2;
    const int end = length - 2;   

    TEST_SORT_ARRAY(InsertionSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(MergeSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(HeapSortArray, numbers, length, begin, end);
    TEST_SORT_ARRAY(QuickSortArray, numbers, length, begin, end);

    return 0;
}
