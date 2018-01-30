#ifndef _STACK_C
#define _STACK_C

#include "NodeC.h"

typedef struct StackC
{
    size_t Count;
    NodeC* Data;
    NodeC* Head;
    NodeC* Tail;
}StackC;

StackC* stackAllocate(StackC* sptr);
void stackClear(StackC* sptr);
NodeC* stackPush(StackC* sptr, void* data, size_t sz);
NodeC* stackPop(StackC* sptr);
size_t stackItemCount(StackC* sptr);

#endif

