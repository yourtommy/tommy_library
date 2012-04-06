#ifndef QUEUE_H__
#define QUEUE_H__

#include <stdbool.h>

#define QUEUE_MAX_CAPACITY 4095

struct Queue
{
    int head;
    int tail;
    // Leave a slot unallocated to distinguish 
    // "empty" and "full".
    int array[QUEUE_MAX_CAPACITY + 1];
};

void QueueInit(struct Queue *q);
bool QueueEmpty(struct Queue *q);
bool QueueFull(struct Queue *q);
bool Enqueue(struct Queue *q, int e);
bool Dequeue(struct Queue *q);
int QueueHead(struct Queue *q);

#endif

