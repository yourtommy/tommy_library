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

bool TestListInsertBefore(int *numbers, int len, char error[])
{
    // Insert all elements before the first one.
    struct List *head;
    head = alloca(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    for (i = 1; i < len; i++) {
        struct List *node = alloca(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!ListInsertBefore(head, node)) {
            sprintf(error, "InsertBefore the 1st element failed!!");
            return false;
        }
        head = node;
    }
    struct List *p = head;
    for (i = len - 1; i >= 0; i--) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertBefore the 1st element (%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
    // Walk through the list from tail to head.
    p = head;
    while (p->next != NULL)
        p = p->next;
    for (i = 0; i < len; i++) {
        if (p->value != numbers[i]) {
            sprintf(error, "(Reverse order) Data inconsitent after InsertBefore the 1st element (%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->prev;
    }

    // Insert all elements before the last one.
    p = alloca(sizeof(struct List));
    p->prev = head->next = NULL;
    p->value = numbers[0];
    head = alloca(sizeof(struct List));
    head->value = numbers[len-1];
    ListInsertBefore(p, head);
    for (i = len-2; i >= 1; i--) {
        struct List *node = alloca(sizeof(struct List));
        node->value = numbers[i];
        if (!ListInsertBefore(p, node)) {
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

    return true;
}

bool TestListInsertAfter(int *numbers, int len, char error[])
{
    // Insert after the last element
    struct List *head;
    head = alloca(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    struct List *p = head;
    for (i = 1; i < len; i++) {
        struct List *node = alloca(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!ListInsertAfter(p, node)) {
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
    head = alloca(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    p = alloca(sizeof(struct List));
    p->value = numbers[len-1];
    ListInsertAfter(head, p);

    for (i = len - 2; i >= 1; i--) {
        struct List *node = alloca(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!ListInsertAfter(head, node)) {
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

bool TestListDelete(int *numbers, int len, char error[])
{
    // insert after the last element. It's already been tested.
    struct List *head;
    head = alloca(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    struct List *p = head;
    for (i = 1; i < len; i++) {
        struct List *node = alloca(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        ListInsertAfter(p, node); 
        p = node;
    }

    // Delete the first element.
    p = head->next;
    if (!ListDelete(head)) {
        sprintf(error, "Delete the first element failed");
        return false;
    }
    if (p->prev != NULL) {
        sprintf(error, "The second element doesn't become head after the first element deleted");
        return false;
    }
    head = p;
    struct List *p1 = head->next;
    struct List *p2 = head->next->next;
    struct List *p3 = head->next->next->next;
    if (!ListDelete(p2)) {
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
    if (!ListDelete(p)) {
        sprintf(error, "Delete the last element failed");
        return false;
    }
    if (p1->next != NULL) {
        sprintf(error, "The last 2nd element doesn't become tail after the last element deleted");
        return false;
    }

    return true;
}

bool TestListSearch(int *numbers, int len, char error[]) {
    // insert after the last element. It's already been tested.
    struct List *head;
    head = alloca(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    struct List *p = head;
    for (i = 1; i < len; i++) {
        struct List *node = alloca(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        ListInsertAfter(p, node); 
        p = node;
    }

    // Search the 1st element.
    p = ListSearch(head, numbers[0]);
    if (p != head) { // The search result must be head because it's the first element of that value.
        sprintf(error, "Searching 1st element failed");
        return false;
    }
    // Search the 5th element.
    p = ListSearch(head, numbers[4]);
    if (p == NULL || p->value != numbers[4]) {
        sprintf(error, "Searching 5th element failed");
        return false;
    }
    // Search the last element.
    p = ListSearch(head, numbers[len-1]);
    if (p == NULL || p->value != numbers[len-1]) {
        sprintf(error, "Searching the last element failed.");
        return false;
    }

    return true;
}

void TestList()
{
    unsigned length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int *numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    char error[1024] = "";
    if (!TestListInsertBefore(numbers, length, error))
        printf("TestListInsertBefore failed!! Error: %s\n", error);
    else if (!TestListInsertAfter(numbers, length, error))
        printf("TestListInsertAfter failed!! Error: %s\n", error);
    else if (!TestListDelete(numbers, length, error))
        printf("TestListDelete failed!! Error: %s\n", error);
    else if (!TestListSearch(numbers, length, error))
        printf("TestListSearch failed!! Error: %s\n", error);
    else
        printf("TestList passed!!\n");
}
