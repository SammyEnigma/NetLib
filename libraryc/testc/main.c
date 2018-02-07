#include "KeyValueC.h"
#include "ListC.h"
#include "MapC.h"
#include "NodeC.h"

#include <stdio.h>
#include <string.h>

#define printLine printf("\n")
void listPrintForward(ListC* lptr);
void listPrintReverse(ListC* lptr);
void listRandomAccess(ListC* lptr, size_t index);

int main(int argc, char *argv[])
{
    int x = atoi("-123");

    ListC* list = NULL;

    list = listAllocate(list);

    listAddToTail(list, "One", strlen("One")+1);
    listAddToTail(list, "Two", strlen("Two")+1);
    listAddToTail(list, "Three", strlen("Three")+1);
    listAddToTail(list, "Four", strlen("Four")+1);
    listAddToTail(list, "Five", strlen("Five")+1);

    listPrintForward(list);

    printLine;

    listPrintReverse(list);

    printLine;

    listRandomAccess(list, 3);

    printLine;

    printf("Remove first\n");
    listRemoveFromHead(list);

    printLine;

    listPrintForward(list);

    printLine;

    listPrintReverse(list);

    printLine;

    printf("Remove last\n");
    listRemoveFromTail(list);

    printLine;

    listPrintForward(list);

    printLine;

    listPrintReverse(list);


    return 0;
}

void listPrintForward(ListC* lptr)
{
    printf("Forward traversal\n");
    NodeC* ptr = NULL;

    ptr = listGetFirst(lptr);

    while(1)
    {
        if(ptr == NULL)
        {
            break;
        }

        printf("%s\n", (char*)ptr->Data);
        ptr = listGetNext(lptr);
    }
}

void listPrintReverse(ListC* lptr)
{
    printf("Reverse traversal\n");
    NodeC* ptr = NULL;

    ptr = listGetLast(lptr);

    while(1)
    {
        if(ptr == NULL)
        {
            break;
        }

        printf("%s\n", (char*)ptr->Data);
        ptr = listGetPrevious(lptr);
    }
}

void listRandomAccess(ListC* lptr, size_t index)
{
    printf("Random access, index %zd\n", index);

    NodeC* ptr = listGetAt(lptr, 2);
    printf("%s\n", (char*)ptr->Data);
}
