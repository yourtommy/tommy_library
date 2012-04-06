#include "testset.h"
#include "utility.h"
#include "list.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58023;
static const int TestMaxValue = 59321;

bool TestListInsertBefore(int *numbers, int len, char error[])
{
    struct List *head;
    head = malloc(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    for (i = 1; i < len; i++) {
        struct List *node = malloc(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!ListInsertBefore(head, node)) {
            sprintf(error, "InsertBefore failed!!");
            return false;
        }
        head = node;
    }
    struct List *p = head;
    for (i = len - 1; i >= 0; i--) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertBefore List(%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
    return true;
}

bool TestListInsertAfter(int *numbers, int len, char error[])
{
    struct List *head;
    head = malloc(sizeof(struct List));
    head->prev = head->next = NULL;
    head->value = numbers[0];
    int i;
    struct List *p = head;
    for (i = 1; i < len; i++) {
        struct List *node = malloc(sizeof(struct List));
        node->prev = node->next = NULL;
        node->value = numbers[i];
        if (!ListInsertAfter(p, node)) {
            sprintf(error, "InsertBefore failed!!");
            return false;
        }
        p = node;
    }
    p = head;
    for (i = 0; i < len; i++) {
        if (p->value != numbers[i]) {
            sprintf(error, "Data inconsitent after InsertAfter List(%d) != [%d](%d)", p->value, i, numbers[i]);
            return false;
        }
        p = p->next;
    }
    return true;
}

//void TestListDelete(int *numbers, int len, char error[])
//{
    /* TODO: not implemented */
//}

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
    else
        printf("TestList passed!!\n");
}
