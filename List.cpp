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
    List->size = 0;
    List->dummy = List->data;
    List->dummy->val = POIZON_VAL;
    List->dummy->next = 0;
    List->dummy->prev = 0;

    for (size_t i = 1; i < List->capacity; i++)
    {
        List->data[i].val = POIZON_VAL;
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

ListErr_t ListInsertNode(List_t *List, ListElem_t val, int node)
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

    List->size++;
    
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
    List->data[Curr].val = DELETED_VAL;

    List->size--;

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

int ListLogInit(List_t *List, LogCtx_t *LogCtx)
{
    const int MAX_PICTS = 16;
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

    LogCtx->pict_index = (char **)calloc(MAX_PICTS, sizeof(char *));
    if (!LogCtx->pict_index)
        return EOF;

    return 0;
}

void ListDump(List_t *List, LogCtx_t *LogCtx)
{
    const int DATE_SIZE = 30;
    const int TIME_SIZE = 30;
    const int PATH_SIZE = 100;
    const int COMMAND_SIZE = 200;

    char dot_path[PATH_SIZE] = {};
    char png_path[PATH_SIZE] = {};
    char date[DATE_SIZE] = {};
    char time[TIME_SIZE] = {};
    char command[COMMAND_SIZE] = {};
    
    int num_picts = LogCtx->num_picts;

    snprintf(date, DATE_SIZE, __DATE__);
    for (char *p = date; *p != '\0' && (p = strchr(p, ' ')) != NULL; p++)
        *p = '_';

    snprintf(time, TIME_SIZE, __TIME__);
    for (char *p = time; *p != '\0' && (p = strchr(p, ':')) != NULL; p++)
        *p = '_';

    snprintf(dot_path, PATH_SIZE, "ListLog/%s_%s_%d.dot", date, time, num_picts);
    FILE *dot_fp = fopen(dot_path, "w");
    if (!dot_fp)
        return;
    DescribeGraph(List, dot_fp);
    fclose(dot_fp);
    snprintf(command, COMMAND_SIZE, "dot %s -Tpng -o ListLog/%s_%s_%d.png", dot_path, date, time, num_picts);
    system(command);

    snprintf(png_path, PATH_SIZE, "%s_%s_%d.png", date, time, num_picts);

    char *temp = strdup(png_path);
    if (!temp) 
        return;         // TODO not void return
    LogCtx->pict_index[LogCtx->num_picts] = temp;
    LogCtx->num_picts++;
} 

void DescribeGraph(List_t *List, FILE *fp) 
{
    fprintf(fp, "digraph {\n");
    fprintf(fp, "bgcolor=\"#283f3f\";\n");
    fprintf(fp, "rankdir=TB;\n");      
    fprintf(fp, "splines=ortho;\n");
    fprintf(fp, "nodesep=0.8;\n");
    fprintf(fp, "ranksep=0.9;\n");

    fprintf(fp, "{ rank=same;\n"); 
    
    fprintf(fp, "node0 [shape=Mrecord; style=filled; fillcolor=\"#6A184DA0\"; color=\"#A5497EA0\"; fontcolor=\"#FFFFFF\";\n");
    fprintf(fp, "       label=\"  { index = 0    | value = %X | {prev = %d | next = %d} }\"];\n", List->dummy->val, List->dummy->prev, List->dummy->next);

    for (int i = 1; i < List->capacity; i++)
    {
        if ((List->data[i].val == DELETED_VAL) || (List->data[i].val == POIZON_VAL))
        {
            fprintf(fp, "node%d [shape=Mrecord; style=filled; fillcolor=\"#000040A0\"; color=\"#FFFFFF\"; fontcolor=\"#FFFFFF\";\n", i);
            fprintf(fp, "       label=\"  { index = %d   | value = %X | {prev = %d | next = %d} }\"];\n", i, List->data[i].val, List->data[i].prev, List->data[i].next);
        }
        else 
        {
            fprintf(fp, "node%d [shape=Mrecord; style=filled; fillcolor=\"#000090A0\"; color=\"#FFFFFF\"; fontcolor=\"#FFFFFF\";\n", i);
            fprintf(fp, "       label=\"  { index = %d   | value = %d | {prev = %d | next = %d} }\"];\n", i, List->data[i].val, List->data[i].prev, List->data[i].next);
        }
    }
    
    fprintf(fp, "}\n");

    fprintf(fp, "edge [color=\"#FFFFFF\"];\n");

    for (int count = 0, curr = 0; count < List->size; count++, curr = List->data[curr].next) 
    {
        int next = List->data[curr].next;
        if (List->data[curr].val != DELETED_VAL && List->data[next].val != DELETED_VAL
        &&  List->data[curr].val != POIZON_VAL && List->data[next].val != POIZON_VAL)
        {
            fprintf(fp, "node%d -> node%d [weight=100];\n", curr, next); 
            fprintf(fp, "node%d -> node%d [weight=1];\n", next, curr);
        }
    }

    fprintf(fp, "node [shape=circle; style=filled; fontcolor=\"#FFFFFF\"; width=0.6; fixedsize=true];\n");
    fprintf(fp, "tail [label=\"tail\", fillcolor=\"#FF6347A0\", color=\"#FFFFFF\"];\n");
    fprintf(fp, "head [label=\"head\", fillcolor=\"#A52A2AA0\", color=\"#FFFFFF\"];\n");

    if (List->size)
    {
        fprintf(fp, "tail:s -> node%d:n;\n", List->dummy->next);
        fprintf(fp, "head:s -> node%d:n;\n", List->dummy->prev);
    }
    else 
    {
        fprintf(fp, "tail:s -> node0:n;\n");
        fprintf(fp, "head:s -> node0:n;\n");
    }

    fprintf(fp, "node [shape=invtrapezium; style=filled; fontcolor=\"#FFFFFF\"; width=1.0; fixedsize=true];\n");
    for (int i = 0; i < List->capacity; i++)
    {
        if (List->data[i].val == DELETED_VAL)
        {
            fprintf(fp, "deleted%d [label=\"deleted\", fillcolor=\"#000000\", color=\"#FFFFFF\"];\n", i);
            fprintf(fp, "deleted%d:s -> node%d:n\n", i, i);
        }
    }

    fprintf(fp, "}\n");
}

void ConstructHtm(List_t *List, LogCtx_t *LogCtx)
{
    FILE *fp = fopen("ListLog/Dump.htm", "w");
    if (!fp)
        return;

    fprintf(fp, "<body style=\"background-color: #283f3f;\">\n");
    fprintf(fp, "<pre style=\"font-family: Consolas, 'Courier New', monospace; color: #ffffff; font-size: 14px;\">\n");

    fprintf(fp, "<h3> <font color=white >LIST DUMP</font> </h3>\n");
    fprintf(fp, "<font color=white>List Visualization:</font>\n");

    for (int i = 0; i < LogCtx->num_picts; i++)
    {
        fprintf(fp, "<div style=\"margin-top: 100px;\"></div>\n");
        fprintf(fp, "<img src=%s width=1400px>", LogCtx->pict_index[i]);
    }

    fprintf(fp, "</pre>\n");
    fprintf(fp, "</body>\n");

    fclose(fp);
}
