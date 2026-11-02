#include <stdio.h>
#include "List.h"
#include <cstdlib>
#include <stdlib.h>

int main(void)
{
    List_t List = {};
    ListErr_t err = ListCtor(&List, 16);
    printf("List successfully created\n");

    int LogErr = ListLogInit(&List);
    if (!LogErr)
        printf("Logging started\n");

    getchar();
    err = ListInsertNode(&List, 50, 1);
    if (err)
        printf("err = %d\n", err);
    printf("Insert 50 at 1\n");
    PrintList(&List);

    getchar();
    err = ListInsertNode(&List, 55, HEAD);
    if (err)
        printf("err = %d\n", err);
    printf("Insert 55 at HEAD\n");
    PrintList(&List);

    getchar();
    err = ListInsertNode(&List, 60, HEAD);
    if (err)
        printf("err = %d\n", err);
    printf("Insert 60 at HEAD\n");
    PrintList(&List);

    getchar();
    err = ListInsertNode(&List, 70, HEAD);
    if (err)
        printf("err = %d\n", err);
    printf("Insert 70 at HEAD\n");
    PrintList(&List);
    
    getchar();
    err = ListInsertNode(&List, 80, HEAD);
    if (err)
        printf("err = %d\n", err);
    printf("Insert 80 at HEAD\n");
    PrintList(&List);

    getchar();
    err = ListDeleteNode(&List, 3);
    if (err)
        printf("err = %d\n", err);
    printf("Delete 3\n");
    
    printf("HEAD = %d TAIL = %d\n", List.dummy->prev, List.dummy->next);
    ListDump(&List);

    //getchar();
    //err = ListDeleteNode(&List, HEAD);
    //if (err)
    //    printf("err = %d\n", err);
    //printf("Delete HEAD\n");
    //PrintList(&List);
//
    //getchar();
    //err = ListDeleteNode(&List, TAIL);
    //if (err)
    //    printf("err = %d\n", err);
    //printf("Delete TAIL\n");
    //PrintList(&List);
//
    //getchar();
    //err = ListDeleteNode(&List, TAIL);
    //if (err)
    //    printf("err = %d\n", err);
    //printf("Delete TAIL\n");
    //PrintList(&List);
//
    //getchar();
    //err = ListDeleteNode(&List, HEAD);
    //if (err)
    //    printf("err = %d\n", err);
    //printf("Delete HEAD\n");
    //PrintList(&List);

    //getchar();
    //err = ListInsertNode(&List, 4, HEAD);
    //if (err)
    //    printf("err = %d\n", err);
    //printf("Insert 4 at HEAD\n");
    //PrintList(&List);

    ListDtor(&List);

    return 0;
}