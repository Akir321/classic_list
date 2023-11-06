#ifndef  __CLASSIC_LIST_H__
#define  __CLASSIC_LIST_H__

typedef int elem_t;
const int Poison = 0;
#define elemFormat "%d"

struct Node;
struct List;

enum ListErrors
{
    LIST_OK                = 0,
    LIST_MEMORY_ERROR      = -1,
    LIST_HEADER_NULL       = -2,
    LIST_BAD_POINTERS      = -3,
    LIST_INCORRECT_SIZE    = -4,
    LIST_INCORRECT_ELEMENT = -5
};

List *listCtor(void);

int listDtor       (List **list);

int listError      (List *list);
int listPrintErrors(List *list);
int listDump       (List *list, const char *file, int line, const char *function);

bool nodeIsInList(List *list, Node *node);

Node  *listHead(List *list);
Node  *listTail(List *list);
Node  *listNext(List *list, Node *node);
Node  *listPrev(List *list, Node *node);
elem_t listData(List *list, Node *node);

Node *listInsertAfter (List *list, Node *anchor, elem_t value);
Node *listInsertBefore(List *list, Node *anchor, elem_t value);

Node *listPushBack (List *list, elem_t value);
Node *listPushFront(List *list, elem_t value);

int listDel(List *list, Node *node);

bool listIsEmpty(List *list);

#define LIST_DUMP(list) listDump(list, __FILE__, __LINE__, __func__)

#endif //__CLASSIC_LIST_H__
