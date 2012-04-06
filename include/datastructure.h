#ifndef DATA_STRUCTURE_H__
#define DATA_STRUCTURE_H__
#include <stdbool.h>

#define STACK_MAX_CAPACITY 4096
#define QUEUE_MAX_CAPACITY 4096

struct Stack
{
    int top;
    int array[STACK_MAX_CAPACITY];
};

void StackInit(struct Stack *s);
bool StackEmpty(struct Stack *s);
bool StackFull(struct Stack *s);
bool StackPush(struct Stack *s, int e);
int StackTop(struct Stack *s);
bool StackPop(struct Stack *s);


struct Queue
{
    int head;
    int tail;
    int array[QUEUE_MAX_CAPACITY]; 
};

void QueueInit(struct Queue *q);
bool QueueEmpty(struct Queue *q);
bool QueueFull(struct Queue *q);
bool Enqueue(struct Queue *q, int e);
bool Dequeue(struct Queue *q);
int QueueHead(struct Queue *q);

#endif

