#include "listimpl.h"
#include <stddef.h>
#include <stdlib.h>

// ===========  List *functions table  =============
DEFINE_FUNCTION_TABLES 
// ================================================
static ListItor null_itor = { .ptr = NULL, .listp = NULL };

// ================= Generic List ==================

bool ListInit(List *listp, ListType type)
{
    if (listp == NULL)
        return false;

    listp->type = type;
    InitPtr ptr = InitTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp);
}

bool ListFree(List *listp)
{
    if (listp == NULL)
        return false;

    FreePtr ptr = FreeTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp);
}

bool ListItorNull(ListItor itor)
{
    if (itor.listp == NULL)
        return true;

    ItorNullPtr ptr = ItorNullTable[itor.listp->type];
    return ptr == NULL ? true : (*ptr)(itor);
}

bool ListEmpty(List *listp)
{
    if (listp == NULL)
        return true;

    return ListItorNull(ListHead(listp));
}

ListItor ListHead(List *listp)
{
    if (listp == NULL) 
        return null_itor;

    HeadPtr ptr = HeadTable[listp->type];
    return ptr == NULL ? null_itor : (*ptr)(listp);
}

ListItor ListTail(List *listp)
{
    if (listp == NULL)
        return null_itor;

    TailPtr ptr = TailTable[listp->type];
    return ptr == NULL ? null_itor : (*ptr)(listp);
}

bool ListPrepend(List *listp, int value)
{
    if (listp == NULL)
        return false;

    PrependPtr ptr = PrependTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, value);
}

bool ListAppend(List *listp, int value)
{
    if (listp == NULL)
        return false;

    AppendPtr ptr = AppendTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, value);
}

ListItor ListItorNext(ListItor itor)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return null_itor;

    ItorNextPtr ptr = ItorNextTable[itor.listp->type];
    return ptr == NULL ? null_itor : (*ptr)(itor);
}

ListItor ListItorPrev(ListItor itor)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return null_itor;

    ItorPrevPtr ptr = ItorPrevTable[itor.listp->type];
    return ptr == NULL ? null_itor : (*ptr)(itor);
}

bool ListInsertBefore(ListItor itor, int value)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return false;

    InsertBeforePtr ptr = InsertBeforeTable[itor.listp->type];
    return ptr == NULL ? false : (*ptr)(itor, value);
}

bool ListInsertAfter(ListItor itor, int value)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return false;

    InsertAfterPtr ptr = InsertAfterTable[itor.listp->type];
    return ptr == NULL ? false : (*ptr)(itor, value);
}

bool ListDelete(ListItor itor)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return false;

    DeletePtr ptr = DeleteTable[itor.listp->type];
    return ptr == NULL ? false : (*ptr)(itor);
}

bool ListDeleteAll(List *listp)
{
    if (listp == NULL)
        return false;

    DeleteAllPtr ptr = DeleteAllTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp);
}

ListItor ListSearch(List *listp, int value)
{
    if (listp == NULL)
        return null_itor;

    SearchPtr ptr = SearchTable[listp->type];
    return ptr == NULL ? null_itor : (*ptr)(listp, value);
}

bool ListSetValue(ListItor itor, int value)
{
    if (itor.listp == NULL)
        return false;

    SetValuePtr ptr = SetValueTable[itor.listp->type];
    return ptr == NULL ? false : (*ptr)(itor, value);
}

int ListValue(ListItor itor)
{
    if (itor.listp == NULL || ListItorNull(itor))
        return false;

    ValuePtr ptr = ValueTable[itor.listp->type];
    return ptr == NULL ? INFINITY : (*ptr)(itor);
}

// =================================================



// ======= LT_DLS (Double Linked Sentinel) =========

bool
DLSListInit(List *listp)
{
    ListNode *sentinelp = malloc(sizeof(ListNode));
    sentinelp->next = sentinelp->prev = sentinelp;
    listp->head_tail[0].listp = listp->head_tail[1].listp = listp;
    listp->head_tail[0].ptr = listp->head_tail[1].ptr = sentinelp;
    
    return true;
}

bool
DLSListFree(List *listp)
{
    if (!DLSListDeleteAll(listp))
        return false;
    return true;
}

bool
DLSListItorNull(ListItor itor) 
{
    return itor.ptr == itor.listp->head_tail[0].ptr;
}

ListItor 
DLSListHead(List *listp)
{
    return (ListItor){ 
        .ptr = ((ListNode *)listp->head_tail[0].ptr)->next, 
        .listp = listp };
}

ListItor 
DLSListTail(List *listp)
{
    return (ListItor){ 
        .ptr = ((ListNode *)listp->head_tail[0].ptr)->prev, 
        .listp = listp };
}

ListItor 
DLSListItorNext(ListItor itor)
{
    return (ListItor){
        .ptr = ((ListNode *)itor.ptr)->next,
        .listp = itor.listp };
}

ListItor 
DLSListItorPrev(ListItor itor)
{
    return (ListItor){
        .ptr = ((ListNode *)itor.ptr)->prev,
        .listp = itor.listp };
}

bool 
DLSListPrepend(List *listp, int value)
{
    return DLSListInsertBefore(DLSListHead(listp), value);
}

bool 
DLSListAppend(List *listp, int value)
{
    return DLSListInsertAfter(DLSListTail(listp), value);
}

bool 
DLSListInsertBefore(ListItor itor, int value)
{
    ListNode *nodep = itor.ptr;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->next = nodep;
    new_nodep->prev = nodep->prev;
    nodep->prev = new_nodep;
    new_nodep->prev->next = new_nodep;

    return true;
}

