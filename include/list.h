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
} ListType;

typedef void *ListItor;

typedef struct List
{
    ListType type;
    ListItor head_tail[2];
} List;


/* Must init before using a listp */
bool ListInit(List *listp, ListType type);

/* List cannot be used after being freed unless being init'd again */
bool ListFree(List *listp);

/* Return true if listp is empty */
bool ListEmpty(List *listp);

/* Get the itor of the first element. Return NULL if listp is empty */
ListItor ListHead(List *listp);

/* Get the itor of the last element. Return NULL if listp is empty */
ListItor ListTail(List *listp);

/* Get the next element of itor. Return NULL if itor is tail */
ListItor ListNext(List *listp, ListItor itor);

/* Get the previous element of itor. Return NULL if itor is head */
ListItor ListPrev(List *listp, ListItor itor);

/* Insert element as the first itor. */
bool ListPrepend(List *listp, int value);

/* Insert element as the last itor. */
bool ListAppend(List *listp, int value);

/* Insert element before itor. Return true if successful. itor cannot be null. */
bool ListInsertBefore(List *listp, ListItor itor, int value);

/* Insert element after itor. Return true if successful. itor cannot be null. */
bool ListInsertAfter(List *listp, ListItor itor, int value);

/* Delete itor from listp */
bool ListDelete(List *listp, ListItor itor);

/* Delete all nodes from listp. This function is different from ListFree
 * since it may not free all allocated memory and can be used without
 * another init.
 */
bool ListDeleteAll(List *listp);

/* Search value in listp and return the itor storing that value.
 * Return Null if not found.
 */
ListItor ListSearch(List *listp, int value); 

/* Stor the value into itor */
bool ListSetValue(List *listp, ListItor itor, int value); 
    
/* Get the value stored in itor */
int ListValue(List *listp, ListItor itor);

#endif

