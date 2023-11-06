#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "classic_list.h"

#define LIST_VERIFY              \
    if (listError(list))         \
    {                            \
        LIST_DUMP(list);         \
        return NULL;             \
    }

#define NODE_IN_LIST(node)       \
    if (!nodeIsInList(list, node)) return NULL;
    

struct Node
{
    elem_t data;

    Node *next;
    Node *prev;
};

struct List
{
    int size;

    Node *header;
};

List *listCtor()
{
    List *list = (List *)calloc(1, sizeof(List));
    if (!list) return NULL;

    list->header = (Node *)calloc(1, sizeof(Node));
    if (!list->header) { free(list); return NULL; }

    list->header->data = Poison;
    list->header->next = list->header;
    list->header->prev = list->header;

    list->size = 0;

    return list;
}

int listError(List *list)
{
    assert(list);
    
    if (!list->header) return LIST_HEADER_NULL;

    int count = 0;
    Node *current = list->header->next;

    while (current != list->header)
    {
        if (current != current->next->prev) return LIST_BAD_POINTERS;

        current = current->next;
        count++;
    }

    if (count != list->size) return LIST_INCORRECT_SIZE;

    return LIST_OK;
}

int listDump(List *list, const char *file, int line, const char *function)
{
    assert(list);
    assert(file);
    assert(function);

    printf("I'm listDump() called from %s(%d) %s:\n", file, line, function);
    printf("list ptr = <%p>\n", list);
    listPrintErrors(list);
    printf("size = %d\n", list->size);

    if (!list->header) return LIST_HEADER_NULL;

    printf("list->header: <%p>\n", list->header);
    
    Node *current = list->header->next;
    int count = 0;

    while (current != list->header)
    {
        printf("Node%3d <%p>:\n data = " elemFormat "\n", count, current, current->data);
        printf(" next = <%p>\n prev = <%p>\n", current->next, current->prev);

        count++;
        current = current->next;
    }

    return EXIT_SUCCESS;
}

int listPrintErrors(List *list)
{
    assert(list);

    int error = listError(list);

    switch(error)
    {
        case LIST_OK:
            printf("no errors determined\n");
            break;
        case LIST_HEADER_NULL:
            printf("ERROR: the header element ptr is NULL\n");
            break;
        case LIST_BAD_POINTERS:
            printf("ERROR: next and prev ptrs don't match\n");
            break;
        case LIST_INCORRECT_SIZE:
            printf("ERROR: list->size doesn't match its contents\n");
            break;
        case  LIST_INCORRECT_ELEMENT:
            printf("ERROR: given element is not in list\n");
            break;
        default:
            printf("UNKNOWN ERROR: %d\n", error);
    }

    return EXIT_SUCCESS;
}

int listDtor(List **listPtr)
{
    assert(listPtr);
    assert(*listPtr);

    List *list = *listPtr;
    list->size = -1;

    if (!list->header)
    {
        free(list);
        listPtr = NULL;

        return EXIT_SUCCESS;
    }

    Node *current = list->header->next;
    while (current != list->header)
    {
        Node *next = current->next;
        free(current);

        current = next;
    }

    free(list->header);
    free(list);
    (*listPtr) = NULL;

    return EXIT_SUCCESS;
}

bool nodeIsInList(List *list, Node *node)
{
    assert(list);
    assert(node);
    assert(list->header);

    Node *current = list->header->next;
    while (current != list->header)
    {
        if (current == node) return true;

        current = current->next;
    }

    if (current == list->header) return true;

    return false;
}

Node *listHead(List *list)
{
    LIST_VERIFY;

    return list->header->next;
}

Node *listTail(List *list)
{
    LIST_VERIFY;

    return list->header->prev;
}

Node *listNext(List *list, Node *node)
{
    LIST_VERIFY;
    NODE_IN_LIST(node);
    
    if (node->next == list->header) return NULL;

    return node->next;
}

Node *listPrev(List *list, Node *node)
{
    LIST_VERIFY;
    NODE_IN_LIST(node);
    
    if (node->prev == list->header) return NULL;

    return node->prev;
}

elem_t listData(List *list, Node *node)
{
    LIST_VERIFY;
    NODE_IN_LIST(node);
    
    return node->data;
}

Node *listInsertAfter(List *list, Node *anchor, elem_t value)
{
    LIST_VERIFY;
    NODE_IN_LIST(anchor);

    Node *node = (Node *)calloc(1, sizeof(Node));
    if (!node) return NULL;

    node->next       = anchor->next;
    node->next->prev = node;

    anchor->next = node;
    node->prev   = anchor;

    node->data = value;

    list->size++;

    return node;
}

Node *listInsertBefore(List *list, Node *anchor, elem_t value)
{
    LIST_VERIFY;
    NODE_IN_LIST(anchor);

    return listInsertAfter(list, anchor->prev, value);
}

int listDel(List *list, Node *node)
{
    LIST_VERIFY;
    if (!nodeIsInList(list, node)) return LIST_INCORRECT_ELEMENT;
    if (node == list->header)      return LIST_INCORRECT_ELEMENT;

    node->prev->next = node->next;
    node->next->prev = node->prev;

    free(node);

    list->size--;

    return EXIT_SUCCESS;
}

Node *listPushBack(List *list, elem_t value)
{
    LIST_VERIFY;

    return listInsertAfter(list, listTail(list), value);
}

Node *listPushFront(List *list, elem_t value)
{
    LIST_VERIFY;

    return listInsertBefore(list, listHead(list), value);
}

bool listIsEmpty(List *list)
{
    LIST_VERIFY;

    return list->size == 0;
}
