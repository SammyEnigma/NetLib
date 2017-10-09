#ifndef _LIST
#define _LIST

typedef struct Node
{
	void* Data;
	int Size;
	struct Node* Previous;
	struct Node* Next;
}Node;

typedef struct List
{
    struct Node* First;
    struct Node* Last;
    struct Node* IteratorPosition;
    int Count;
    int IsSorted;
}List;

extern void insertat(List *list, const void* data, int sz, int atpos);
extern void insert(List *list, const  void* data, int sz);
extern void append(List *list, const  void* data, int sz);

extern void appendlist(List *list, const List *newlist);
extern List* mergelists(const List *list1, const List *list2);

extern void* removeat(List *list, int atpos);
extern void* remove(List *list, const  void* data);
extern void* truncate(List *list);

extern void sort(List *list);
extern int indexOf(List *list, const  void* data);
extern void clear(List *list);

extern void* getAt(List *list, int atpos);
extern void setAt(List *list, int atpos, void* data);
extern void* getFirst(List *list);
extern void* getLast(List *list);
extern void* getNext(List *list);
extern void* getPrevious(List *list);

extern void copyNode(List *list, void* dest, const void* orig, int sz);
extern int search(List *list, const void* data);
extern int count(List *list);

#endif
