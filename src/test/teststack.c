#include "datastructure.h"
#include "testset.h"
#include "utilitytest.h"
#include <stdbool.h>
#include <stdio.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58023;
static const int TestMaxValue = 59321;

bool TestStackInit(struct Stack *s, char error[])
{
    StackInit(s);
    if (!StackEmpty(s)) {
        sprintf(error, "Stack is not empty after init");
        return false;
    }
    return true;
}

bool TestStackPush(struct Stack *s, int *numbers, int length, char error[])
{
    int i;
    for (i = 0; i < length; i++) {
        StackPush(s, numbers[i]);
        if (StackTop(s) != numbers[i]) {
            sprintf(error, "Stack top (%d) != [%d](%d)", StackTop(s), i, numbers[i]);
            return false;
        }
    }
    return true;    
}

bool TestStackPop(struct Stack *s, int *numbers, int length, char error[])
{
    int i;
    for (i = length-1; i >= 0; i--) {
        if (StackTop(s) != numbers[i]) {
            sprintf(error, "Stack top (%d) != [%d](%d)", StackTop(s), i, numbers[i]);
            return false;
        }
        if (!StackPop(s)) {
            sprintf(error, "Stack pop failed");
            return false;
        }
    }
    if (!StackEmpty(s)) {
        sprintf(error, "Stack is not empty after poping all elements");
        return false;
    }
    // empty now 
    if (StackPop(s)) {
        sprintf(error, "Stack is empty but allows to pop");
        return false;
    }
    
    return true;    
}

bool TestStackPressure(struct Stack *s, char error[])
{
    int i = 0;
    for (i = 0; i < STACK_MAX_CAPACITY; i++) {
        if (!StackPush(s, i)) {
            sprintf(error, "Stack cannot push before reaching max capacity");
            return false;
        }
    }
    if (!StackFull(s)) {
        sprintf(error, "Stack is not full after reaching max capacity");
        return false;
    }
    for (i = 0; i < 100; i++) {
        if (StackPush(s, i)) {
            sprintf(error, "Stack can push after reaching max capacity");
            return false;
        }
    }
    if (!StackFull(s)) {
        sprintf(error, "Stack is not full after trying to push after reaching max capacity");
        return false;
    }
    if (!StackPop(s)) {
        sprintf(error, "Cannot pop when stack is full");
        return false;
    }
    if (!StackPush(s, 5)) {
        sprintf(error, "Cannot push after pop");
        return false;
    }
    return true;
}

void
TestStack()
{
    struct Stack s;
    char error[1024];
    if (!TestStackInit(&s, error)) {
        printf("StackInit failed!! Error: %s\n", error);
        return;
    }

    unsigned length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int numbers[length];
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    if (!TestStackPush(&s, numbers, length, error)) {
        printf("TestStackPush failed!! Error: %s\n", error);
        return;
    }
    if (!TestStackPop(&s, numbers, length, error)) {
        printf("TestStackPop failed!! Error: %s\n", error);
        return;
    }
    if (!TestStackPressure(&s, error)) {
        printf("TestStackPressure failed!! Error: %s\n", error);
        return;
    }
        
    printf("TestStack passed!!\n");
}
