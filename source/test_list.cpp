#include <stdio.h>

#include "classic_list.h"

#define ASSISTED(funcCall)               \
    error = funcCall;                    \
    if (error) printf("%d\n", error);  

int main()
{
    List *lst = listCtor();

    LIST_DUMP(lst);

    Node *node1 = listPushBack(lst, 3);
    LIST_DUMP(lst);

    Node *node2 = listPushFront(lst, 6);
    LIST_DUMP(lst);

    Node *node3 = listInsertAfter(lst, node2, 7);
    LIST_DUMP(lst);

    listDel(lst, node1);
    LIST_DUMP(lst);

    Node *node4 = listPushFront(lst, 18);
    LIST_DUMP(lst);

    Node *node5 = listPushBack(lst, 19);
    LIST_DUMP(lst);

    listInsertBefore(lst, node5, 23);
    LIST_DUMP(lst);

    listDel(lst, node2);
    LIST_DUMP(lst);

    listDtor(&lst);
    printf("list after destruction = %p\n", lst);
}
