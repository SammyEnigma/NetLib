#include "ListC.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

ListC *listAllocate(ListC* lptr)
{
    lptr = (ListC*)malloc(sizeof(ListC));
    memset(lptr, 0, sizeof(ListC));
    return lptr;
}

void listClear(ListC* lptr)
{
    if(lptr == NULL)
    {
        return;
    }
}

NodeC* listAddToHead(ListC* lptr, void* data, size_t sz)
{
    if(lptr == NULL)
    {
        lptr = listAllocate(lptr);
    }

    NodeC* ptr = nodeAllocate(data, sz);

    if(lptr->Count == 0)
    {
        lptr->IteratorPosition = lptr->Data = lptr->Head = lptr->Tail = ptr;
    }
    else
    {
        lptr->Head->Previous = ptr;
        ptr->Next = lptr->Head;
        lptr->Head = ptr;
    }

    lptr->Count++;

    return ptr;
}


NodeC* listAddToTail(ListC* lptr, void* data, size_t sz)
{
    if(lptr == NULL)
    {
        lptr = listAllocate(lptr);
    }

    NodeC* ptr = nodeAllocate(data, sz);

    if(lptr->Count == 0)
    {
        lptr->IteratorPosition = lptr->Data = lptr->Head = lptr->Tail = ptr;
    }
    else
    {
        lptr->Tail->Next = ptr;
        ptr->Previous = lptr->Tail;
        lptr->Tail = ptr;
    }

    lptr->Count++;

    return ptr;
}

NodeC* listInsert(ListC* lptr, void* data, size_t sz, int pos)
{
    if(lptr == NULL && pos == 0)
    {
        lptr = listAllocate(lptr);
    }
    else
    {
        if(lptr == NULL && pos > 0)
        {
            return NULL;
        }
    }

    if(pos > lptr->Count || pos < 0)
    {
        return NULL;
    }

    if(pos == 0)
    {
        return listAddToHead(lptr, data, sz);
    }

    if(pos == lptr->Count)
    {
        return listAddToTail(lptr, data, sz);
    }

    int idx = 1;

    NodeC* ptr = NULL;

    for(NodeC* curptr = lptr->Head ; curptr->Next != NULL; curptr = curptr->Next, idx++)
    {
        if(pos == idx)
        {
            ptr = nodeAllocate(data, sz);

            NodeC* prev = curptr->Previous;
            NodeC* next = curptr->Next;

            prev->Next = ptr;
            ptr->Previous = prev;

            next->Previous = ptr;
            ptr->Next = next;

            lptr->Count++;
            break;
        }

        idx++;
    }

    return ptr;
}

void listRemoveFromHead(ListC* lptr)
{
    if(lptr == NULL)
    {
        return;
    }

    NodeC* oldhead = lptr->Head;
    lptr->Head = lptr->Head->Next;
    lptr->Head->Previous = NULL;
    nodeFree(oldhead);
    lptr->Count--;
}

void listRemoveFromTail(ListC* lptr)
{
    if(lptr == NULL)
    {
        return;
    }

    NodeC* oldtail = lptr->Tail;
    lptr->Tail = oldtail->Previous;
    lptr->Tail->Next = NULL;
    nodeFree(oldtail);
    lptr->Count--;
}

void listRemove(ListC* lptr, const NodeC* node)
{
    if(lptr == NULL || node == NULL)
    {
        return;
    }

    for(NodeC* curptr = lptr->Head ; curptr->Next != NULL; curptr = curptr->Next)
    {
        if(node == curptr)
        {
            if(curptr->Next == NULL)
            {
                listRemoveFromTail(lptr);
            }

            if(curptr->Previous == NULL)
            {
                listRemoveFromHead(lptr);
            }

            NodeC* prev = curptr->Previous;
            NodeC* next = curptr->Next;

            prev->Next = next;
            next->Previous = prev;

            nodeFree(curptr);

            lptr->Count--;
            break;
        }
    }
}

void listRemoveAt(ListC* lptr, int pos)
{
    if(lptr == NULL || pos < 0)
    {
        return;
    }

    if(pos > lptr->Count -1)
    {
        return;
    }

    if(pos == lptr->Count -1)
    {
        listRemoveFromTail(lptr);
    }

    if(pos == 0)
    {
        listRemoveFromHead(lptr);
    }

    int idx = 1;
    for(NodeC* curptr = lptr->Head ; curptr->Next != NULL; curptr = curptr->Next, idx++)
    {
        if(idx == pos)
        {
            NodeC* prev = curptr->Previous;
            NodeC* next = curptr->Next;

            prev->Next = next;
            next->Previous = prev;

            nodeFree(curptr);

            lptr->Count--;
            break;
        }
    }
}

