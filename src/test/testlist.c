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

static int length;
static int *numbers;
char error[1024];

bool 
VerifyListConsistency(List *list)
{                                                       
    int i;    
    ListItor itor = ListHead(list);
    for (i = 0; i < length; i++) {                        
        if (ListValue(itor) != numbers[i]) {                   
            sprintf(error, "Encounter wrong data [i:%d/%d] when walking from the head to the tail", i, length-1); 
            return false;
        }                                               
        itor = ListItorNext(itor);
    }                                                   
    if (!ListItorNull(itor)) {                                     
        sprintf(error, "List has more elements at the end then numbers");                                
        return false;                                   
    }                                                   
    itor = ListTail(list);
    for (i = length-1; i >= 0; i--) {                      
        if (ListValue(itor) != numbers[i]) {                   
            sprintf(error, "Encounter wrong data [i:%d/%d] when walking from the tail to the head", i, length-1); 
            return false; 
        }
        itor = ListItorPrev(itor);
    }
    if (!ListItorNull(itor)) {
        sprintf(error, "List has more elements at the begining then numbers");                                
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
TestListDeleteAll(List *list)
{
    if (!ListDeleteAll(list)) {
        sprintf(error, "ListDeleteAll failed!!");
        return false;
    }
    if (!ListEmpty(list)) {
        sprintf(error, "List is not empty after delete all");
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
    if (!VerifyListConsistency(list)) {
        strcat(error, " - after ListPrepend");
        return false;
    }
    return true;
}

bool
TestListAppend(List *list)
{
    int i;
    for (i = 0; i < length; i++) {
        if (!ListAppend(list, numbers[i])) {
            sprintf(error, "Prepend failed!!");
            return false;
        }
    }
    if (!VerifyListConsistency(list)) {
        sprintf(error, "List is not consistent with numbers after ListAppend");
        return false;
    }
    return true;
}

bool TestListInsertBefore(List *list)
{
    if (!ListEmpty(list)) {
        sprintf(error, "This test requires list to be empty");
        return false;
    }

    int i;
    int k = 5;
    for (i = length-k; i < length; i++)
        ListAppend(list, numbers[i]); // Already tested
    for (i = k - 1; i >= 0; i--)
        ListPrepend(list, numbers[i]); // Already tested

    ListItor itor = ListTail(list);
    for (i = 0; i < k-1; i++)
        itor = ListItorPrev(itor);
    for (i = k; i < length - k; i++)
        if (!ListInsertBefore(itor, numbers[i])) {
            sprintf(error, "ListInsertBefore failed");
            return false;
        }
    if (!VerifyListConsistency(list)) {
        sprintf(error, "List is not consistent with numbers after ListInsertBefore");
        return false;
    }
    return true;
}

bool TestListInsertAfter(List *list)
{
    ListDeleteAll(list); // Already tested
    if (!ListEmpty(list)) {
        sprintf(error, "This test requires list to be empty");
        return false;
    }

    int i;
    int k = 5;
    for (i = length-k; i < length; i++)
        ListAppend(list, numbers[i]); // Already tested
    for (i = k - 1; i >= 0; i--)
        ListPrepend(list, numbers[i]); // Already tested

    ListItor itor = ListHead(list);
    for (i = 0; i < k-1; i++)
        itor = ListItorNext(itor);
    for (i = length - k - 1; i >= k; i--)
        if (!ListInsertAfter(itor, numbers[i])) {
            sprintf(error, "ListInsertAfter failed");
            return false;
        }
    if (!VerifyListConsistency(list)) {
        sprintf(error, "List is not consistent with numbers after ListInsertAfter");
        return false;
    }
    return true;
}

bool
TestListDelete(List *list)
{
    /* Precondition */
    if (!VerifyListConsistency(list)){
        sprintf(error, "This test requires list to be consist with numbers!!");
        return false;
    }
    
    ListItor itor;

    /* Delete last element */
    if (!ListDelete(ListTail(list))) {
        sprintf(error, "Delete the first element failed");
        return false;
    }
    itor = ListTail(list);
    if (ListValue(itor) != numbers[length-2]
            || !ListItorNull(ListItorNext(itor))
            || ListValue(ListItorPrev(itor)) != numbers[length-3]) {
        sprintf(error, "Data is not consistent after deleting the first element");
        return false;
    }

    /* Delete the k'th element */
    int k = 5;
    int i;
    itor = ListHead(list);
    for (i = 0; i < k; i++) 
        itor = ListItorNext(itor);

    if (ListValue(itor) != numbers[k] 
            || ListValue(ListItorPrev(itor)) != numbers[k-1]
            || ListValue(ListItorNext(itor)) != numbers[k+1]) {
        sprintf(error, "Test code's assumption is wrong!!");
        return false;
    }

    if (!ListDelete(itor)) {
        sprintf(error, "ListDelete failed!!");
        return false;
    }
    itor = ListHead(list);
    for (i = 0; i < k; i++)
        itor = ListItorNext(itor);
    if (ListValue(itor) != numbers[k+1]
            || ListValue(ListItorPrev(itor)) != numbers[k-1]) {
        sprintf(error, "Data is not consistent after deleting the %dth element", k);
        return false;
    }

    /* Delete the first */
    if (!ListDelete(ListHead(list))) {
        sprintf(error, "Delete the first element failed");
        return false;
    }
    itor = ListHead(list);
    if (ListValue(itor) != numbers[1] 
            || !ListItorNull(ListItorPrev(itor))
            || ListValue(ListItorNext(itor)) != numbers[2]) {
        sprintf(error, "Data is not consistent after deleting the first element");
        return false;
    }

    /* Delete all the rest */
    for (i = 0; i < 10; i++)
        if (!ListDelete(ListItorPrev(ListTail(list)))) {
            sprintf(error, "Delete the last 2nd element failed when cleaning up");
            return false;
        }
    for (; i < 20; i++)
        if (!ListDelete(ListItorNext(ListHead(list)))) {
            sprintf(error, "Delete the 2nd element failed when cleaning up");
            return false;
        }
    for (; i < length - 40; i++)
        if (!ListDelete(ListTail(list))) {
            sprintf(error, "Delete the last element failed when cleaning up");
            return false;
        }
    for (; i < length - 3; i++)
        if (!ListDelete(ListHead(list))) {
            sprintf(error, "Delete the first element failed when cleaning up");
            return false;
        }

    if (!ListEmpty(list)) {
        sprintf(error, "List is not empty after delete all its elements");
        return false;
    }

    return true;
}

bool
TestListSearch(List *list)
{
    ListDeleteAll(list); // Already tested
    int i;
    for (i = 0; i < length; i++)
        ListAppend(list, numbers[i]);
    for (i = length - 1; i >= 0; i--)
        if (ListItorNull(ListSearch(list, numbers[i])) ||
                ListValue(ListSearch(list, numbers[i])) != numbers[i]) {
            sprintf(error, "Cannot find existing element [%d]", i);
            return false;
        }
    if (!ListItorNull(ListSearch(list, TestMaxValue + 1))) {
        sprintf(error, "Found nonexisting element - bigger than max");
        return false;
    }
    if (!ListItorNull(ListSearch(list, TestMinValue - 1))) {
        sprintf(error, "Found nonexisting element - smaller than min");
        return false;
    }
    return true;
}

bool
TestListSetValue(List *list)
{
    ListDeleteAll(list); // Already tested
    int i;
    for (i = 0; i < length; i++)
        ListAppend(list, 0);
    ListItor itor = ListHead(list);
    for (i = 0; i < length; i++) {
        if (!ListSetValue(itor, numbers[i])) {
            sprintf(error, "ListSetValue failed [%d]", i);
            return false;
        }
        itor = ListItorNext(itor);
    }
    if (!VerifyListConsistency(list)) {
        sprintf(error, "Data is wrong after set value - %s", error);
        return false;
    }
    return true;
}

void
PrintError(const char *op) 
{
    printf("%s failed!! Error: %s\n", op, error);
}

#define TEST_LIST_TYPE(TYPE)  TestListType(TYPE, #TYPE)

void TestListType(ListType type, const char* typename)
{
    printf("TestList type %s begins\n", typename);
    List l;
    if (!TestListInit(&l, type)) {
        PrintError("TestListInit");
        return;
    }
    if (!TestListPrepend(&l)) {
        PrintError("TestListPrepend");
        return;
    }
    if (!TestListDeleteAll(&l)) {
        PrintError("TestListDeleteAll");
        return;
    }
    if (!TestListAppend(&l)) {
        PrintError("TestListAppend");
        return;
    }
    if (!TestListDelete(&l)) {
        PrintError("TestListDelete");
        return;
    }
    if (!TestListInsertBefore(&l)) {
        PrintError("TestListInsertBefore");
        return;
    }
    if (!TestListInsertAfter(&l)) {
        PrintError("TestListInsertAfter");
        return;
    }
    if (!TestListSearch(&l)) {
        PrintError("TestListInsertSearch");
        return;
    }
    if (!TestListSetValue(&l)) {
        PrintError("TestListSetValue");
        return;
    }
    else
        printf("TestList type %s passed!!\n\n", typename);
}

void TestList()
{
    length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    TEST_LIST_TYPE(LT_DLS);
    TEST_LIST_TYPE(LT_DLNS);
    TEST_LIST_TYPE(LT_DLA);
}
