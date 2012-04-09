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

/* List cannot be used after being freed unless being init'd again */
bool ListFree(List *list);

/* Return true if list is empty */
bool ListEmpty(List *list);

/* Get the itor of the first element. Return NULL if list is empty */
ListItor ListHead(List *list);

/* Get the itor of the last element. Return NULL if list is empty */
ListItor ListTail(List *list);

/* Get the next element of itor. Return NULL if itor is tail */
ListItor ListNext(List *list, ListItor itor);

/* Get the previous element of itor. Return NULL if itor is head */
ListItor ListPrev(List *list, ListItor itor);

/* Insert element as the first itor. */
bool ListPrepend(List *list, int value);

/* Insert element as the last itor. */
bool ListAppend(List *list, int value);

/* Insert element before itor. Return true if successful. itor cannot be null. */
bool ListInsertBefore(List *list, ListItor itor, int value);

/* Insert element after itor. Return true if successful. itor cannot be null. */
bool ListInsertAfter(List *list, ListItor itor, int value);

/* Delete itor from list */
bool ListDelete(List *list, ListItor itor);

/* Delete all nodes from list. This function is different from ListFree
 * since it may not free all allocated memory and can be used without
 * another init.
 */
bool ListDeleteAll(List *list);

/* Get the value stored in itor */
int ListValue(List *list, ListItor itor);

#endif

