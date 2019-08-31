#ifndef _MAP_C
#define _MAP_C

#include "KeyValue.h"
#include "Node.h"

typedef struct Map
{
	unsigned long Count;
	unsigned long MapMemorySize;
	KeyValue* MapMemory;
	long IteratorPosition;
}Map;

void Maplear(Map* mptr);
void mapAllocate(Map* mptr);

void mapInsert(Map* mptr, Node key, const Node data);
void mapRemove(Map* mptr, const Node key);

unsigned long mapItemCount();
unsigned long mapIndexOf(const Node key);

Node* mapGetkey(Map* mptr, const Node data);
Node* mapGetvalue(Map* mptr, const Node key);
KeyValue* mapGetAt(Map* mptr, unsigned int atpos);

KeyValue* mapGetFirst(Map* mptr);
KeyValue* mapGetLast(Map* mptr);
KeyValue* mapGetNext(Map* mptr);
KeyValue* mapGetPrevious(Map* mptr);


#endif
