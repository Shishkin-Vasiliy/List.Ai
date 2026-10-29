#include <stdio.h>
#include "List.h"
#include <cstdlib>

ListErr_t ListCtor(List_t *List, size_t size)
{
    ListErr_t err = LIST_OK;

    List->data = (Node_t *)calloc(size, sizeof(Node_t));
    if (!(List->data))
        return LIST_CALLOC_FAIL;

    List->size = size;
    List->head = 1;
    List->tail = 1;
    List->data[0].val = POIZON_VAL;
    List->data[0].next = 0;
    List->data[0].prev = 0;

    for (size_t i = 1; i < List->size; i++)
    {
        List->data[i].next = -1;
        List->data[i].prev = -1;
    }
 
    return LIST_OK;
}

void ListDtor(List_t *List)
{
    for (size_t i = 0; i < List->size; i++)
    {
        List->data[i].val = POIZON_VAL;
        List->data[i].next = -1;
        List->data[i].prev = -1;
    }

    List->head = 0;
    List->tail = 0;

    free(List->data);
    List->data = POIZON_DATA_PTR;
}

int FindFree(List_t *List)
{
    for (int i = 0; i < List->size; i++)
    {
        if (List->data[i].next == -1)
            return i;
    }
    return -1;
}

ListErr_t ListInsertNode(List_t *List, int val, int node)
{ 
    int InsertPos = FindFree(List);
    ListErr_t err = LIST_OK;

    if (InsertPos == -1)
        return LIST_OVERFLOW;

    if (List->tail == List->head && List->data[List->tail].val == POIZON_VAL)
    {
        LIST_VERIFY(err, List);

        List->data[List->tail].val = val;
        
        LIST_VERIFY(err, List);

        return LIST_OK;
    }

    else if (node == HEAD ||
            (List->tail == List->head && List->data[List->tail].val != POIZON_VAL))
    {
        LIST_VERIFY(err, List);

        int OldHead = List->head;
        List->data[List->head].next = InsertPos;
        List->data[InsertPos].next = 0;
        List->head = InsertPos;
        List->data[List->head].prev = OldHead;
        List->data[List->head].val = val;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }

else if (node == TAIL && List->head != List->tail)
    {
        LIST_VERIFY(err, List);

        List->data[InsertPos].next = List->tail;
        List->data[InsertPos].prev = 0;
        List->data[List->tail].prev = InsertPos;
        List->tail = InsertPos;
        List->data[List->tail].val = val;
        List->data[List->tail].prev = 0;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }

    else
    {
        int Curr = List->tail;
        LIST_VERIFY(err, List);

        for (int count = 1; count < node; count++)
            Curr = List->data[Curr].next;

        int CurrPrev = List->data[Curr].prev;
        List->data[CurrPrev].next = InsertPos;
        List->data[InsertPos].prev = CurrPrev;
        List->data[InsertPos].next = Curr;
        List->data[Curr].prev = InsertPos;
        List->data[InsertPos].val = val; 
        
        LIST_VERIFY(err, List);

        return LIST_OK;
    }
}

ListErr_t ListDeleteNode(List_t *List, int node)
{
    ListErr_t err = LIST_OK;

    if (List->tail == List->head && List->data[List->tail].val == POIZON_VAL)
    {
        return LIST_EMPTY;
    }

    else if ((node == HEAD || node == TAIL) &&
            (List->tail == List->head && List->data[List->tail].val != POIZON_VAL))
    {
        LIST_VERIFY(err, List);

        List->data[List->head].val = POIZON_VAL;
        List->data[List->head].next = -1;
        List->data[List->head].prev = -1;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }

    else if (node == HEAD && List->tail != List->head)
    {
        LIST_VERIFY(err, List);

        int OldHead = List->head;
        int OldHeadPrev = List->data[OldHead].prev;
        List->data[OldHead].val = POIZON_VAL;
        List->data[OldHead].next = -1;
        List->data[OldHead].prev = -1;
        List->head = OldHeadPrev;
        List->data[List->head].next = 0;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }

    else if (node == TAIL && List->tail != List->head)
    {
        int OldTail = List->tail;
        int OldTailNext = List->data[OldTail].next;

        LIST_VERIFY(err, List);
        
        List->data[OldTail].val = POIZON_VAL;
        List->data[OldTail].next = -1;
        List->data[OldTail].prev = -1;
        List->tail = OldTailNext;
        List->data[List->tail].prev = 0;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }

    else 
    {
        LIST_VERIFY(err, List);

        int Curr = List->tail;
        for (int count = 1; count < node; count++)
            Curr = List->data[Curr].next;

        int Prev = List->data[Curr].prev;
        int Next = List->data[Curr].next;

        List->data[Prev].next = Next;
        List->data[Next].prev = Prev;
        List->data[Curr].val = POIZON_VAL;
        List->data[Curr].next = -1;
        List->data[Curr].prev = -1;

        LIST_VERIFY(err, List);

        return LIST_OK;
    }
}

void PrintList(List_t *List)
{
    int Curr = List->tail;
    int Count = 1;

    //while (Curr != 0)
    //{
    //    printf("[%d].val = %d, .next = %d .prev = %d\n", Count, List->data[Curr].val, List->data[Curr].next, List->data[Curr].prev);
    //    Curr = List->data[Curr].next;
    //    Count++;
    //}

    for (int i = 0; i < List->size; i++)
        printf("[%d] = %d\n", i, List->data[i].val);
}

ListErr_t ListVerify(List_t *List)
{
    int count = 0;
    int i = List->tail;

    if (!(List->data))
        return LIST_CALLOC_FAIL;

    for (; count < List->size; count++)
        i = List->data[i].next;

    if (i != 0 && i != List->head)
        return LIST_CORRUPTED;

    if (List->data[0].val != 0x0D01B0EB)
        return LIST_CORRUPTED;

    return LIST_OK;
}