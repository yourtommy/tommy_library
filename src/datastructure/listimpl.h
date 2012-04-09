#ifndef LISTIMPL_H__
#define LISTIMPL_H__

#include "list.h"

// Used to declare both function pointers and functions
#define API_PROTOTYPE(TYPEDEF, \
        Init, \
        Free, \
        ItorNull, \
        Head, \
        Tail, \
        Next, \
        Prev, \
        Prepend, \
        Append, \
        InsertBefore, \
        InsertAfter, \
        Delete, \
        DeleteAll, \
        Search, \
        SetValue, \
        Value) \
    TYPEDEF bool Init(List *listp); \
    TYPEDEF bool Free(List *listp); \
    TYPEDEF bool ItorNull(List *listp, ListItor itor); \
    TYPEDEF ListItor Head(List *listp); \
    TYPEDEF ListItor Tail(List *listp); \
    TYPEDEF ListItor Next(List *listp, ListItor itor); \
    TYPEDEF ListItor Prev(List *listp, ListItor itor); \
    TYPEDEF bool Prepend(List *listp, int value); \
    TYPEDEF bool Append(List *listp, int value); \
    TYPEDEF bool InsertBefore(List *listp, ListItor itor, int value); \
    TYPEDEF bool InsertAfter(List *listp, ListItor itor, int value); \
    TYPEDEF bool Delete(List *listp, ListItor itor); \
    TYPEDEF bool DeleteAll(List *listp); \
    TYPEDEF ListItor Search(List *listp, int value); \
    TYPEDEF bool SetValue(List *listp, ListItor itor, int value); \
    TYPEDEF int Value(List *listp, ListItor itor);

#define DECLARE_FUNCTIONS(LIST_TYPE) \
    API_PROTOTYPE(, \
            LIST_TYPE ## Init, \
            LIST_TYPE ## Free, \
            LIST_TYPE ## ItorNull, \
            LIST_TYPE ## Head, \
            LIST_TYPE ## Tail, \
            LIST_TYPE ## Next, \
            LIST_TYPE ## Prev, \
            LIST_TYPE ## Prepend, \
            LIST_TYPE ## Append, \
            LIST_TYPE ## InsertBefore, \
            LIST_TYPE ## InsertAfter, \
            LIST_TYPE ## Delete, \
            LIST_TYPE ## DeleteAll, \
            LIST_TYPE ## Search, \
            LIST_TYPE ## SetValue, \
            LIST_TYPE ## Value) 

// Declare function pointers
API_PROTOTYPE(typedef, 
        (*InitPtr),
        (*FreePtr), 
        (*ItorNullPtr), 
        (*HeadPtr), 
        (*TailPtr), 
        (*NextPtr), 
        (*PrevPtr), 
        (*PrependPtr), 
        (*AppendPtr),
        (*InsertBeforePtr), 
        (*InsertAfterPtr),
        (*DeletePtr),
        (*DeleteAllPtr),
        (*SearchPtr),
        (*SetValuePtr),
        (*ValuePtr))

DECLARE_FUNCTIONS(DLSList)
DECLARE_FUNCTIONS(DLNSList)

#define DEFINE_FUNCTION_TABLE(op) \
    static op ## Ptr op ## Table[] = { \
        & DLSList ## op, \
        & DLNSList ## op, \
        NULL \
    };

#define DEFINE_FUNCTION_TABLES \
    DEFINE_FUNCTION_TABLE(Init) \
    DEFINE_FUNCTION_TABLE(Free) \
    DEFINE_FUNCTION_TABLE(ItorNull) \
    DEFINE_FUNCTION_TABLE(Head) \
    DEFINE_FUNCTION_TABLE(Tail) \
    DEFINE_FUNCTION_TABLE(Next) \
    DEFINE_FUNCTION_TABLE(Prev) \
    DEFINE_FUNCTION_TABLE(Prepend) \
    DEFINE_FUNCTION_TABLE(Append) \
    DEFINE_FUNCTION_TABLE(InsertBefore) \
    DEFINE_FUNCTION_TABLE(InsertAfter) \
    DEFINE_FUNCTION_TABLE(Delete) \
    DEFINE_FUNCTION_TABLE(DeleteAll) \
    DEFINE_FUNCTION_TABLE(Search) \
    DEFINE_FUNCTION_TABLE(SetValue) \
    DEFINE_FUNCTION_TABLE(Value)

/* DLS types */
typedef struct ListNode
{
    int value;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

#define UNUSED(p) ((void)(p))

#endif

