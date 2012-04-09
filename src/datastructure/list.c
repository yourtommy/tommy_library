#include "listimpl.h"
#include <stddef.h>
#include <stdlib.h>

// ===========  List *functions table  =============
DEFINE_FUNCTION_TABLES 
// ================================================


// ================= Generic List ==================

bool ListInit(List *list, ListType type)
{
    list->type = type;
    InitPtr ptr = InitTable[list->type];
    return ptr == NULL ? false : (*ptr)(list);
}

bool ListFree(List *list)
{
    FreePtr ptr = FreeTable[list->type];
    return ptr == NULL ? false : (*ptr)(list);
}

bool ListEmpty(List *list)
{
    return ListHead(list) == NULL;
}

ListItor ListHead(List *list)
{
    HeadPtr ptr = HeadTable[list->type];
    return ptr == NULL ? NULL : (*ptr)(list);
}

ListItor ListTail(List *list)
{
    TailPtr ptr = TailTable[list->type];
    return ptr == NULL ? NULL : (*ptr)(list);
}

bool ListPrepend(List *list, int value)
{
    PrependPtr ptr = PrependTable[list->type];
    return ptr == NULL ? false : (*ptr)(list, value);
}

bool ListAppend(List *list, int value)
{
    AppendPtr ptr = AppendTable[list->type];
    return ptr == NULL ? false : (*ptr)(list, value);
}

ListItor ListNext(List *list, ListItor node)
{
    NextPtr ptr = NextTable[list->type];
    return ptr == NULL ? NULL : (*ptr)(list, node);
}

ListItor ListPrev(List *list, ListItor node)
{
    PrevPtr ptr = PrevTable[list->type];
    return ptr == NULL ? NULL : (*ptr)(list, node);
}

bool ListInsertBefore(List *list, ListItor node, int value)
{
    InsertBeforePtr ptr = InsertBeforeTable[list->type];
    return ptr == NULL ? false : (*ptr)(list, node, value);
}

bool ListInsertAfter(List *list, ListItor node, int value)
{
    InsertAfterPtr ptr = InsertAfterTable[list->type];
    return ptr == NULL ? false : (*ptr)(list, node, value);
}

int ListValue(List *list, ListItor node)
{
    ValuePtr ptr = ValueTable[list->type];
    return ptr == NULL ? INFINITY : (*ptr)(list, node);
}

// =================================================



// ======= DLSListLT_ (Double Linked Sentinel) =========

bool DLSListInit(List *list)
{
    list->head_tail[0] = list->head_tail[1] = NULL;
    return true;
}

bool DLSListFree(List *list)
{
    while (list->head_tail[0] != NULL) {
        ListNode *head = ((ListNode*)list->head_tail[0])->next;
        free(list->head_tail[0]);
        list->head_tail[0] = head;
    }
    return true;
}

ListItor DLSListHead(List *list)
{
    return list->head_tail[0];
}

ListItor DLSListTail(List *list)
{
    return list->head_tail[1];
}

ListItor DLSListNext(List *list, ListItor node)
{
    UNUSED(list);
    return ((ListNode *)node)->next;
}

ListItor DLSListPrev(List *list, ListItor node)
{
    UNUSED(list);
    return ((ListNode *)node)->prev;
}

bool DLSListPrepend(List *list, int value)
{
    if (list->head_tail[0] == NULL) {
        ListNode *new_node = malloc(sizeof(ListNode));
        new_node->value = value;
        list->head_tail[0] = list->head_tail[1] = new_node;
        return true;
    } else
        return DLSListInsertBefore(list, list->head_tail[0], value);
}

bool DLSListAppend(List *list, int value)
{
    if (list->head_tail[1] == NULL) {
        ListNode *new_node = malloc(sizeof(ListNode));
        new_node->value = value;
        list->head_tail[0] = list->head_tail[1] = new_node;
        return true;
    } else
        return DLSListInsertAfter(list, list->head_tail[1], value);
}

bool DLSListInsertBefore(List *list, ListItor node, int value)
{
    ListNode *where = node;
    ListNode *new_node = malloc(sizeof(ListNode));
    new_node->value = value;

    new_node->next = where;
    new_node->prev = where->prev;
    where->prev = new_node;
    if (new_node->prev != NULL)
        new_node->prev->next = new_node;

    if (list->head_tail[0] == node)
        list->head_tail[0] = new_node;

    return true;
}

bool DLSListInsertAfter(List *list, ListItor node, int value)
{
    ListNode *where = node;
    ListNode *new_node = malloc(sizeof(ListNode));
    new_node->value = value;

    new_node->prev = where;
    new_node->next = where->next;
    where->next = new_node;
    if (new_node->next != NULL)
        new_node->next->prev = new_node;

    if (list->head_tail[1] == node)
        list->head_tail[1] = new_node;

    return true;
}

int DLSListValue(List *list, ListItor node)
{
    UNUSED(list);
    return ((ListNode *)node)->value;
}

// =================================================


#ifdef TOREMOVE

bool 
DeleteDLList(DLList node)
{
    if (node == NULL)
        return false;
    if (node->prev != NULL)
        node->prev->next = node->next;
    if (node->next != NULL)
        node->next->prev = node->prev;
    return true;
}

DLList  
SearchDLList(DLList l, int key)
{
    DLList p = l;
    while (p != NULL && p->value != key) 
        p = p->next;
    return p;
}

bool
InsertBeforeDLSList(DLSList where, DLSList node)
{
    if (where == NULL || node == NULL)
        return false;
    node->prev = where->prev;
    node->next = where;
    where->prev->next = node;
    where->prev = node;
    return false;
}

bool
InsertAfterDLSList(DLSList where, DLSList node)
{
    if (where == NULL || node == NULL)
        return false;
    node->next = where->next;
    node->prev = where;
    where->next->prev = node;
    where->next = node;
    return true;
}

bool 
DeleteDLSList(DLSList node) 
{
    if (node == NULL)
        return false;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    return true;
}

DLSList  
SearchDLSList(DLSList head, int key)
{
    if (head == NULL)
        return NULL;
    DLSList p = head;
    while (p != head->prev && p->value != key)
        p = p->next;
    return p;
}

#endif