void listRemoveValue(ListC* lptr, void* data, size_t sz)
{
    if(lptr == NULL)
    {
        return;
    }

    NodeC* ptr = NULL;

    NodeC* node = nodeAllocate(data, sz);

    ptr = listGetFirst(lptr);

    int idx = 0;

    while(1)
    {
        if(ptr == NULL)
        {
            break;
        }

        if(nodeAreEqual(ptr, node))
        {
            if(idx == lptr->Count - 1)
            {
                listRemoveFromTail(lptr);
                break;
            }

            if(idx == 0)
            {
                listRemoveFromHead(lptr);
                break;
            }

            NodeC* prev = ptr->Previous;
            NodeC* next = ptr->Next;

            prev->Next = next;
            next->Previous = prev;

            nodeFree(ptr);
            lptr->Count--;
            break;
        }

        ptr = listGetNext(lptr);
        idx++;
    }

    nodeFree(node);
    return;
}

size_t listItemCount(ListC* lptr)
{
    if(lptr != NULL)
    {
        return lptr->Count;
    }

    return -1;
}

int listIndexOf(ListC *lptr, const NodeC* node)
{
    if(lptr == NULL)
    {
        return -1;
    }

    NodeC* ptr = NULL;

    ptr = listGetFirst(lptr);

    int idx = 0;

    if(ptr == node)
    {
        return idx;
    }

    while(1)
    {
        if(ptr == NULL)
        {
            break;
        }

        ptr = listGetNext(lptr);
        idx++;

        if(ptr == node)
        {
            return idx;
        }
    }

    return -1;
}

int listIndexOfValue(ListC* lptr, void* data, size_t sz)
{
    if(lptr == NULL)
    {
        return -1;
    }

    NodeC* ptr = NULL;

    NodeC* node = nodeAllocate(data, sz);

    ptr = listGetFirst(lptr);

    int idx = 0;

    if(nodeAreEqual(ptr, node))
    {
        nodeFree(node);
        return idx;
    }

    while(1)
    {
        if(ptr == NULL)
        {
            break;
        }

        ptr = listGetNext(lptr);
        idx++;

        if(nodeAreEqual(ptr, node))
        {
            nodeFree(node);
            return idx;
        }
    }

    nodeFree(node);
    return -1;
}

NodeC* listGetAt(ListC* lptr, int atpos)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    if(atpos > (lptr->Count - 1) || atpos < 0)
    {
        return NULL;
    }

    NodeC* ptr = NULL;

    ptr = listGetFirst(lptr);

    if(atpos > 0)
    {
        for(int idx = 0; idx < atpos; idx++)
        {
            ptr = listGetNext(lptr);
        }
    }

    return ptr;
}

NodeC* listGetFirst(ListC* lptr)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    lptr->IteratorPosition = lptr->Head;
    return lptr->IteratorPosition;
}

NodeC* listGetNext(ListC* lptr)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    lptr->IteratorPosition = lptr->IteratorPosition->Next;

    return lptr->IteratorPosition;
}

NodeC* listGetLast(ListC* lptr)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    lptr->IteratorPosition = lptr->Tail;
    return lptr->IteratorPosition;
}

NodeC* listGetPrevious(ListC* lptr)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    lptr->IteratorPosition = lptr->IteratorPosition->Previous;

    return lptr->IteratorPosition;
}

ListC *listSort(ListC* lptr)
{
    if(lptr == NULL)
    {
        return NULL;
    }

    return NULL;
}

ListC* listMerge(ListC* lptrFirst, ListC* lptrSecond)
{
    if(lptrFirst == NULL)
    {
        return lptrSecond;
    }

    if(lptrSecond == NULL)
    {
        return lptrFirst;
    }

    return NULL;
}

ListC* listJoin(ListC* lptrFirst, ListC* lptrSecond)
{
    if(lptrFirst == NULL)
    {
        return lptrSecond;
    }

    if(lptrSecond == NULL)
    {
        return lptrFirst;
    }

    return NULL;
}
