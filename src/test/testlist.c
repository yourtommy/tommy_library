#include "testset.h"
#include "utility.h"
#include "list.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58902;
static const int TestMaxValue = 58292;

#ifdef TOREMOVE

#define BUILD_LIST(LIST_TYPE)                           \
    LIST_TYPE *head, *tail;                             \
    if (strcmp(#LIST_TYPE, "DLList") == 0) {            \
        head = alloca(sizeof(LIST_TYPE));               \
        head->prev = head->next = NULL;                 \
        tail = head;                                    \
    }

#define INSERT_ALL_BEFORE_HEAD(LIST_TYPE)               \
    head->value = numbers[len-1];                       \
    int i;                                              \
    for (i = len-2; i >= 0; i--) {                      \
        DLList *node = alloca(sizeof(LIST_TYPE));       \
        node->prev = node->next = NULL;                 \
        node->value = numbers[i];                       \
        if (!INSERT_BEFORE_LIST(                        \
                    LIST_TYPE, head, node)) {           \
            sprintf(error, #LIST_TYPE " - Insert before"\
                    " the 1st element failed!!");       \
            return false;                               \
        }                                               \
        head = node;                                    \
    }                                                                           

#define INSERT_ALL_BEFORE_TAIL(LIST_TYPE)               \
    tail = alloca(sizeof(LIST_TYPE));                   \
    tail->prev = tail->next = NULL;                     \
    INSERT_BEFORE_LIST(LIST_TYPE, tail, head);          \
    tail = head;                                        \
    head->value = numbers[0];                           \
    tail->value = numbers[len-1];                       \
    int i;                                              \
    for (i = len-2; i >= 0; i--) {                      \
        DLList *node = alloca(sizeof(LIST_TYPE));       \
        node->prev = node->next = NULL;                 \
        node->value = numbers[i];                       \
        if (!INSERT_BEFORE_LIST(                         \
                    LIST_TYPE, tail, node)) {           \
            sprintf(error, #LIST_TYPE " - Insert before"\
                    " the last element failed!!");      \
            return false;                               \
        }                                               \
        tail = node;                                    \
    }                                                                           

#define VERIFY_LIST_CONSISTENCY                         \
{                                                       \
    DLList *end = head->prev; /* sentinel or null*/     \
    DLList *p = head;                                   \
    int i;                                              \
    for (i = 0; i < len; i++) {                         \
        if (p->value != numbers[i]) {                   \
            sprintf(error, "Encounter wrong data when " \
                    "going from the head to the tail"); \
            return false;                               \
        }                                               \
        p = p->next;                                    \
    }                                                   \
    if (p != end) {                                     \
        sprintf(error, "List *doesn't reach its tail "   \
                "after covering all numbers from the "  \
                "head");                                \
        return false;                                   \
    }                                                   \
    p = tail;                                           \
    for (i = len-1; i >= 0; i--) {                      \
        if (p->value != numbers[i]) {                   \
            sprintf(error, "Encounter wrong data when " \
                    "going from the tail to the head"); \
            return false;                               \
        }                                               \
        p = p->prev;                                    \
    }                                                   \
    if (p != end) {                                     \
        sprintf(error, "List *doesn't reach its head "   \
                "after covering all numbers from the "  \
                "tail");                                \
        return false;                                   \
    }                                                   \
    return true;                                        \
}

bool TestINSERT_BEFORE_LIST(int *numbers, int len, char error[])
{
    {
        BUILD_LIST(DLList)
        INSERT_ALL_BEFORE_HEAD(DLList)
        VERIFY_LIST_CONSISTENCY
    }

#ifdef LATER
    // Insert all elements before the last one.
    DLList *p = alloca(sizeof(DLList));
    p->prev = head->next = NULL;
    p->value = numbers[0];
    head = alloca(sizeof(DLList));
    head->value = numbers[len-1];
    INSERT_BEFORE_LIST(DLList, p, head);
    for (i = len-2; i >= 1; i--) {
        DLList *node = alloca(sizeof(DLList));
        node->value = numbers[i];
        if (!INSERT_BEFORE_LIST(DLList, p, node)) {
            sprintf(error, "InsertBefore the last element failed!!");
            return false;
        }
    }
    p = head;
    for (i = len - 1; i >= 0; i--) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertBefore the last element (%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
#endif

    return true;
}

bool TestINSERT_AFTER_LIST(int *numbers, int len, char error[])
{
    // Insert after the last element
    DLList *head;
    head = alloca(sizeof(DLList));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    DLList *p = head;
    for (i = 1; i < len; i++) {
        DLList *node = alloca(sizeof(DLList));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!INSERT_AFTER_LIST(DLList, p, node)) {
            sprintf(error, "InsertAfter the last element failed!!");
            return false;
        }
        p = node;
    }
    p = head;
    for (i = 0; i < len; i++) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertAfter the last element(%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
    // Walk through the list from tail to head.
    p = head;
    while (p->next != NULL)
        p = p->next;
    for (i = len - 1; i >= 0; i--) {
        if (p->value != numbers[i]) {
            sprintf(error, "(Reverse order) Data inconsitent after InsertAfter the last element (%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->prev;
    }

    // Insert after the first element
    head = alloca(sizeof(DLList));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    p = alloca(sizeof(DLList));
    p->value = numbers[len-1];
    INSERT_AFTER_LIST(DLList, head, p);

    for (i = len - 2; i >= 1; i--) {
        DLList *node = alloca(sizeof(DLList));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!INSERT_AFTER_LIST(DLList, head, node)) {
            sprintf(error, "InsertAfter the first element failed!!");
            return false;
        }
        p = node;
    }
    p = head;
    for (i = 0; i < len; i++) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertAfter the first element(%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
    // Walk through the list from tail to head.
    p = head;
    while (p->next != NULL)
        p = p->next;
    for (i = len - 1; i >= 0; i--) {
        if (p->value != numbers[i]) {
            sprintf(error, "(Reverse order) Data inconsitent after InsertAfter the first element (%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->prev;
    }
    return true;
}

bool TestDELETE_LIST(int *numbers, int len, char error[])
{
    // insert after the last element. It's already been tested.
    DLList *head;
    head = alloca(sizeof(DLList));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    DLList *p = head;
    for (i = 1; i < len; i++) {
        DLList *node = alloca(sizeof(DLList));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        INSERT_AFTER_LIST(DLList, p, node); 
        p = node;
    }

    // Delete the first element.
    p = head->next;
    if (!DELETE_LIST(DLList, head)) {
        sprintf(error, "Delete the first element failed");
        return false;
    }
    if (p->prev != NULL) {
        sprintf(error, "The second element doesn't become head after the first element deleted");
        return false;
    }
    head = p;
    DLList *p1 = head->next;
    DLList *p2 = head->next->next;
    DLList *p3 = head->next->next->next;
    if (!DELETE_LIST(DLList, p2)) {
        sprintf(error, "Delete the 3rd (middle) element failed");
        return false;
    }
    if (p1->next != p3 || p3->prev != p1) {
        sprintf(error, "The 2nd element is not connected to the 4th element after the 3rd element is deleted");
        return false;
    }
    p = head;
    while (p->next != NULL) // reach the last element
        p = p->next;
    p1 = p->prev; // the last 2nd element;
    if (!DELETE_LIST(DLList, p)) {
        sprintf(error, "Delete the last element failed");
        return false;
    }
    if (p1->next != NULL) {
        sprintf(error, "The last 2nd element doesn't become tail after the last element deleted");
        return false;
    }

    return true;
}

bool TestSEARCH_LIST(int *numbers, int len, char error[]) {
    // insert after the last element. It's already been tested.
    DLList *head;
    head = alloca(sizeof(DLList));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    DLList *p = head;
    for (i = 1; i < len; i++) {
        DLList *node = alloca(sizeof(DLList));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        INSERT_AFTER_LIST(DLList, p, node); 
        p = node;
    }

    // Search the 1st element.
    p = SEARCH_LIST(DLList, head, numbers[0]);
    if (p != head) { // The search result must be head because it's the first element of that value.
        sprintf(error, "Searching 1st element failed");
        return false;
    }
    // Search the 5th element.
    p = SEARCH_LIST(DLList, head, numbers[4]);
    if (p == NULL || p->value != numbers[4]) {
        sprintf(error, "Searching 5th element failed");
        return false;
    }
    // Search the last element.
    p = SEARCH_LIST(DLList, head, numbers[len-1]);
    if (p == NULL || p->value != numbers[len-1]) {
        sprintf(error, "Searching the last element failed.");
        return false;
    }

    return true;
}

#endif

static int length;
static int *numbers;
char error[1024] = "";

bool 
VerifyListConsistency(List *list)
{                                                       
    int i;    
    ListItor itor = ListHead(list);
    for (i = 0; i < length; i++) {                        
        if (ListValue(list, itor) != numbers[i]) {                   
            sprintf(error, "Encounter wrong data when walking from the head to the tail"); 
            return false;
        }                                               
        itor = ListNext(list, itor);
    }                                                   
    if (itor != NULL) {                                     
        sprintf(error, "List *has more elements at the end then numbers");                                
        return false;                                   
    }                                                   
    itor = ListTail(list);
    for (i = length-1; i >= 0; i--) {                      
        if (ListValue(list, itor) != numbers[i]) {                   
            sprintf(error, "Encounter wrong data when walking from the tail to the head"); 
            return false; 
        }
        ListPrev(list, itor);
    }
    if (itor != NULL) {
        sprintf(error, "List *has more elements at the begining then numbers");                                
        return false; 
    } 
    return true; 
}

bool
TestListInit(List *list, ListType type)
{
    if (!ListInit(list, type)) {
        sprintf(error, "ListInit failed");
        return false;
    }
    if (!ListEmpty(list)) {
        sprintf(error, "List *is not empty after init");
        return false;
    }
    return true;
}


bool
TestListPrepend(List *list)
{
    int i;
    for (i = length-1; i >= 0; i--) {
        if (!ListPrepend(list, numbers[i])) {
            sprintf(error, "Prepend failed!!");
            return false;
        }
    }
    return VerifyListConsistency(list);
}

void
PrintError(const char *op) 
{
    printf("%s failed!! Error: %s\n", op, error);
}

void TestList()
{
    length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    List l;
    if (!TestListInit(&l, LT_DLS))
        PrintError("TestListInit");
    else if (!TestListPrepend(&l ))
        PrintError("TestListPrepend");
    
    //ListItor head = ListHead(l);

#ifdef TOREMOVE
    if (!TestINSERT_BEFORE_LIST(numbers, length, error))
        printf("TestINSERT_BEFORE_LIST failed!! Error: %s\n", error);
    else if (!TestINSERT_AFTER_LIST(numbers, length, error))
        printf("TestINSERT_AFTER_LIST failed!! Error: %s\n", error);
    else if (!TestDELETE_LIST(numbers, length, error))
        printf("TestDELETE_LIST failed!! Error: %s\n", error);
    else if (!TestSEARCH_LIST(numbers, length, error))
        printf("TestSEARCH_LIST failed!! Error: %s\n", error);
#endif
    else
        printf("TestList passed!!\n");
}
