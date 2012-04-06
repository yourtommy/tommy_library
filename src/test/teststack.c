#include "datastructure.h"
#include "testset.h"
#include "utility.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58023;
static const int TestMaxValue = 59321;

bool TestStackInit(struct Stack *s)
{
    StackInit(s);
    return StackEmpty(s);
}

bool TestStackPush(struct Stack *s, int *numbers, int length)
{
    int i;
    for (i = 0; i < length; i++) {
        StackPush(s, numbers[i]);
        if (StackTop(s) != numbers[i]) 
            return false;
    }
    return true;    
}

bool TestStackPop(struct Stack *s, int *numbers, int length)
{
    int i;
    for (i = length-1; i >= 0; i--) {
        if (StackTop(s) != numbers[i]) 
            return false;
        if (!StackPop(s))
            return false;
    }
    if (!StackEmpty(s))
        return false;
    // empty now 
    if (StackPop(s))
        return false;
    
    return true;    
}

void
TestStack()
{
    struct Stack s;
    if (!TestStackInit(&s)) {
        printf("StackInit failed!!\n");
        return;
    }

    unsigned length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    if (!TestStackPush(&s, numbers, length)) {
        printf("TestStackPush failed!!\n");
        return;
    }
    if (!TestStackPop(&s, numbers, length)) {
        printf("TestStackPop failed!!\n");
        return;
    }
        
    printf("TestStack passed!!\n");
}
