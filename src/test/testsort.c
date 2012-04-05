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

void 
TestSortArray(void (*sortp)(int *, int, int),
        int *a, unsigned length, int begin, int end)
{
    // avoid the sorted result affecting the following
    // tests.
    int *numbers = alloca(sizeof(int)*length);
    memcpy(numbers, a, sizeof(int)*length);

    PrintArrayInt("Before sort", numbers, length);
    (*sortp)(numbers, begin, end);
    PrintArrayInt("After sort", numbers, length);
    if (!IsArraySorted(numbers, begin, end))
        printf("Error: Array is not sorted!!\n");
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

    return 0;
}
