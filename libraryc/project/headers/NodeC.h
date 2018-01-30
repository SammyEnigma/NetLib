#ifndef _NODE_C
#define _NODE_C

#include <stddef.h>

typedef struct NodeC
{
    void* Data;
    size_t Size;
    struct NodeC* Next;
    struct NodeC* Previous;
}NodeC;

NodeC* nodeAllocate(void* data, size_t sz);

void nodeFree(NodeC* ptr);

void nodeCopy(NodeC* dest, NodeC* orig);

int nodeAreEqual(NodeC* first, NodeC* second);

int nodeIsGreater(NodeC* first, NodeC* second);

int nodeIsLess(NodeC* first, NodeC* second);

#endif
