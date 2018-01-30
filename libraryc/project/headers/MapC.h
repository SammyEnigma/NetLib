#ifndef _MAP_C
#define _MAP_C

#include "KeyValueC.h"
#include "NodeC.h"

typedef struct MapC
{
	unsigned long Count;
	unsigned long MapMemorySize;
	KeyValueC* MapMemory;
	long IteratorPosition;
}MapC;

void mapClear(MapC* mptr);
void mapAllocate(MapC* mptr);

void mapInsert(MapC* mptr, NodeC key, const NodeC data);
void mapRemove(MapC* mptr, const NodeC key);

unsigned long mapItemCount();
unsigned long mapIndexOf(const NodeC key);

NodeC* mapGetkey(MapC* mptr, const NodeC data);
NodeC* mapGetvalue(MapC* mptr, const NodeC key);
KeyValueC* mapGetAt(MapC* mptr, unsigned int atpos);

KeyValueC* mapGetFirst(MapC* mptr);
KeyValueC* mapGetLast(MapC* mptr);
KeyValueC* mapGetNext(MapC* mptr);
KeyValueC* mapGetPrevious(MapC* mptr);


#endif
