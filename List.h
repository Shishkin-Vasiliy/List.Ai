#ifndef LIST_H
#define LIST_H

#define POIZON_VAL 0x0D01B0EB
#define POIZON_DATA_PTR ((Node_t *)666)

struct Node_t {
    int val;
    int next;
    int prev;
};

struct List_t {
    Node_t *data;
    Node_t *dummy;
    size_t capacity;
};

enum ListErr_t {
    LIST_OK,
    LIST_CALLOC_FAIL,
    LIST_OVERFLOW,
    LIST_CORRUPTED,
    LIST_EMPTY
};

enum KeyNodes {
    HEAD = 0,
    TAIL = 1
};

int ListLogInit(List_t *List);
void PrintList(List_t *List);
ListErr_t ListCtor(List_t *List, size_t capacity);
int FindFree(List_t *List);
int FindNode(List_t *List, int node);
ListErr_t ListInsertNode(List_t *List, int val, int node);
ListErr_t ListDeleteNode(List_t *List, int node);
ListErr_t ListVerify(List_t *List);
void ListDtor(List_t *List);
void ListDump(List_t *List);
void DescribeGraph(List_t *List, FILE *fp);
void ConstructHtm(List_t *List, FILE *fp, const char *png_path);

#define LIST_VERIFY(err, List)  \
do                              \
{                               \
    err = ListVerify(List);     \
    if (err)                    \
        return err;             \
} while (0)

#endif