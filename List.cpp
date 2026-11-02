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
    List->dummy = List->data;
    List->dummy->val = POIZON_VAL;
    List->dummy->next = 0;
    List->dummy->prev = 0;

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

    free(List->data);
    List->data = POIZON_DATA_PTR;
}

int FindFree(List_t *List)
{
    for (int i = 1; i < List->size; i++)
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

    LIST_VERIFY(err, List);
    int Curr = List->dummy->next;

    if (node == (int)HEAD)
    {
        Curr = 0;
    }
    else if (node == (int)TAIL)
    {
        Curr = List->dummy->next;
    }
    else 
    {
        Curr = FindNode(List, node);
    }

    int CurrPrev = List->data[Curr].prev;
    List->data[CurrPrev].next = InsertPos;
    List->data[InsertPos].prev = CurrPrev;
    List->data[InsertPos].next = Curr;
    List->data[Curr].prev = InsertPos;
    List->data[InsertPos].val = val; 

    if (node == HEAD)
        List -> dummy -> prev = InsertPos;
    else if (node == TAIL)
        List -> dummy -> next = InsertPos;
    
    LIST_VERIFY(err, List);
    return LIST_OK; 
}

ListErr_t ListDeleteNode(List_t *List, int node)
{
    ListErr_t err = LIST_OK;

    LIST_VERIFY(err, List);
    if (List->dummy->next == List->dummy->prev && List -> dummy -> next == 0)
    {
        printf("List is empty, cannot delete\n");
        return LIST_EMPTY;
    }
    int Curr = List->dummy->next;

    if (node == (int)HEAD)
    {
        Curr = List->dummy->prev;
        List -> dummy -> prev = List -> data[Curr].prev;
    }
    else if (node == (int)TAIL)
    {
        Curr = List->dummy->next;
        List -> dummy -> next = List -> data[Curr].next;
    }
    else
    {
        Curr = FindNode(List, node);
    }

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

void PrintList(List_t *List)
{
    int Curr = List->dummy->next;
    int Count = 1;

    while (Curr != 0)
    {
        printf("[%d].val = %d, .next = %d .prev = %d\n", Count, List->data[Curr].val, List->data[Curr].next, List->data[Curr].prev);
        Curr = List->data[Curr].next;
        Count++;
    }
}

ListErr_t ListVerify(List_t *List)
{
    int count = 0;
    
    if (!(List->data))
        return LIST_CALLOC_FAIL;

    int i = List->dummy->next;
    for (; count < List->size && i != List -> dummy -> prev; count++)
        i = List->data[i].next;
    if (i != List -> dummy -> prev)
        return LIST_CORRUPTED;

    i = List->dummy->prev;
    for (count = 0; count < List -> size && i != List -> dummy -> next; count++)
        i = List->data[i].prev;
    if (i != List -> dummy -> next)
        return LIST_CORRUPTED;

    if (List->dummy->val != POIZON_VAL)
        return LIST_CORRUPTED;

    return LIST_OK;
}

int FindNode(List_t *List, int node)
{
    int Curr = List->dummy->next;
    for (int count = 1; count < node; count++)
        Curr = List->data[Curr].next;

    return Curr;
}