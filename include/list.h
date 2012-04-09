#ifndef LIST_H__
#define LIST_H__

#include <stdbool.h>

#ifndef INFINITY
#define INFINITY (int)(~(unsigned)0 >> 1)
#endif

typedef enum ListType
{
    LT_DLS = 0, // DoubleLinkedSentinel 
    LT_DLNS, // DoubleLinkedNoSentinel
    LT_num // total list type num
} ListType;

typedef void *ListItor;

typedef struct List
{
    ListType type;
    ListItor head_tail[2];
} List;


/* Must init before using a list */
bool ListInit(List *list, ListType type);

/* List *cannot be used after being freed unless being init'd again */
bool ListFree(List *list);

/* Return true if list is empty */
bool ListEmpty(List *list);

/* Get the itor of the first element. Return NULL if list is empty */
ListItor ListHead(List *list);

/* Get the itor of the last element. Return NULL if list is empty */
ListItor ListTail(List *list);

/* Get the next element of node. Return NULL if node is tail */
ListItor ListNext(List *list, ListItor node);

/* Get the previous element of node. Return NULL if node is head */
ListItor ListPrev(List *list, ListItor node);

/* Insert element as the first node. */
bool ListPrepend(List *list, int value);

/* Insert element as the last node. */
bool ListAppend(List *list, int value);

/* Insert element before node. Return true if successful. node cannot be null. */
bool ListInsertBefore(List *list, ListItor node, int value);

/* Insert element after node. Return true if successful. node cannot be null. */
bool ListInsertAfter(List *list, ListItor node, int value);

/* Insert element after node. Return true if successful. node cannot be null. */
bool ListInsertAfter(List *list, ListItor node, int value);

/* Get the value stored in node */
int ListValue(List *list, ListItor node);

#endif

