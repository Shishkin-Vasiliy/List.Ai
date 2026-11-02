#include <stdio.h>
#include "List.h"
#include <cstdlib>
#include <stdlib.h>
#include <string.h>

ListErr_t ListCtor(List_t *List, size_t capacity)
{
    ListErr_t err = LIST_OK;

    List->data = (Node_t *)calloc(capacity, sizeof(Node_t));
    if (!(List->data))
        return LIST_CALLOC_FAIL;

    List->capacity = capacity;
    List->dummy = List->data;
    List->dummy->val = POIZON_VAL;
    List->dummy->next = 0;
    List->dummy->prev = 0;

    for (size_t i = 1; i < List->capacity; i++)
    {
        List->data[i].next = -1;
        List->data[i].prev = -1;
    }

    return LIST_OK;
}

void ListDtor(List_t *List)
{
    for (size_t i = 0; i < List->capacity; i++)
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
    for (int i = 1; i < List->capacity; i++)
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
    for (; count < List->capacity && i != List->dummy->prev; count++)
        i = List->data[i].next;
    if (i != List->dummy->prev)
        return LIST_CORRUPTED;

    i = List->dummy->prev;
    for (count = 0; count < List->capacity && i != List -> dummy->next; count++)
        i = List->data[i].prev;
    if (i != List->dummy->next)
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

int ListLogInit(List_t *List)
{
    int DirNotFound = system("[ -d ListLog ]");

    if (DirNotFound)
    {
        int err = system("mkdir ListLog");
        if (err)
        {
            return EOF;
        }
    }
    else 
    {
        system("rm -f ListLog/*");
    }
    
    return 0;
}

void ListDump(List_t *List)
{
    const int DATE_SIZE = 30;
    const int TIME_SIZE = 30;
    const int PATH_SIZE = 100;
    const int COMMAND_SIZE = 200;

    char dot_path[PATH_SIZE] = {};
    char htm_path[PATH_SIZE] = {};
    char png_path[PATH_SIZE] = {};
    char date[DATE_SIZE] = {};
    char time[TIME_SIZE] = {};
    char command[COMMAND_SIZE] = {};
    
    snprintf(date, DATE_SIZE, __DATE__);
    for (char *p = date; *p != '\0' && (p = strchr(p, ' ')) != NULL; p++)
        *p = '_';

    snprintf(time, TIME_SIZE, __TIME__);
    for (char *p = time; *p != '\0' && (p = strchr(p, ':')) != NULL; p++)
        *p = '_';

    snprintf(dot_path, PATH_SIZE, "ListLog/%s_%s.dot", date, time);
    FILE *dot_fp = fopen(dot_path, "w");
    if (!dot_fp)
        return;
    DescribeGraph(List, dot_fp);
    fclose(dot_fp);
    snprintf(command, COMMAND_SIZE, "dot %s -Tpng -o ListLog/%s_%s.png", dot_path, date, time);
    system(command);

    snprintf(htm_path, PATH_SIZE, "ListLog/%s_%s.htm", date, time);
    snprintf(png_path, PATH_SIZE, "%s_%s.png", date, time);
    FILE *htm_fp = fopen(htm_path, "w");
    if (!htm_fp)
        return;
    ConstructHtm(List, htm_fp, png_path);
    fclose(htm_fp);
} 

void DescribeGraph(List_t *List, FILE *fp) 
{
    fprintf(fp, "digraph {");

    fprintf(fp, "bgcolor=\"#283f3f\";\n");
    fprintf(fp, "rankdir=LR;\n");

    fprintf(fp, "node0 [shape=Mrecord; style = filled; fillcolor = \"#6A184D\"; color = \"#A5497E\"; fontcolor = \"#FFFFFF\";\n");
    fprintf(fp, "       label = \"   DUMMY    | value = %X | {prev = %d | next = %d} \"; ]\n", List->dummy->val, List->dummy->prev, List->dummy->next);

    int curr = List->dummy->next;
    int count = 1;
    
    for (; curr != List->dummy->prev; curr = List->data[curr].next, count++)
    {
        if (curr == List->dummy->next)
        {
            fprintf(fp, "node%d [shape=Mrecord; style = filled; fillcolor = \"#BA55D3\"; color = \"#FFFFFF\"; fontcolor = \"#FFFFFF\";\n", count);
            fprintf(fp, "       label = \"   TAIL    | value = %d | {prev = %d | next = %d} \"; ]\n", List->data[List->dummy->next].val, List->data[List->dummy->next].prev, List->data[List->dummy->next].next);
        }
        else 
        {
            fprintf(fp, "node%d [shape=Mrecord; style = filled; fillcolor = \"#006000\"; color = \"#FFFFFF\"; fontcolor = \"#FFFFFF\";\n", count);
            fprintf(fp, "       label = \"   USED    | value = %d | {prev = %d | next = %d} \"; ]\n", List->data[curr].val, List->data[curr].prev, List->data[curr].next);
        }

        fprintf(fp, "edge [color = \"#FFFFFF\"; ]\n");
        fprintf(fp, "node%d -> node%d [weight=100]\n", count - 1, count);
        fprintf(fp, "node%d -> node%d [weight=1]\n", count, count - 1);
    }
    
    fprintf(fp, "node%d [shape=Mrecord; style = filled; fillcolor = \"#000080\"; color = \"#FFFFFF\"; fontcolor = \"#FFFFFF\";\n", count);
    fprintf(fp, "       label = \"   HEAD    | value = %d | {prev = %d | next = %d} \"; ]\n", List->data[List->dummy->prev].val, List->data[List->dummy->prev].prev, List->data[List->dummy->prev].next);

    fprintf(fp, "edge [color = \"#FFFFFF\"; ]");
    fprintf(fp, "node%d -> node%d [weight=100]\n", count - 1, count);
    fprintf(fp, "node%d -> node%d [weight=1]\n", count, count - 1);
    fprintf(fp, "node%d -> node%d [weight=1]\n", count, 0);
    fprintf(fp, "node%d -> node%d [weight=1]\n", 0, count);

    fprintf(fp, "}\n");
}

void ConstructHtm(List_t *List, FILE *fp, const char *png_path)
{
    fprintf(fp, "<pre>\n");
    fprintf(fp, "<body style=\"background-color: #283f3f;\">\n");
    fprintf(fp, "<h3> <font color=white>LIST DUMP</font> </h3>\n");
    fprintf(fp, "<font color=white>List Visualization:</font>\n");
    fprintf(fp, "<div style=\"margin-top: 50px;\"></div>\n");
    fprintf(fp, "<img src=%s width=300px>", png_path);
}
