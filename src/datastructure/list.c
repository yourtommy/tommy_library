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

bool ListEmpty(List *listp)
{
    if (listp == NULL)
        return true;

    return ListHead(listp) == NULL;
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
    if (listp == NULL || itor == NULL)
        return NULL;

    NextPtr ptr = NextTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp, itor);
}

ListItor ListPrev(List *listp, ListItor itor)
{
    if (listp == NULL || itor == NULL)
        return NULL;

    PrevPtr ptr = PrevTable[listp->type];
    return ptr == NULL ? NULL : (*ptr)(listp, itor);
}

bool ListInsertBefore(List *listp, ListItor itor, int value)
{
    if (listp == NULL || itor == NULL)
        return false;

    InsertBeforePtr ptr = InsertBeforeTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor, value);
}

bool ListInsertAfter(List *listp, ListItor itor, int value)
{
    if (listp == NULL || itor == NULL)
        return false;

    InsertAfterPtr ptr = InsertAfterTable[listp->type];
    return ptr == NULL ? false : (*ptr)(listp, itor, value);
}

bool ListDelete(List *listp, ListItor itor)
{
    if (listp == NULL || itor == NULL)
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
    if (listp == NULL || itor == NULL)
        return false;

    ValuePtr ptr = ValueTable[listp->type];
    return ptr == NULL ? INFINITY : (*ptr)(listp, itor);
}

// =================================================



// ======= DLSListLT_ (Double Linked Sentinel) =========

bool DLSListInit(List *listp)
{
    listp->head_tail[0] = listp->head_tail[1] = NULL;
    return true;
}

bool DLSListFree(List *listp)
{
    while (listp->head_tail[0] != NULL) {
        ListNode *headp = ((ListNode*)listp->head_tail[0])->next;
        free(listp->head_tail[0]);
        listp->head_tail[0] = headp;
    }
    return true;
}

ListItor DLSListHead(List *listp)
{
    return listp->head_tail[0];
}

ListItor DLSListTail(List *listp)
{
    return listp->head_tail[1];
}

ListItor DLSListNext(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->next;
}

ListItor DLSListPrev(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->prev;
}

bool DLSListPrepend(List *listp, int value)
{
    if (listp->head_tail[0] == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0] = listp->head_tail[1] = new_nodep;
        return true;
    } else
        return DLSListInsertBefore(listp, listp->head_tail[0], value);
}

bool DLSListAppend(List *listp, int value)
{
    if (listp->head_tail[1] == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_tail[0] = listp->head_tail[1] = new_nodep;
        return true;
    } else
        return DLSListInsertAfter(listp, listp->head_tail[1], value);
}

bool DLSListInsertBefore(List *listp, ListItor itor, int value)
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

bool DLSListInsertAfter(List *listp, ListItor itor, int value)
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

bool DLSListDelete(List *listp, ListItor itor)
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

bool DLSListDeleteAll(List *listp)
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

ListItor DLSListSearch(List *listp, int value)
{
    ListNode *headp = listp->head_tail[0];
    while (headp != NULL && headp->value != value)
        headp = headp->next;
    return headp;
}

bool DLSListSetValue(List *listp, ListItor itor, int value)
{
    UNUSED(listp);
    ((ListNode *)itor)->value = value;
    return true;
}

int DLSListValue(List *listp, ListItor itor)
{
    UNUSED(listp);
    return ((ListNode *)itor)->value;
}

// =================================================


#ifdef TOREMOVE

bool 
DeleteDLList(DLList itor)
{
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
InsertBeforeDLSList(DLSList nodep, DLSList itor)
{
    if (nodep == NULL || itor == NULL)
        return false;
    itor->prev = nodep->prev;
    itor->next = nodep;
    nodep->prev->next = itor;
    nodep->prev = itor;
    return false;
}

bool
InsertAfterDLSList(DLSList nodep, DLSList itor)
{
    if (nodep == NULL || itor == NULL)
        return false;
    itor->next = nodep->next;
    itor->prev = nodep;
    nodep->next->prev = itor;
    nodep->next = itor;
    return true;
}

bool 
DeleteDLSList(DLSList itor) 
{
    if (itor == NULL)
        return false;
    itor->prev->next = itor->next;
    itor->next->prev = itor->prev;
    return true;
}

DLSList  
SearchDLSList(DLSList headp, int key)
{
    if (headp == NULL)
        return NULL;
    DLSList p = headp;
    while (p != headp->prev && p->value != key)
        p = p->next;
    return p;
}

#endif

