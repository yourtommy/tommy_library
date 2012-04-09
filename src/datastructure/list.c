#include "listimpl.h"
#include <stddef.h>
#include <stdlib.h>

// ===========  List *functions table  =============
DEFINE_FUNCTION_TABLES 
// ================================================


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

bool ListItorNull(List *listp, ListItor itor)
{
    if (listp == NULL)
        return true;

    ItorNullPtr ptr = ItorNullTable[listp->type];
    return ptr == NULL ? true : (*ptr)(listp, itor);
}

bool ListEmpty(List *listp)
{
    if (listp == NULL)
        return true;

    return ListItorNull(listp, ListHead(listp));
}

ListItor ListHead(List *listp)
{
    if (listp == NULL)
        return NULL;

    HeadPtr ptr = HeadTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp);
}

ListItor ListTail(List *listp)
{
    if (listp == NULL)
        return NULL;

    TailPtr ptr = TailTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp);
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

ListItor ListNext(List *listp, ListItor itor)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return NULL;

    NextPtr ptr = NextTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp, itor);
}

ListItor ListPrev(List *listp, ListItor itor)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return NULL;

    PrevPtr ptr = PrevTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp, itor);
}

bool ListInsertBefore(List *listp, ListItor itor, int value)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return false;

    InsertBeforePtr ptr = InsertBeforeTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor, value);
}

bool ListInsertAfter(List *listp, ListItor itor, int value)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return false;

    InsertAfterPtr ptr = InsertAfterTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor, value);
}

bool ListDelete(List *listp, ListItor itor)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return false;

    DeletePtr ptr = DeleteTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor);
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
        return NULL;

    SearchPtr ptr = SearchTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp, value);
}

bool ListSetValue(List *listp, ListItor itor, int value)
{
    if (listp == NULL)
        return false;

    SetValuePtr ptr = SetValueTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor, value);
}

int ListValue(List *listp, ListItor itor)
{
    if (listp == NULL || ListItorNull(listp, itor))
        return false;

    ValuePtr ptr = ValueTable[listp->type];
    return ptr == NULL ? INFINITY : (*ptr)(listp, itor);
}

// =================================================



// ======= LT_DLS (Double Linked Sentinel) =========

