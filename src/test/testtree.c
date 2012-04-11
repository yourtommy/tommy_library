#include "tree.h"
#include "utility.h"
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

bool TestBTInit(BinaryTree *btp)
{
    if (!BTInit(btp)) {
        sprintf(error, "BTInit falied");
        return false;
    }
    if (!BTEmpty(btp)) {
        sprintf(error, "Tree is not empty after init");
        return false;
    }
    return true;
}

bool TestBTInsert(BinaryTree *btp) 
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    // TODO: 
    return false;
}

bool TestBTDelete(BinaryTree *btp) 
{
    // TODO: 
    (void)(btp);
    return false;
}

bool TestBTDeleteAll(BinaryTree *btp) 
{
    if (BTEmpty(btp)) {
        sprintf(error, "This test requires tree not to be empty");
        return false;
    }
    if (!BTDeleteAll(btp)) {
        sprintf(error, "BTDeleteAlll failed");
        return false;
    }
    if (!BTEmpty(btp)) {
        sprintf(error, "Tree is not empty after deleting all");
        return false;
    }
    return true;
}

bool TestBTSetValue(BinaryTree *btp) 
{
    // TODO
    (void)(btp);
    return false;
}

bool TestBTFree(BinaryTree *btp) 
{
    if (!BTFree(btp)) {
        sprintf(error, "BTFree failed");
        return false;
    }
    return true;
}

void TestTree()
{
    length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    BinaryTree bt;
    if (!TestBTInit(&bt)) 
        printf("TestBTInit failed. Error: %s\n", error);
    else if (!TestBTInsert(&bt)) 
        printf("TestBTInsert failed. Error: %s\n", error);
    else if (!TestBTDelete(&bt)) 
        printf("TestBTInsert failed. Error: %s\n", error);
    else if (!TestBTDeleteAll(&bt)) 
        printf("TestBTInsert failed. Error: %s\n", error);
    else if (!TestBTSetValue(&bt)) 
        printf("TestBTInsert failed. Error: %s\n", error);
    else if (!TestBTFree(&bt)) 
        printf("TestBTFree failed. Error: %s\n", error);
}
