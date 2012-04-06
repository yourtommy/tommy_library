#ifndef LIST_H__
#define LIST_H__

#include <stdbool.h>

struct List
{
    int value;
    struct List *prev;
    struct List *next;
};

bool ListInsertBefore(struct List *where, struct List *node);
bool ListInsertAfter(struct List *where, struct List *node);
bool ListDelete(struct List *node); 
struct List * ListSearch(struct List *l, int key);

#endif

