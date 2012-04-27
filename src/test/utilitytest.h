#ifndef UTILITY_TEST_H__
#define UTILITY_TEST_H__

#include "utility.h"

unsigned int GenerateRandomArrayLength(unsigned int min, unsigned int max);

void GenerateRandomArrayInt(int *array, unsigned int length, int min, int max);

void GenerateUniqueRandomArrayInt(int *array, unsigned int length, int min, int max);

void ShuffleArrayInt(int *array, unsigned length);

void PrintArrayInt(char *title, int *array, unsigned int length);

int UnsortedIndex(int *a, int begin, int end);

int FindMissingElement(int *origin, int *sorted, int begin, int end); 

#endif

