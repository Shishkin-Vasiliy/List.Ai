#include <stdio.h>
#include "List.h"

int main(void)
{
    List_t List = {};
    ListErr_t err = ListCtor(&List, 16);
    printf("List successfully created\n");

    err = ListInsertNode(&List, 50, 1);
    if (err)
        printf("err = %d\n", err);

    err = ListInsertNode(&List, 55, HEAD);
    if (err)
        printf("err = %d\n", err);

    err = ListInsertNode(&List, 60, HEAD);
    if (err)
        printf("err = %d\n", err);

    err = ListInsertNode(&List, 70, HEAD);
    if (err)
        printf("err = %d\n", err);
    
    err = ListInsertNode(&List, 80, HEAD);
    if (err)
        printf("err = %d\n", err);

    err = ListDeleteNode(&List, 3);
    if (err)
        printf("err = %d\n", err);
    
    err = ListDeleteNode(&List, HEAD);
    if (err)
        printf("err = %d\n", err);

    err = ListDeleteNode(&List, TAIL);
    if (err)
        printf("err = %d\n", err);

    err = ListDeleteNode(&List, TAIL);
    if (err)
        printf("err = %d\n", err);

    err = ListDeleteNode(&List, HEAD);
    if (err)
        printf("err = %d\n", err);

    err = ListInsertNode(&List, 4, HEAD);
    if (err)
        printf("err = %d\n", err);


    PrintList(&List);    
    ListDtor(&List);

    return 0;
}