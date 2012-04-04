#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sort.h"
#include "utility.h"

bool IsArraySorted(int *a, unsigned int length)
{
    int *p;
    for (p = a + 1; p < a + length; p++)
        if (*p < *(p-1))
            return false;
    return true;
}

void TestInsertionSortArray(unsigned int length, int min_value, int max_value)
{
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, min_value, max_value);
    PrintArrayInt("Before sort", numbers, length);
    InsertionSortArray(numbers, 0, length);
    PrintArrayInt("After sort", numbers, length);
    if (!IsArraySorted(numbers, length))
        printf("Error: Array is not sorted!!\n");
}

int
main(void)
{
    TestInsertionSortArray(GenerateRandomArrayLength(5, 10), 0, 100);
    return 0;
}
