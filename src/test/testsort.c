#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sort.h"
#include "utility.h"

#define TEST_SORT_ARRAY(sort, numbers, length) { \
    printf("===%s===\n", #sort); \
    TestSortArray(&sort, numbers, length); \
    printf("====================\n\n"); \
}

bool IsArraySorted(int *a, unsigned int length)
{
    int *p;
    for (p = a + 1; p < a + length; p++)
        if (*p < *(p-1))
            return false;
    return true;
}

void 
TestSortArray(void (*sortp)(int *, int, int), int *a,
        unsigned int length)
{
    // avoid the sorted result affecting the following
    // tests.
    int *numbers = alloca(sizeof(int)*length);
    memcpy(numbers, a, sizeof(int)*length);

    PrintArrayInt("Before sort", numbers, length);
    (*sortp)(numbers, 0, length);
    PrintArrayInt("After sort", numbers, length);
    if (!IsArraySorted(numbers, length))
        printf("Error: Array is not sorted!!\n");
}


int
main(void)
{
    unsigned length = GenerateRandomArrayLength(15, 20);
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, 0, 100);

    TEST_SORT_ARRAY(InsertionSortArray, numbers, length);
    TEST_SORT_ARRAY(MergeSortArray, numbers, length);

    return 0;
}
