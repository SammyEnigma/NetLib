#ifndef _NODE_C
#define _NODE_C

#include <stddef.h>

typedef struct Node
{
    void* Data;
    size_t Size;
    struct Node* Next;
    struct Node* Previous;
}Node;

Node* nodeAllocate(void* data, size_t sz);

void nodeFree(Node* ptr);

void nodeCopy(Node* dest, Node* orig);

int nodeAreEqual(Node* first, Node* second);

int nodeIsGreater(Node* first, Node* second);

int nodeIsLess(Node* first, Node* second);

#endif
