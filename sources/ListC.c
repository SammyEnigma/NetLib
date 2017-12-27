#include "ListC.h"
#include <stdlib.h>
#include <string.h>
#include <memory.h>

void insertat(List *list, const void* data, int sz, int atpos)
{
    if (atpos > list->Count || atpos < 0)
    {
        return;
    }

    Node* newnode = (void*)malloc((size_t)sz);

    if (atpos == 0)
    {
        list->First = newnode;
        list->Last = newnode;
        list->Last->Next = NULL;
        list->First->Previous = NULL;
    }
    else
    {
        if (atpos >= list->Count)
        {
            list->Last->Next = newnode;
            newnode->Previous = list->Last;
            newnode->Next = NULL;
            list->Last = newnode;
        }
        else
        {
            int ctr = 0;
            Node* target = NULL;
            Node* targetsnext = NULL;

            for (target = list->First, ctr = 0; ctr < atpos; target = target->Next, ctr++) {}

            targetsnext = target->Next;
            newnode->Previous = target;
            newnode->Next = targetsnext;
            target->Next = newnode;

            if (targetsnext != NULL)
            {
                targetsnext->Previous = newnode;
            }
        }
    }
    list->Count++;

    list->IsSorted = 0;
}


void insert(List *list, const  void* data, int sz)
{
    Node* newnode = (void*)malloc((size_t)sz);

    // Special case for the first node
    if (list->First == NULL)
    {
        list->First = newnode;
        list->Last = newnode;
        list->Last->Next = NULL;
        list->First->Previous = NULL;
        list->Count++;
        return;
    }

    if (list->IsSorted)
    {
        // Locate the node before the point of insertion
        Node* target = list->First;
        Node* targetsnext = NULL;

        while (target->Next != NULL)
        {
            if ( memcmp(target->Next->Data, data, target->Next->Size) > 0)
            {
                break;
            }

            target = target->Next;
        }

        targetsnext = target->Next;
        newnode->Previous = target;
        newnode->Next = targetsnext;
        target->Next = newnode;

        if (targetsnext != NULL)
        {
            targetsnext->Previous = newnode;
        }
    }
    else
    {
        list->Last->Next = newnode;
        newnode->Previous = list->Last;
        newnode->Next = NULL;
        list->Last = newnode;
    }
}

void append(List *list, const  void* data, int sz)
{
    insertat(list, data, sz, list->Count - 1);
}

void appendlist(List *list, const List *newlist)
{
}


List* mergelists(const List *list1, const List *list2)
{
    return NULL;
}


void* removeat(List *list, int atpos)
{
    void* ret = NULL;

    if (atpos > list->Count - 1 || atpos < 0)
    {
        return ret;
    }

    int ctr = 0;

    if (atpos == 0)
    {
        Node* temp = list->First;
        list->First->Next->Previous = NULL;
        list->First = list->First->Next;
        ret = temp->Data;
        free(temp);

    }
    else
    {
        if (atpos >= list->Count - 1)
        {
            Node* ndptr = list->Last;
            list->Last->Previous->Next = NULL;
            list->Last = list->Last->Previous;
            ret = ndptr->Data;
            free(ndptr);

        }
        else
        {
            ctr = 0;
            Node* target = NULL;
            Node* targetsnext = NULL;
            Node* targetsprev = NULL;

            for (target = list->First, ctr = 0; ctr < atpos; target = target->Next, ctr++) {}

            targetsnext = target->Next;
            targetsprev = target->Previous;

            targetsprev->Next = targetsnext;
            targetsnext->Previous = targetsprev;

            ret = target->Data;
            free(target);
        }
    }

    list->Count--;

    list->IsSorted = 0;

    return ret;
}

void* remove(List *list, const  void* data)
{
    Node *ptr = NULL;
    int idx = 0;
    void* ret = NULL;

    idx = search(list, data);

    if (idx < 0)
    {
        return ret;
    }

    Node* target = ptr;
    Node* targetsnext = NULL;
    Node* targetsprev = NULL;

    int ctr = 0;
    for (target = list->First, ctr = 0; ctr < idx; target = target->Next, ctr++) {}

    targetsnext = target->Next;
    targetsprev = target->Previous;

    targetsprev->Next = targetsnext;
    targetsnext->Previous = targetsprev;

    ret = target->Data;
    free(target);

    return ret;
}


void* truncate(List *list)
{
    return removeat(list, list->Count - 1);
}

void sort(List *list)
{
    Node *currentnode;

    int i, j;

    for (i = 0; i < list->Count; i++)
    {
        currentnode = list->First;

        for (j = 0; j < list->Count - 1 - i; j++)
        {
            if ( memcmp(currentnode->Data, currentnode->Next->Data, currentnode->Size) > 0 )
            {
                void* temp = currentnode->Data;
                currentnode->Data = currentnode->Next->Data;
                currentnode->Next->Data = temp;
            }
            currentnode = currentnode->Next;
        }
    }

    list->IsSorted = 1;
}


void clear(List *list)
{
}

int indexOf(List *list, const  void* data)
{
    return search(list, data);
}

int search(List *list, const void* data)
{
    if (list->Count < 1)
    {
        return -1;
    }

    int ctr = 0;
    Node* ptr = list->First;

    while (1)
    {
        if (ptr == NULL)
        {
            break;
        }

        if (memcmp(data, ptr->Data, ptr->Size) == 0)
        {
            return ctr;
        }

        ptr = ptr->Next;

        ctr++;
    }

    return -1;
}

void setAt(List *list, int atpos, void* data)
{
    void* d = getAt(list, atpos);

    if (d)
    {
        memcpy(d, data, list->First->Size);
    }
}


void* getAt(List *list, int atpos)
{
    if (atpos > list->Count - 1 || atpos < 0)
    {
        return NULL;
    }

    int ctr = 0;

    Node* ptr = list->First;

    while (1)
    {
        if (ptr == NULL)
        {
            return NULL;
        }

        if (ctr >= atpos)
        {
            break;
        }

        ptr = ptr->Next;

        ctr++;
    }

    if (ptr == NULL)
    {
        return NULL;
    }

    return ptr->Data;
}

void* getFirst(List *list)
{
    list->IteratorPosition = list->First;

    if (list->IteratorPosition == NULL)
    {
        return NULL;
    }

    return list->IteratorPosition->Data;
}


void* getLast(List *list)
{
    list->IteratorPosition = list->Last;

    if (list->IteratorPosition == NULL)
    {
        return NULL;
    }

    return list->IteratorPosition->Data;
}


void* getNext(List *list)
{
    list->IteratorPosition = list->IteratorPosition->Next;

    if (list->IteratorPosition == NULL)
    {
        return NULL;
    }

    return list->IteratorPosition->Data;
}


void* getPrevious(List *list)
{
    list->IteratorPosition = list->IteratorPosition->Previous;

    if (list->IteratorPosition == NULL)
    {
        return NULL;
    }

    return list->IteratorPosition->Data;
}


void copyNode(List *list, void* dest, const void* orig, int sz)
{
    char* destptr = (char*)dest;
    char* origptr = (char*)orig;

    for (int ctr = 0; ctr < sz; ctr++)
    {
        destptr[ctr] = origptr[ctr];
    }
}

int count(List *list)
{
    return list->Count;
}

