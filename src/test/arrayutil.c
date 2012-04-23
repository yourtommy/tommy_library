#include "utility.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

unsigned int
GenerateRandomArrayLength(unsigned int min, unsigned int max)
{
    srand(time(0));
    return rand() % (max-min+1) + min;
}

void
GenerateRandomArrayInt(int *array, unsigned int length, int min, int max)
{
    srand(time(0));

    unsigned int i;
    for (i = 0; i < length; i++)
        array[i]=rand() % (max-min+1) + min;
}

void 
ShuffleArrayInt(int *array, unsigned length)
{
    srand(time(0));
    for (unsigned i = 0; i < length-1; i++) {
        int swap = rand() % (length-i-1) + (i+1);
        int temp = array[swap];
        array[swap] = array[i];
        array[i] = temp;
    }
}

void 
PrintArrayInt(char *title, int *array, unsigned int length)
{
    unsigned int i;
    printf("%s: ", title);
    for (i = 0; i < length; i++)
        printf("%d ", array[i]); 
    printf("\n\n");
}
