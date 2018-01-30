#ifndef _LIST_C
#define _LIST_C

#include "NodeC.h"

typedef struct ListC
{
    size_t Count;
    NodeC* Data;
    NodeC* Head;
    NodeC* Tail;
    NodeC* IteratorPosition;
}ListC;

ListC* listAllocate(ListC* lptr);
void listClear(ListC* lptr);

NodeC* listAddToHead(ListC* lptr, void* data, size_t sz);
NodeC* listAddToTail(ListC* lptr, void* data, size_t sz);
NodeC* listInsert(ListC* lptr, void* data, size_t sz, int pos);

void listRemoveFromHead(ListC* lptr);
void listRemoveFromTail(ListC* lptr);
void listRemove(ListC* lptr, const NodeC* node);
void listRemoveAt(ListC* lptr, int pos);
void listRemoveValue(ListC* lptr, void* data, size_t sz);

size_t listItemCount(ListC* lptr);
int listIndexOf(ListC* lptr, const NodeC* node);
int listIndexOfValue(ListC* lptr, void* data, size_t sz);
NodeC* listGetAt(ListC* lptr, int atpos);

NodeC* listGetFirst(ListC* lptr);
NodeC* listGetLast(ListC* lptr);
NodeC* listGetNext(ListC* lptr);
NodeC* listGetPrevious(ListC* lptr);

ListC* listSort(ListC* lptr);
ListC* listMerge(ListC* lptrFirst, ListC* lptrSecond);
ListC* listJoin(ListC* lptrFirst, ListC* lptrSecond);

#endif
