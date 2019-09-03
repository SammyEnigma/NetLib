#ifndef _LIST_C
#define _LIST_C

#include "Node.h"

typedef struct List
{
    size_t Count;
    Node* Head;
    Node* Tail;
    Node* IteratorPosition;
}List;

List* listAllocate(List* lptr);
void listClear(List* lptr);

Node* listAddToHead(List* lptr, void* data, size_t sz);
Node* listAddToTail(List* lptr, void* data, size_t sz);
Node* listInsert(List* lptr, void* data, size_t sz, int pos);

void listRemoveFromHead(List* lptr);
void listRemoveFromTail(List* lptr);
void listRemove(List* lptr, const Node* node);
void listRemoveAt(List* lptr, int pos);
void listRemoveValue(List* lptr, void* data, size_t sz);

size_t listItemCount(List* lptr);
int listIndexOf(List* lptr, const Node* node);
int listIndexOfValue(List* lptr, void* data, size_t sz);
Node* listGetAt(List* lptr, int atpos);

Node* listGetFirst(List* lptr);
Node* listGetLast(List* lptr);
Node* listGetNext(List* lptr);
Node* listGetPrevious(List* lptr);

List* listSort(List* lptr);
List* listMerge(List* lptrFirst, List* lptrSecond);
List* listJoin(List* lptrFirst, List* lptrSecond);

#endif
