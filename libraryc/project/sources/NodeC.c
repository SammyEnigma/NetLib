#include "NodeC.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

NodeC* nodeAllocate(void* data, size_t sz)
{
    NodeC* nd = (NodeC*)calloc(1, sizeof(NodeC));
    nd->Data = (char*)calloc(1, sz);
    nd->Size = sz;
    nd->Next = NULL;
    nd->Previous = NULL;
    memcpy(nd->Data, data, sz);
    return nd;
}

void nodeFree(NodeC* ptr)
{
    free(ptr->Data);
    free(ptr);
}

void nodeCopy(NodeC* dest, NodeC* orig)
{
    if( (dest != NULL && dest->Data != NULL)
            && (orig != NULL && orig->Data != NULL) )
    {
        free(dest->Data);
        dest->Size = orig->Size;
        dest->Data = (char*)calloc(1, dest->Size);
        memcpy(dest->Data, orig->Data, dest->Size);
    }
}

int nodeAreEqual(NodeC* first, NodeC* second)
{
    if( (first != NULL && first->Data != NULL)
            && (second != NULL && second->Data != NULL) )
    {
        if(first->Size != second->Size)
        {
            return 0;
        }

        if(memcmp(first->Data, second->Data, first->Size) == 0)
        {
            return 1;
        }
    }

    return 0;
}

int nodeIsGreater(NodeC* first, NodeC* second)
{
    if( (first != NULL && first->Data != NULL)
            && (second != NULL && second->Data != NULL) )
    {
        if(first->Size != second->Size)
        {
            return 0;
        }

        if(memcmp(first->Data, second->Data, first->Size) > 0)
        {
            return 1;
        }
    }

    return 0;
}

int nodeIsLess(NodeC* first, NodeC* second)
{
    if( (first != NULL && first->Data != NULL)
            && (second != NULL && second->Data != NULL) )
    {
        if(first->Size != second->Size)
        {
            return 0;
        }

        if(memcmp(first->Data, second->Data, first->Size) < 0)
        {
            return 1;
        }
    }

    return 0;
}

