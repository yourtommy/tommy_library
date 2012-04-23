#include "datastructure.h"
#include "utilitytest.h"
#include <stdbool.h>
#include <stdio.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58023;
static const int TestMaxValue = 59321;

bool TestQueueInit(struct Queue *q, char error[])
{
    QueueInit(q);
    if(!QueueEmpty(q)) {
        sprintf(error, "Queue is not empty after init");
        return false;
    }
    return true;
}

bool TestEnqueue(struct Queue *q, int *numbers, int length, char error[])
{
    int i;
    for (i = 0; i < length; i++) {
        if (!Enqueue(q, numbers[i])) {
            sprintf(error, "Enqueue [%d](%d) failed.", i, numbers[i]);
            return false;
        }
        int tail = q->array[(q->tail + QUEUE_MAX_CAPACITY - 1) % QUEUE_MAX_CAPACITY];
        if (tail != numbers[i]) {
            sprintf(error, "Queue tail (%d) != [%d](%d)", tail, i, numbers[i]);
            return false;
        }
    }
    return true;    
}

bool TestDequeue(struct Queue *q, int *numbers, int length, char error[])
{
    int i;
    for (i = 0; i < length; i++) {
        if (QueueHead(q) != numbers[i]) {
            sprintf(error, "Queue head (%d) != [%d](%d)", QueueHead(q), i, numbers[i]);
            return false;
        }
        if (!Dequeue(q)) {
            sprintf(error, "Deque failed");
            return false;
        }
    }
    if (!QueueEmpty(q)) {
        sprintf(error, "Queue is not empty after dequeing all elemements");
        return false;
    }
    // empty now 
    if (Dequeue(q)) {
        sprintf(error, "Queue is empty but allows dequeue");
        return false;
    }
    
    return true;    
}

bool TestQueuePressure(struct Queue *q, char error[])
{
    int i = 0;
    for (i = 0; i < QUEUE_MAX_CAPACITY; i++) {
        if (!Enqueue(q, i)) {
            sprintf(error, "Cannot enqueue before reaching max capacity [%d]", i);
            return false;
        }
    }
    if (!QueueFull(q)) {
        sprintf(error, "Queue is not full after reaching max capacity");
        return false;
    }
    for (i = 0; i < 100; i++) {
        if (Enqueue(q, i)) {
            sprintf(error, "Queue can push after reaching max capacity");
            return false;
        }
    }
    if (!QueueFull(q)) {
        sprintf(error, "Queue is not full after trying to push after reaching max capacity");
        return false;
    }
    if (!Dequeue(q)) {
        sprintf(error, "Cannot pop when stack is full");
        return false;
    }
    if (!Enqueue(q, 5)) {
        sprintf(error, "Cannot push after pop");
        return false;
    }
    return true;
}

void
TestQueue()
{
    struct Queue q;
    char error[1024] = "";
    if (!TestQueueInit(&q, error)) {
        printf("QueueInit failed!! Error: %s\n", error);
        return;
    }

    unsigned length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int numbers[length];
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    if (!TestEnqueue(&q, numbers, length, error)) {
        printf("TestEnqueue failed!! Error: %s\n", error);
        return;
    }
    if (!TestDequeue(&q, numbers, length, error)) {
        printf("TestDequeue failed!! Error: %s\n", error);
        return;
    }
    if (!TestQueuePressure(&q, error)) {
        printf("TestQueuePressure failed!! Error: %s\n", error);
        return;
    }
        
    printf("TestQueue passed!!\n");
}
