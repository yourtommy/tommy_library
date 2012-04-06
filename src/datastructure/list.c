#include "list.h"
#include <stddef.h>

bool
ListInsertBefore(struct List *where, struct List *node)
{
    if (node == NULL || where == NULL)
        return false;
    node->next = where;
    node->prev = where->prev;
    where->prev = node;
    if (node->prev != NULL)
        node->prev->next = node;

    return true;
}

bool
ListInsertAfter(struct List *where, struct List *node)
{
    if (node == NULL || where == NULL)
        return false;
    node->prev = where;
    node->next = where->next;
    where->next = node;
    if (where->next != NULL)
        where->next->prev = node;
    return true;
}

bool 
ListDelete(struct List *node)
{
    if (node == NULL)
        return false;
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
    return true;
}

struct List * 
ListSearch(struct List *l, int key)
{
    struct List *p = l;
    while (p != NULL && p->value != key) 
        p = p->next;
    return p;
}
