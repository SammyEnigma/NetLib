#include "QueueC.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

QueueC *queueAllocate(QueueC* qptr)
{
    qptr = (QueueC*)malloc(sizeof(QueueC));
    memset(qptr, 0, sizeof(QueueC));
    return qptr;
}

void queueClear(QueueC *qptr)
{
    if(qptr == NULL)
    {
        return;
    }
}

NodeC* queueEnqueue(QueueC *qptr, void* data, size_t sz)
{
    if(qptr == NULL)
    {
        qptr = queueAllocate(qptr);
    }

    NodeC* ptr = nodeAllocate(data, sz);

    if(qptr->Count == 0)
    {
        qptr->Data = qptr->Head = qptr->Tail = ptr;
    }
    else
    {
        qptr->Head->Previous = ptr;
        ptr->Next = qptr->Head;
        qptr->Head = ptr;
    }

    qptr->Count++;

    return ptr;
}

NodeC* queueDenqueue(QueueC *qptr)
{
    if(qptr == NULL)
    {
        return NULL;
    }

    NodeC* oldtail = qptr->Tail;
    qptr->Tail = oldtail->Previous;
    qptr->Tail->Next = NULL;
    qptr->Count--;

    //nodeFree(oldtail);
    return oldtail;
}

size_t queueItemCount(QueueC *qptr)
{
    if(qptr != NULL)
    {
        return qptr->Count;
    }

    return -1;
}
