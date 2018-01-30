#include "KeyValueC.h"

#include <string.h>
#include <memory.h>
#include <stdlib.h>

void keyValueCopy(KeyValueC* dest, KeyValueC* orig)
{
    nodeCopy(dest->Key, orig->Key);
    nodeCopy(dest->Value, orig->Value);
}

int keyValueAreEqual(KeyValueC* first, KeyValueC* second)
{
    return nodeAreEqual(first->Key, second->Key);
}

int keyValueIsGreater(KeyValueC* first, KeyValueC* second)
{
    return nodeIsGreater(first->Key, second->Key);
}

int keyValueIsLess(KeyValueC* first, KeyValueC* second)
{
    return nodeIsLess(first->Key, second->Key);
}
