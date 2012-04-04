#include <stdio.h>
#include <stdlib.h>
#include "sort.h"
#include "utility.h"


void TestInsertionSort(unsigned int length, int min_value, int max_value)
{
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, min_value, max_value);
    PrintArrayInt("Before sort", numbers, length);
    InsertionSortArray(numbers, 0, length);
    PrintArrayInt("After sort", numbers, length);
}

int
main(void)
{
    TestInsertionSort(GenerateRandomArrayLength(5, 10), -100, 100);
    
	return 0;
}
