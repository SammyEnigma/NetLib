#ifndef _QUEUE_C
#define _QUEUE_C

#include "NodeC.h"

typedef struct QueueC
{
    size_t Count;
    NodeC* Data;
    NodeC* Head;
    NodeC* Tail;
}QueueC;

QueueC* queueAllocate(QueueC *qptr);
void queueClear(QueueC* qptr);

NodeC* queueEnqueue(QueueC* qptr, void* data, size_t sz);
NodeC* queueDenqueue(QueueC* qptr);
size_t queueItemCount(QueueC* qptr);

#endif
