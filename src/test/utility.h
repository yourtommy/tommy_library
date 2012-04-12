#ifndef UTILITY_H

unsigned int GenerateRandomArrayLength(unsigned int min, unsigned int max);

void GenerateRandomArrayInt(int *array, unsigned int length, int min, int max);

void PrintArrayInt(char *title, int *array, unsigned int length);

int UnsortedIndex(int *a, int begin, int end);

int FindMissingElement(int *origin, int *sorted, int begin, int end); 

#endif

