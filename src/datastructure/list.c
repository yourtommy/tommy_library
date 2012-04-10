#include "listimpl.h"
#include <stddef.h>
#include <stdlib.h>

static ListItor null_itor = { .ptr = NULL, .listp = NULL };

static const int null_index = -1;
static int ListValuePool[LIST_POOL_CAPACITY];
static int ListPrevPool[LIST_POOL_CAPACITY];
static int ListNextPool[LIST_POOL_CAPACITY];
static int pool_free;

// ===========  List functions table  =============
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
    listp->head_itor.listp = listp->tail_itor.listp = listp;
    listp->head_itor.ptr = listp->tail_itor.ptr = sentinelp;
    
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
    return itor.ptr == itor.listp->head_itor.ptr;
}

ListItor 
DLSListHead(List *listp)
{
    return (ListItor){ 
        .ptr = ((ListNode *)listp->head_itor.ptr)->next, 
        .listp = listp };
}

ListItor 
DLSListTail(List *listp)
{
    return (ListItor){ 
        .ptr = ((ListNode *)listp->head_itor.ptr)->prev, 
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
    while (headp != listp->head_itor.ptr) {
        ListNode *p = headp->next;
        free(headp);
        headp = p;
    }
    ListNode *sentinel = listp->head_itor.ptr;
    sentinel->next = sentinel->prev = sentinel;
    return true;
}

ListItor 
DLSListSearch(List *listp, int value)
{
    ListNode *headp = DLSListHead(listp).ptr;
    while (headp != listp->head_itor.ptr && headp->value != value)
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
    listp->head_itor.listp = listp->tail_itor.listp = listp;
    listp->head_itor.ptr = listp->tail_itor.ptr = NULL;
    return true;
}

bool 
DLNSListFree(List *listp)
{
    while (listp->head_itor.ptr != NULL) {
        ListNode *headp = ((ListNode*)listp->head_itor.ptr)->next;
        free(listp->head_itor.ptr);
        listp->head_itor.ptr = headp;
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
    return listp->head_itor;
}

ListItor 
DLNSListTail(List *listp)
{
    return listp->tail_itor;
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
    if (listp->head_itor.ptr == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_itor.ptr = listp->tail_itor.ptr = new_nodep;
        return true;
    } else
        return DLNSListInsertBefore(listp->head_itor, value);
}

bool 
DLNSListAppend(List *listp, int value)
{
    if (listp->tail_itor.ptr == NULL) {
        ListNode *new_nodep = malloc(sizeof(ListNode));
        new_nodep->prev = new_nodep->next = NULL;
        new_nodep->value = value;
        listp->head_itor.ptr = listp->tail_itor.ptr = new_nodep;
        return true;
    } else
        return DLNSListInsertAfter(listp->tail_itor, value);
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
        itor.listp->head_itor.ptr = new_nodep;

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
        itor.listp->tail_itor.ptr = new_nodep;

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

    if (itor.listp->head_itor.ptr == nodep)
        itor.listp->head_itor.ptr = nodep->next;
    if (itor.listp->tail_itor.ptr == nodep)
        itor.listp->tail_itor.ptr = nodep->prev;
    free(nodep);
    return true;
}

bool 
DLNSListDeleteAll(List *listp)
{
    ListNode *headp = listp->head_itor.ptr;
    ListNode *p = headp;
    while (headp != NULL) {
        p = headp->next;
        free(headp);
        headp = p;
    }
    listp->head_itor.ptr = listp->tail_itor.ptr = NULL;
    return true;
}

ListItor 
DLNSListSearch(List *listp, int value)
{
    ListNode *headp = listp->head_itor.ptr;
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

// ======= LT_DLA (Double Linked Array) =========

static bool 
InitListPool()
{
    static bool inited = false;
    if (!inited) {
        int i;
        for (i = 0; i < LIST_POOL_CAPACITY; i++) {
            ListNextPool[i] = i+1;
            pool_free = 0;
        }
    }
    return true;
}

static int
AllocPoolNode()
{
    if (pool_free == null_index)
        return null_index;
    int index = pool_free;
    pool_free = ListNextPool[index];
    return index;
}

static void
FreePoolNode(int index)
{
    ListNextPool[index] = pool_free;
    pool_free = index;
}

bool
DLAListInit(List *listp)
{
    if (!InitListPool())
        return false;
    listp->head_itor.index = listp->tail_itor.index = null_index;
    return true;
}

bool
DLAListFree(List *listp)
{
    if (!DLAListDeleteAll(listp))
        return false;
    return true;
}

bool
DLAListItorNull(ListItor itor) 
{
    return itor.index == null_index;
}

ListItor 
DLAListHead(List *listp)
{
    return (ListItor){ 
        .index = listp->head_itor.index, 
        .listp = listp };
}

ListItor 
DLAListTail(List *listp)
{
    return (ListItor){ 
        .index = listp->tail_itor.index, 
        .listp = listp };
}

ListItor 
DLAListItorNext(ListItor itor)
{
    return (ListItor){
        .index = ListNextPool[itor.index],
        .listp = itor.listp };
}

ListItor 
DLAListItorPrev(ListItor itor)
{
    return (ListItor){
        .index = ListPrevPool[itor.index],
        .listp = itor.listp };
}

bool 
DLAListPrepend(List *listp, int value)
{
    if (listp->head_itor.index == null_index) {
        int new_node = AllocPoolNode();
        if (new_node == null_index)
            return false;
        ListValuePool[new_node] = value;
        ListNextPool[new_node] = ListPrevPool[new_node] = null_index;
        listp->head_itor.index = listp->tail_itor.index = new_node;
        return true;
    }
    return DLAListInsertBefore(listp->head_itor, value);
}

bool 
DLAListAppend(List *listp, int value)
{
    if (listp->head_itor.index == null_index) {
        int new_node = AllocPoolNode();
        if (new_node == null_index)
            return false;
        ListValuePool[new_node] = value;
        ListNextPool[new_node] = ListPrevPool[new_node] = null_index;
        listp->head_itor.index = listp->tail_itor.index = new_node;
        return true;
    }
    return DLAListInsertAfter(listp->tail_itor, value);
}

bool 
DLAListInsertBefore(ListItor itor, int value)
{
    int new_node = AllocPoolNode();
    if (new_node == null_index)
        return false;
    ListValuePool[new_node] = value;

    ListPrevPool[new_node] = ListPrevPool[itor.index];
    ListNextPool[new_node] = itor.index;
    ListPrevPool[itor.index] = new_node;
    if (ListPrevPool[new_node] != null_index)
        ListNextPool[ListPrevPool[new_node]] = new_node;

    if (itor.listp->head_itor.index == itor.index)
        itor.listp->head_itor.index = new_node;

    return true;
}

bool 
DLAListInsertAfter(ListItor itor, int value)
{
    int new_node = AllocPoolNode();
    if (new_node == null_index)
        return false;
    ListValuePool[new_node] = value;

    ListNextPool[new_node] = ListNextPool[itor.index];
    ListPrevPool[new_node] = itor.index;
    ListNextPool[itor.index] = new_node;
    if (ListNextPool[new_node] != null_index)
        ListPrevPool[ListNextPool[new_node]] = new_node;

    if (itor.listp->tail_itor.index == itor.index)
        itor.listp->tail_itor.index = new_node;

    return true;
}

bool 
DLAListDelete(ListItor itor)
{
    if (ListPrevPool[itor.index] != null_index)
        ListNextPool[ListPrevPool[itor.index]] = ListNextPool[itor.index];
    if (ListNextPool[itor.index] != null_index)
        ListPrevPool[ListNextPool[itor.index]] = ListPrevPool[itor.index];

    if (itor.listp->head_itor.index == itor.index)
        itor.listp->head_itor.index = ListNextPool[itor.index];
    if (itor.listp->tail_itor.index == itor.index)
        itor.listp->tail_itor.index = ListPrevPool[itor.index];

    FreePoolNode(itor.index);

    return true;
}

bool 
DLAListDeleteAll(List *listp)
{
    int head = listp->head_itor.index;
    while (head != null_index) {
        int index = ListNextPool[head];
        FreePoolNode(head);
        head = index;
    }
    listp->head_itor.index = listp->tail_itor.index = null_index;
    return true;
}

ListItor 
DLAListSearch(List *listp, int value)
{
    int index = listp->head_itor.index;
    while (index != null_index && ListValuePool[index] != value)
        index = ListNextPool[index];
    return (ListItor){ .index = index, .listp = listp };
}

bool 
DLAListSetValue(ListItor itor, int value)
{
    ListValuePool[itor.index] = value;
    return true;
}

int 
DLAListValue(ListItor itor)
{
    return ListValuePool[itor.index];
}

// =================================================

