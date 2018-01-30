#include "StackC.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

StackC *stackAllocate(StackC* sptr)
{
    sptr = (StackC*)malloc(sizeof(StackC));
    memset(sptr, 0, sizeof(StackC));
    return sptr;
}

void stackClear(StackC* sptr)
{
    if(sptr == NULL)
    {
        return;
    }
}

NodeC* stackPush(StackC* sptr, void* data, size_t sz)
{
    if(sptr == NULL)
    {
        sptr = stackAllocate(sptr);
    }

    NodeC* ptr = nodeAllocate(data, sz);

    if(sptr->Count == 0)
    {
        sptr->Data = sptr->Head = sptr->Tail = ptr;
    }
    else
    {
        sptr->Tail->Next = ptr;
        ptr->Previous = sptr->Tail;
        sptr->Tail = ptr;
    }

    sptr->Count++;

    return ptr;
}

NodeC *stackPop(StackC* sptr)
{
    if(sptr == NULL)
    {
        return NULL;
    }

    NodeC* oldtail = sptr->Tail;
    sptr->Tail = oldtail->Previous;
    sptr->Tail->Next = NULL;
    sptr->Count--;

    //nodeFree(oldtail);
    return oldtail;
}

size_t stackItemCount(StackC *sptr)
{
    if(sptr != NULL)
    {
        return sptr->Count;
    }

    return -1;
}
