#ifndef _STACK_C
#define _STACK_C

#include "Node.h"

typedef struct Stack
{
    size_t Count;
    Node* Data;
    Node* Head;
    Node* Tail;
}Stack;

Stack* stackAllocate(Stack* sptr);
void Stacklear(Stack* sptr);
Node* stackPush(Stack* sptr, void* data, size_t sz);
Node* stackPop(Stack* sptr);
size_t stackItemCount(Stack* sptr);

#endif

