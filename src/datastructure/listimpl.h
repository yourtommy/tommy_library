#ifndef LISTIMPL_H__
#define LISTIMPL_H__

#include "list.h"

// Used to declare both function pointers and functions
#define API_PROTOTYPE(TYPEDEF, \
        Init, \
        Free, \
        Head, \
        Tail, \
        Next, \
        Prev, \
        Prepend, \
        Append, \
        InsertBefore, \
        InsertAfter, \
        Value) \
    TYPEDEF bool Init(List *list); \
    TYPEDEF bool Free(List *list); \
    TYPEDEF ListItor Head(List *list); \
    TYPEDEF ListItor Tail(List *list); \
    TYPEDEF ListItor Next(List *list, ListItor node); \
    TYPEDEF ListItor Prev(List *list, ListItor node); \
    TYPEDEF bool Prepend(List *list, int value); \
    TYPEDEF bool Append(List *list, int value); \
    TYPEDEF bool InsertBefore(List *list, ListItor node, int value); \
    TYPEDEF bool InsertAfter(List *list, ListItor node, int value); \
    TYPEDEF int Value(List *list, ListItor node);

#define DECLARE_FUNCTIONS(LIST_TYPE) \
    API_PROTOTYPE(, \
            LIST_TYPE ## Init, \
            LIST_TYPE ## Free, \
            LIST_TYPE ## Head, \
            LIST_TYPE ## Tail, \
            LIST_TYPE ## Next, \
            LIST_TYPE ## Prev, \
            LIST_TYPE ## Prepend, \
            LIST_TYPE ## Append, \
            LIST_TYPE ## InsertBefore, \
            LIST_TYPE ## InsertAfter, \
            LIST_TYPE ## Value); 

// Declare function pointers
API_PROTOTYPE(typedef, 
        (*InitPtr),
        (*FreePtr), 
        (*HeadPtr), 
        (*TailPtr), 
        (*NextPtr), 
        (*PrevPtr), 
        (*PrependPtr), 
        (*AppendPtr),
        (*InsertBeforePtr), 
        (*InsertAfterPtr),
        (*ValuePtr))

DECLARE_FUNCTIONS(DLSList)

#define DEFINE_FUNCTION_TABLE(op) \
    static op ## Ptr op ## Table[] = { \
        & DLSList ## op \
    };

#define DEFINE_FUNCTION_TABLES \
    DEFINE_FUNCTION_TABLE(Init) \
    DEFINE_FUNCTION_TABLE(Free) \
    DEFINE_FUNCTION_TABLE(Head) \
    DEFINE_FUNCTION_TABLE(Tail) \
    DEFINE_FUNCTION_TABLE(Next) \
    DEFINE_FUNCTION_TABLE(Prev) \
    DEFINE_FUNCTION_TABLE(Prepend) \
    DEFINE_FUNCTION_TABLE(Append) \
    DEFINE_FUNCTION_TABLE(InsertBefore) \
    DEFINE_FUNCTION_TABLE(InsertAfter) \
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