bool
DLSListInit(List *listp)
{
    ListNode *sentinelp = malloc(sizeof(ListNode));
    sentinelp->next = sentinelp->prev = sentinelp;
    listp->head_tail[0] = listp->head_tail[1] = sentinelp;
    
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
DLSListItorNull(List *listp, ListItor itor) 
{
    return itor == listp->head_tail[0];
}

ListItor 
DLSListHead(List *listp)
{
    return ((ListNode *)listp->head_tail[0])->next;
}

ListItor 
DLSListTail(List *listp)
{
    return ((ListNode *)listp->head_tail[0])->prev;
}

ListItor 
DLSListNext(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->next;
}

ListItor 
DLSListPrev(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->prev;
}

bool 
DLSListPrepend(List *listp, int value)
{
    return DLSListInsertBefore(listp, DLSListHead(listp), value);
}

bool 
DLSListAppend(List *listp, int value)
{
    return DLSListInsertAfter(listp, DLSListTail(listp), value);
}

bool 
DLSListInsertBefore(List *listp, ListItor itor, int value)
{
    UNUSED(listp);
    ListNode *nodep = itor;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->next = nodep;
    new_nodep->prev = nodep->prev;
    nodep->prev = new_nodep;
    new_nodep->prev->next = new_nodep;

    return true;
}

bool 
DLSListInsertAfter(List *listp, ListItor itor, int value)
{
    UNUSED(listp);
    ListNode *nodep = itor;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->prev = nodep;
    new_nodep->next = nodep->next;
    nodep->next = new_nodep;
    new_nodep->next->prev = new_nodep;

    return true;
}

bool 
DLSListDelete(List *listp, ListItor itor)
{
    UNUSED(listp);
    ListNode *nodep = itor;
    nodep->prev->next = nodep->next;
    nodep->next->prev = nodep->prev;

    return true;
}

bool 
DLSListDeleteAll(List *listp)
{
    ListNode *headp = DLSListHead(listp);
    while (headp != listp->head_tail[0]) {
        ListNode *p = headp->next;
        free(headp);
        headp = p;
    }
    ListNode *sentinel = listp->head_tail[0];
    sentinel->next = sentinel->prev = sentinel;
    return true;
}

ListItor 
DLSListSearch(List *listp, int value)
{
    ListNode *headp = DLSListHead(listp);
    while (!ListItorNull(listp, headp) && headp->value != value)
        headp = headp->next;
    return headp;
}

bool 
DLSListSetValue(List *listp, ListItor itor, int value)
{
    UNUSED(listp);
    ((ListNode *)itor)->value = value;
    return true;
}

int 
DLSListValue(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->value;
}

// =================================================


// ======= LT_DLNS (Double Linked No Sentinel) =========

bool 
DLNSListInit(List *listp)
{
    listp->head_tail[0] = listp->head_tail[1] = NULL;
    return true;
}

bool 
DLNSListFree(List *listp)
{
    while (listp->head_tail[0] != NULL) {
        ListNode *headp = ((ListNode*)listp->head_tail[0])->next;
        free(listp->head_tail[0]);
        listp->head_tail[0] = headp;
    }
    return true;
}

bool
DLNSListItorNull(List *listp, ListItor itor) 
{
    UNUSED(listp);
    return itor == NULL;
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
DLNSListNext(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->next;
}

ListItor 
DLNSListPrev(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->prev;
}

bool 
DLNSListPrepend(List *listp, int value)
{
    if (listp->head_tail[0] == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0] = listp->head_tail[1] = new_nodep;
        return true;
    } else
        return DLNSListInsertBefore(listp, listp->head_tail[0], value);
}

bool 
DLNSListAppend(List *listp, int value)
{
    if (listp->head_tail[1] == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0] = listp->head_tail[1] = new_nodep;
        return true;
    } else
        return DLNSListInsertAfter(listp, listp->head_tail[1], value);
}

bool 
DLNSListInsertBefore(List *listp, ListItor itor, int value)
{
    ListNode *nodep = itor;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->next = nodep;
    new_nodep->prev = nodep->prev;
    nodep->prev = new_nodep;
    if (new_nodep->prev != NULL)
        new_nodep->prev->next = new_nodep;
    else
        listp->head_tail[0] = new_nodep;

    return true;
}

bool 
DLNSListInsertAfter(List *listp, ListItor itor, int value)
{
    ListNode *nodep = itor;
    ListNode *new_nodep = malloc(sizeof(ListNode));
    new_nodep->value = value;

    new_nodep->prev = nodep;
    new_nodep->next = nodep->next;
    nodep->next = new_nodep;
    if (new_nodep->next != NULL)
        new_nodep->next->prev = new_nodep;
    else
        listp->head_tail[1] = new_nodep;

    return true;
}

bool 
DLNSListDelete(List *listp, ListItor itor)
{
    ListNode *nodep = itor;
    if (nodep->prev != NULL)
        nodep->prev->next = nodep->next;
    if (nodep->next != NULL)
        nodep->next->prev = nodep->prev;

    if (listp->head_tail[0] == nodep)
        listp->head_tail[0] = nodep->next;
    if (listp->head_tail[1] == nodep)
        listp->head_tail[1] = nodep->prev;
    free(nodep);
    return true;
}

bool 
DLNSListDeleteAll(List *listp)
{
    ListNode *headp = listp->head_tail[0];
    ListNode *p = headp;
    while (headp != NULL) {
        p = headp->next;
        free(headp);
        headp = p;
    }
    listp->head_tail[0] = listp->head_tail[1] = NULL;
    return true;
}

ListItor 
DLNSListSearch(List *listp, int value)
{
    ListNode *headp = listp->head_tail[0];
    while (headp != NULL && headp->value != value)
        headp = headp->next;
    return headp;
}

bool 
DLNSListSetValue(List *listp, ListItor itor, int value)
{
    UNUSED(listp);
    ((ListNode *)itor)->value = value;
    return true;
}

int 
DLNSListValue(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->value;
}

// =================================================
