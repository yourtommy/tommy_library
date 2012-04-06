#ifndef DATA_STRUCTURE_H__
#define DATA_STRUCTURE_H__
#include <stdbool.h>

#define STACK_MAX_CAPACITY 4096

struct Stack
{
    int top;
    int array[STACK_MAX_CAPACITY];
};

void StackInit(struct Stack *s);
bool StackEmpty(struct Stack *s);
bool StackPush(struct Stack *s, int e);
int StackTop(struct Stack *s);
bool StackPop(struct Stack *s);

#endif