bool 
DLSListInsertAfter(ListItor itor, int value)
{
    ListNode *nodep = itor.ptr;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->prev = nodep;
    new_nodep->next = nodep->next;
    nodep->next = new_nodep;
    new_nodep->next->prev = new_nodep;

    return true;
}

bool 
DLSListDelete(ListItor itor)
{
    ListNode *nodep = itor.ptr;
    nodep->prev->next = nodep->next;
    nodep->next->prev = nodep->prev;

    return true;
}

bool 
DLSListDeleteAll(List *listp)
{
    ListNode *headp = DLSListHead(listp).ptr;
    while (headp != listp->head_tail[0].ptr) {
        ListNode *p = headp->next;
        free(headp);
        headp = p;
    }
    ListNode *sentinel = listp->head_tail[0].ptr;
    sentinel->next = sentinel->prev = sentinel;
    return true;
}

ListItor 
DLSListSearch(List *listp, int value)
{
    ListNode *headp = DLSListHead(listp).ptr;
    while (headp != listp->head_tail[0].ptr && headp->value != value)
        headp = headp->next;
    return (ListItor){ .ptr = headp, .listp = listp };
}

bool 
DLSListSetValue(ListItor itor, int value)
{
    ((ListNode *)itor.ptr)->value = value;
    return true;
}

int 
DLSListValue(ListItor itor)
{
    return ((ListNode *)itor.ptr)->value;
}

// =================================================


// ======= LT_DLNS (Double Linked No Sentinel) =========

bool 
DLNSListInit(List *listp)
{
    listp->head_tail[0].listp = listp->head_tail[1].listp = listp;
    listp->head_tail[0].ptr = listp->head_tail[1].ptr = NULL;
    return true;
}

bool 
DLNSListFree(List *listp)
{
    while (listp->head_tail[0].ptr != NULL) {
        ListNode *headp = ((ListNode*)listp->head_tail[0].ptr)->next;
        free(listp->head_tail[0].ptr);
        listp->head_tail[0].ptr = headp;
    }
    return true;
}

bool
DLNSListItorNull(ListItor itor) 
{
    return itor.ptr == NULL;
}

ListItor 
DLNSListHead(List *listp)
{
    return listp->head_tail[0];
}

ListItor 
DLNSListTail(List *listp)
{
    return listp->head_tail[1];
}

ListItor 
DLNSListItorNext(ListItor itor)
{
    return (ListItor){ .ptr = ((ListNode *)itor.ptr)->next, .listp = itor.listp };
}

ListItor 
DLNSListItorPrev(ListItor itor)
{
    return (ListItor){ .ptr = ((ListNode *)itor.ptr)->prev, .listp = itor.listp };
}

bool 
DLNSListPrepend(List *listp, int value)
{
    if (listp->head_tail[0].ptr == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0].ptr = listp->head_tail[1].ptr = new_nodep;
        return true;
    } else
        return DLNSListInsertBefore(listp->head_tail[0], value);
}

bool 
DLNSListAppend(List *listp, int value)
{
    if (listp->head_tail[1].ptr == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0].ptr = listp->head_tail[1].ptr = new_nodep;
        return true;
    } else
        return DLNSListInsertAfter(listp->head_tail[1], value);
}

bool 
DLNSListInsertBefore(ListItor itor, int value)
{
    ListNode *nodep = itor.ptr;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->next = nodep;
    new_nodep->prev = nodep->prev;
    nodep->prev = new_nodep;
    if (new_nodep->prev != NULL)
        new_nodep->prev->next = new_nodep;
    else
        itor.listp->head_tail[0].ptr = new_nodep;

    return true;
}

bool 
DLNSListInsertAfter(ListItor itor, int value)
{
    ListNode *nodep = itor.ptr;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->prev = nodep;
    new_nodep->next = nodep->next;
    nodep->next = new_nodep;
    if (new_nodep->next != NULL)
        new_nodep->next->prev = new_nodep;
    else
        itor.listp->head_tail[1].ptr = new_nodep;

    return true;
}

bool 
DLNSListDelete(ListItor itor)
{
    ListNode *nodep = itor.ptr;
    if (nodep->prev != NULL)
        nodep->prev->next = nodep->next;
    if (nodep->next != NULL)
        nodep->next->prev = nodep->prev;

    if (itor.listp->head_tail[0].ptr == nodep)
        itor.listp->head_tail[0].ptr = nodep->next;
    if (itor.listp->head_tail[1].ptr == nodep)
        itor.listp->head_tail[1].ptr = nodep->prev;
    free(nodep);
    return true;
}

bool 
DLNSListDeleteAll(List *listp)
{
    ListNode *headp = listp->head_tail[0].ptr;
    ListNode *p = headp;
    while (headp != NULL) {
        p = headp->next;
        free(headp);
        headp = p;
    }
    listp->head_tail[0].ptr = listp->head_tail[1].ptr = NULL;
    return true;
}

ListItor 
DLNSListSearch(List *listp, int value)
{
    ListNode *headp = listp->head_tail[0].ptr;
    while (headp != NULL && headp->value != value)
        headp = headp->next;
    return (ListItor){ .ptr = headp, .listp = listp };
}

bool 
DLNSListSetValue(ListItor itor, int value)
{
    ((ListNode *)itor.ptr)->value = value;
    return true;
}

int 
DLNSListValue(ListItor itor)
{
    return ((ListNode *)itor.ptr)->value;
}

// =================================================
