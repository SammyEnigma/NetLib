#ifndef _KEY_VALUE_C
#define _KEY_VALUE_C

#include "Node.h"

typedef struct KeyValue
{
    Node*	Key;
    Node*	Value;
}KeyValue;

void keyValueCopy(KeyValue* dest, KeyValue* orig);

int keyValueAreEqual(KeyValue* first, KeyValue* second);

int keyValueIsGreater(KeyValue* first, KeyValue* second);

int keyValueIsLess(KeyValue* first, KeyValue* second);

#endif
