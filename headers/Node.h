#ifndef _NODE
#define _NODE

typedef struct Node
{
	void* Data;
    int Size;
    struct Node* Previous;
    struct Node* Next;
}Node;

#endif
