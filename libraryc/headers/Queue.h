#ifndef _QUEUE_C
#define _QUEUE_C

#include "Node.h"

typedef struct Queue
{
    size_t Count;
    Node* Data;
    Node* Head;
    Node* Tail;
}Queue;

Queue* queueAllocate(Queue *qptr);
void Queuelear(Queue* qptr);

Node* queueEnqueue(Queue* qptr, void* data, size_t sz);
Node* queueDenqueue(Queue* qptr);
size_t queueItemCount(Queue* qptr);

#endif
