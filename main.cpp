#include <stdio.h>
#include "List.h"
#include <cstdlib>
#include <stdlib.h>

int main(void)
{
    List_t List = {};
    ListErr_t err = ListCtor(&List, 8);

    LogCtx_t LogCtx = {};
    int LogErr = ListLogInit(&List, &LogCtx);
    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 50, 1);
    printf("Insert 50 at 1\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 55, HEAD);
    printf("Insert 55 at HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 60, HEAD);
    printf("Insert 60 at HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListDeleteNode(&List, 2);
    printf("Delete 2\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 70, HEAD);
    printf("Insert 70 at HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);
    
    getchar();
    ListInsertNode(&List, 80, HEAD);
    printf("Insert 80 at HEAD\n");
    PrintList(&List);

    ListDump(&List, &LogCtx);

    getchar();
    ListDeleteNode(&List, 3);
    printf("Delete 3\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);
    
    getchar();
    ListDeleteNode(&List, HEAD);
    printf("Delete HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 100, HEAD);
    printf("Insert 100 at HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListDeleteNode(&List, TAIL);
    printf("Delete TAIL\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListDeleteNode(&List, TAIL);
    printf("Delete TAIL\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    getchar();
    ListDeleteNode(&List, HEAD);
    printf("Delete HEAD\n");
    PrintList(&List);

    ListDump(&List, &LogCtx);

    getchar();
    ListInsertNode(&List, 4, HEAD);
    printf("Insert 4 at HEAD\n");
    PrintList(&List);
    ListDump(&List, &LogCtx);

    ConstructHtm(&List, &LogCtx);

    ListLogDeinit(&LogCtx);
    ListDtor(&List);

    return 0;
}