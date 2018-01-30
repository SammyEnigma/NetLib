#ifndef _KEY_VALUE_C
#define _KEY_VALUE_C

#include "NodeC.h"

typedef struct KeyValueC
{
    NodeC*	Key;
    NodeC*	Value;
}KeyValueC;


void keyValueCopy(KeyValueC* dest, KeyValueC* orig);

int keyValueAreEqual(KeyValueC* first, KeyValueC* second);

int keyValueIsGreater(KeyValueC* first, KeyValueC* second);

int keyValueIsLess(KeyValueC* first, KeyValueC* second);

#endif
