#include "datastructure.h"

void
StackInit(struct Stack *s)
{
    s->top = -1;
}

bool 
StackEmpty(struct Stack *s)
{
    return s->top == -1;
}

bool
StackFull(struct Stack *s)
{
    return s->top == STACK_MAX_CAPACITY - 1;
}

bool
StackPush(struct Stack *s, int e)
{
    if (StackFull(s))
        return false;
    s->top++;
    s->array[s->top] = e;
    return true;
}

bool
StackPop(struct Stack *s)
{
    if (s->top == -1)
        return false;
    --s->top;
    return true;
}

int
StackTop(struct Stack *s)
{
    return s->array[s->top];
}
