#include "queue.h"

#define QUEUE_MAX_ARRAY_SIZE    (QUEUE_MAX_CAPACITY+1)

inline static int IncreaseIndex(int i)
{
    if (i == QUEUE_MAX_ARRAY_SIZE - 1)
        return 0;
    else
        return i+1;
}

void QueueInit(struct Queue *q)
{
    q->head = 0;
    q->tail = 0;
}

bool QueueEmpty(struct Queue *q)
{
    return q->head == q->tail;
}

bool QueueFull(struct Queue *q)
{
    return IncreaseIndex(q->tail) == q->head;
}

bool Enqueue(struct Queue *q, int e)
{
    if (QueueFull(q))
        return false;
    q->array[q->tail] = e;
    q->tail = IncreaseIndex(q->tail);
    return true;
}

bool Dequeue(struct Queue *q)
{
    if (QueueEmpty(q))
        return false;
    q->head = IncreaseIndex(q->head);
    return true;
}

int QueueHead(struct Queue *q)
{
    if (QueueEmpty(q))
        return (~(int)0)>>1; // INFINITY
    return q->array[q->head];
}
