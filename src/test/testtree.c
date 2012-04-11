#include "tree.h"
#include "utility.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const int TestMinArrayLen = 80;
static const int TestMaxArrayLen = 120;
static const int TestMinValue = -58902;
static const int TestMaxValue = 58292;

static int length;
static int *numbers;
char error[1024];

bool VerifyBTINullLeftChild(BinaryTreeItor itor, const char *target)
{
    if (!BTINull(BTILeftChild(itor))) {
        sprintf(error, "The left child of %s is not null", target);
        return false;
    }
    return true;
}

bool VerifyBTINullRightChild(BinaryTreeItor itor, const char *target)
{
    if (!BTINull(BTIRightChild(itor))) {
        sprintf(error, "The right child of %s is not null", target);
        return false;
    }
    return true;
}

bool VerifyBTINullChildren(BinaryTreeItor itor, const char *target)
{
    return VerifyBTINullLeftChild(itor, target) && VerifyBTINullRightChild(itor, target);
}

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
    BinaryTreeItor itor = BTRoot(btp);
    if (!BTINull(itor)) {
        sprintf(error, "The root of an empty tree is not null");
        return false;
    }
    return true;
}

bool TestBTAddRoot(BinaryTree *btp)
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    if (!BTAddRoot(btp, numbers[0])) {
        sprintf(error, "BTAddRoot failed");
        return false;
    }
    BinaryTreeItor itor = BTRoot(btp);
    if (BTINull(itor)) {
        sprintf(error, "The root of a nonempty tree is null");
        return false;
    }
    if (BTIValue(itor) != numbers[0]) {
        sprintf(error, "The value of root is not correct after BTAddRoot");
        return false;
    }
    if (VerifyBTINullChildren(itor, "root that is first time added")) {
        return false;
    }
    return true;
}

bool TestBTDeleteRoot(BinaryTree *btp)
{
    if (BTEmpty(btp)) {
        sprintf(error, "This test requires tree not to be empty");
        return false;
    }

    BinaryTreeItor itor = BTRoot(btp);
    if (!BTIDelete(itor)) {
        sprintf(error, "BTIDelete root failed");
        return false;
    }
    if (!BTEmpty(btp)) {
        sprintf(error, "Tree is not empty after root deleted");
        return false;
    }
    if (!BTINull(BTRoot(btp))) {
        sprintf(error, "Root is not null after being deleted");
        return false;
    }
    return true;
}

bool TestBTRoot(BinaryTree *btp)
{
    return TestBTAddRoot(btp) && TestBTDeleteRoot(btp);
}

bool TestBTLeftChild(BinaryTree *btp) 
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    BTAddRoot(btp, numbers[0]); // Already tested
    BinaryTreeItor itor = BTRoot(btp);

    // All insert to the left
    int i;
    for (i = 1; i < length; i++) {
        if (!BTIAddLeftChild(itor, numbers[i])) {
            sprintf(error, "BTIAddLeftChild failed");
            return false;
        }
        if (BTIAddLeftChild(itor, -1)) {
            sprintf(error, "Itor already has a left child, but we can still add a left child to it");
            return false;
        }
        if (VerifyBTINullRightChild(itor, "itor that just has a new left child")) {
            return false;
        }
        BinaryTreeItor left_itor = BTILeftChild(left_itor);
        if (BTINull(left_itor)) {
            sprintf(error, "Left child is still null after adding a left child");
            return false;
        }
        if (VerifyBTINullChildren(left_itor, "newly added left child")) {
            return false;
        }
        if (!BTIEqual(BTIParent(left_itor), itor)) {
            sprintf(error, "The parent of the newly added left child is wrong");
            return false;
        }
        if (BTIValue(left_itor) != numbers[i]) {
            sprintf(error, "The new left child's value is wrong");
            return false;
        }
        itor = left_itor;
    }

    // Verify the inserted data
    itor = BTRoot(btp);
    if (BTIValue(itor) != numbers[0]) {
        sprintf(error, "Root's value is changed after inserting left chilren");
        return false;
    }
    for (i = 1; i < length; i++) {
        itor = BTILeftChild(itor);
        if (BTINull(itor)) {
            sprintf(error, "A left child that should exist is gone [%d]", i);
            return false;
        }
        if (BTIValue(itor)) {
            sprintf(error, "A left child's value is wrong [%d]", i);
            return false;
        }
    }
    if (!BTINull(itor)) {
        sprintf(error, "Tree has more nodes than numbers");
        return false;
    }

    // Delete all the nodes we just added
    int deleted = 0;
    itor = BTRoot(btp);
    while (!BTINull(BTILeftChild(itor)))
        itor = BTILeftChild(itor);
    while (!BTIEqual(itor, BTRoot(btp))) {
        BinaryTreeItor parent = BTIParent(itor);
        if (!BTIDelete(itor)) {
            sprintf(error, "BTIDelete failed");
            return false;
        }
        if (VerifyBTINullChildren(parent, "node whose left child was just deleted"))
            return false;
        itor = parent;
        deleted++;
    }
    if (deleted != length - 1) {
        sprintf(error, "The number deleted [%d] is different than length-1 [%d]", deleted, length-1);
        return false;
    }

    BTIDelete(itor); // delete root already tested
    return true;
}

bool TestBTRightChild(BinaryTree *btp) 
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    BTAddRoot(btp, numbers[0]); // Already tested
    BinaryTreeItor itor = BTRoot(btp);

    // All insert to the right
    int i;
    for (i = 1; i < length; i++) {
        if (!BTIAddRightChild(itor, numbers[i])) {
            sprintf(error, "BTIAddRightChild failed");
            return false;
        }
        if (BTIAddRightChild(itor, -1)) {
            sprintf(error, "Itor already has a right child, but we can still add a right child to it");
            return false;
        }
        if (VerifyBTINullLeftChild(itor, "itor that just has a new right child")) {
            return false;
        }
        BinaryTreeItor right_itor = BTIRightChild(right_itor);
        if (BTINull(right_itor)) {
            sprintf(error, "Right child is still null after adding a right child");
            return false;
        }
        if (VerifyBTINullChildren(right_itor, "newly added right child")) {
            return false;
        }
        if (!BTIEqual(BTIParent(right_itor), itor)) {
            sprintf(error, "The parent of the newly added right child is wrong");
            return false;
        }
        if (BTIValue(right_itor) != numbers[i]) {
            sprintf(error, "The new right child's value is wrong");
            return false;
        }
        itor = right_itor;
    }

    // Verify the inserted data
    itor = BTRoot(btp);
    if (BTIValue(itor) != numbers[0]) {
        sprintf(error, "Root's value is changed after inserting right chilren");
        return false;
    }
    for (i = 1; i < length; i++) {
        itor = BTIRightChild(itor);
        if (BTINull(itor)) {
            sprintf(error, "A right child that should exist is gone [%d]", i);
            return false;
        }
        if (BTIValue(itor)) {
            sprintf(error, "A right child's value is wrong [%d]", i);
            return false;
        }
    }
    if (!BTINull(itor)) {
        sprintf(error, "Tree has more nodes than numbers");
        return false;
    }

    // Delete all the nodes we just added
    int deleted = 0;
    itor = BTRoot(btp);
    while (!BTINull(BTIRightChild(itor)))
        itor = BTIRightChild(itor);
    while (!BTIEqual(itor, BTRoot(btp))) {
        BinaryTreeItor parent = BTIParent(itor);
        if (!BTIDelete(itor)) {
            sprintf(error, "BTIDelete failed");
            return false;
        }
        if (VerifyBTINullChildren(parent, "node whose right child was just deleted"))
            return false;
        itor = parent;
        deleted++;
    }
    if (deleted != length - 1) {
        sprintf(error, "The number deleted [%d] is different than length-1 [%d]", deleted, length-1);
        return false;
    }

    BTIDelete(itor); // delete root already tested
    return true;
}

bool TestBTBothChildren(BinaryTree *btp)
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    BTAddRoot(btp, numbers[0]); // Already tested

    BinaryTreeItor *itor_queue = alloca(sizeof(BinaryTreeItor) * length);
    int qhead, qtail;
    qhead = qtail = 0; // When qhead equals to qtail the queue is empty

    itor_queue[qtail++] = BTRoot(btp);

    int i;
    // Insert from up to down, from left to right,
    // to make it a complete binary tree.
    for (i = 1; i < length; i++) {
        int value = numbers[i];
        bool added = false;
        while (!added) {
            BinaryTreeItor itor = itor_queue[qhead];
            if (BTINull(BTILeftChild(itor))) {
                if (!BTIAddLeftChild(itor, value)) {
                    sprintf(error, "BTIAddLeftChild failed [%d]", i);
                    return false;
                }
                itor_queue[qtail++] = itor; // Enqueue
                added = true;
            } else if (BTINull(BTIRightChild(itor))) {
                if (!BTIAddRightChild(itor, value)) {
                    sprintf(error, "BTIAddRightChild failed [%d]", i);
                    return false;
                }
                itor_queue[qtail++] = itor; // Enqueue
                added = true;
            } else {
                qhead++; // Dequeue
            }
       }
    }

    // Verify the data that we just added
    qhead = qtail = 0;
    itor_queue[qtail++] = BTRoot(btp);
    int count = 0;
    while (qhead != qtail) {
        BinaryTreeItor itor = itor_queue[qhead++];
        if (!BTINull(BTILeftChild(itor)))
            itor_queue[qtail++] = BTILeftChild(itor);
        if (!BTINull(BTIRightChild(itor)))
            itor_queue[qtail++] = BTIRightChild(itor);
        if (BTIValue(itor) != numbers[count]) {
            sprintf(error, "data is wrong when walking in layer order [%d]", count);
            return false;
        }
        count++;
    }
    if (count != length) {
        sprintf(error, "The number of tree nodes [%d] is smaller then length [%d]", count, length);
        return false;
    }

    // Delete all the nodes we just added from the last layer upwards
    int deleted = 0;
    BinaryTreeItor itor = BTRoot(btp);
    while (!BTINull(BTIRightChild(itor)))
        itor = BTIRightChild(itor);
    while (!BTINull(itor)) {
        BinaryTreeItor parent = BTIParent(itor);
        if (!BTIDelete(itor)) {
            sprintf(error, "BTIDelete failed");
            return false;
        }
        if (VerifyBTINullChildren(parent, "node whose right child was just deleted"))
            return false;
        itor = parent;
        deleted++;
    }
    if (deleted != log2(length) + 1) { // the number of layers
        sprintf(error, "The number of deleted layers [%d] is different than log2(length) [%d]", deleted, (int)log2(length));
        return false;
    }

    if (!BTEmpty(btp)) {
        sprintf(error, "Tree is not empty after deleting all nodes");
        return false;
    }
    return true;
}

bool TestBTDeleteAll(BinaryTree *btp) 
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

   
    BinaryTreeItor *itor_queue = alloca(sizeof(BinaryTreeItor) * length);
    int qhead, qtail;
    qhead = qtail = 0; // When qhead equals to qtail the queue is empty

    int i;
    // Insert from up to down, from left to right,
    // to make it a complete binary tree.
    for (i = 1; i < length; i++) {
        int value = numbers[i];
        bool added = false;
        while (!added) {
            BinaryTreeItor itor = itor_queue[qhead];
            if (BTINull(BTILeftChild(itor))) {
                BTIAddLeftChild(itor, value); // Already tested
                itor_queue[qtail++] = itor; // Enqueue
                added = true;
            } else if (BTINull(BTIRightChild(itor))) {
                BTIAddRightChild(itor, value); // Already tested
                itor_queue[qtail++] = itor; // Enqueue
                added = true;
            } else {
                qhead++; // Dequeue
            }
       }
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
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    BinaryTreeItor itor = BTILeftChild(itor);
    // All insert to the left
    int i;
    for (i = 1; i < length; i++) {
        BTIAddLeftChild(itor, -1); // Already tested
        itor = BTILeftChild(itor);
    }

    itor = BTRoot(btp);
    i = 0;
    while (!BTINull(itor)) {
        if (BTISetValue(itor, numbers[i])) {
            sprintf(error, "BTISetValue failed [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }

    // Verify the data
    itor = BTRoot(btp);
    i = 0;
    while (!BTINull(itor)) {
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "Data is wrong after SetVaue [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }
    if (i != length) {
        sprintf(error, "Node number is smaller [%d] than length [%d] after SetValue", i, length);
        return false;
    }

    BTDeleteAll(btp);
    return true;
}

bool TestBTMove(BinaryTree *btp)
{
    if (!BTEmpty(btp)) {
        sprintf(error, "This test requires tree to be empty");
        return false;
    }

    BTAddRoot(btp, -1);
    BinaryTreeItor root = BTRoot(btp);
    BTIAddLeftChild(root, -1);
    BTIAddRightChild(root, -1);
    BinaryTreeItor left = BTILeftChild(root);
    BinaryTreeItor right = BTIRightChild(root);

    BTIAddLeftChild(left, numbers[0]);
    BinaryTreeItor moved_itor = BTILeftChild(left);
    // All insert to the left
    int i;
    for (i = 1; i < length; i++) {
        BTIAddLeftChild(moved_itor, -1); // Already tested
        moved_itor = BTILeftChild(moved_itor);
    }
    // Move from left's left to right's right
    if (!BTIMoveAsRightChild(moved_itor, right)) {
        sprintf(error, "BTIMoveAsRightChild failed");
        return false;
    }
    if (!VerifyBTINullChildren(left, "Left node whose left child is just moved"))
        return false;

    // Verify moved data
    moved_itor = BTIRightChild(right);
    BinaryTreeItor itor = moved_itor;
    i = 0;
    while (!BTINull(itor)) {
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "Data is wrong after moving [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }
    if (i != length) {
        sprintf(error, "Node number is smaller [%d] than length [%d] after Moving", i, length);
        return false;
    }

    moved_itor = BTIRightChild(right);
    // Move from right's right to right's left
    if (!BTIMoveAsLeftChild(moved_itor, right)) {
        sprintf(error, "BTIMoveAsRightChild failed");
        return false;
    }
    if (!VerifyBTINullRightChild(right, "Right node whose right child is just moved"))
        return false;

    // Verify moved data
    moved_itor = BTILeftChild(right);
    itor = moved_itor;
    i = 0;
    while (!BTINull(itor)) {
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "Data is wrong after moving [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }
    if (i != length) {
        sprintf(error, "Node number is smaller [%d] than length [%d] after Moving", i, length);
        return false;
    }
    
    return true;
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
    else if (!TestBTRoot(&bt)) 
        printf("TestBTRoot failed. Error: %s\n", error);
    else if (!TestBTLeftChild(&bt)) 
        printf("TestBTLeftChild failed. Error: %s\n", error);
    else if (!TestBTRightChild(&bt)) 
        printf("TestBTRightChild failed. Error: %s\n", error);
    else if (!TestBTBothChildren(&bt)) 
        printf("TestBTBothChildren failed. Error: %s\n", error);
    else if (!TestBTDeleteAll(&bt)) 
        printf("TestBTDeleteAll failed. Error: %s\n", error);
    else if (!TestBTSetValue(&bt)) 
        printf("TestBTSetValue failed. Error: %s\n", error);
    else if (!TestBTMove(&bt)) 
        printf("TestBTMove failed. Error: %s\n", error);
    else if (!TestBTFree(&bt)) 
        printf("TestBTFree failed. Error: %s\n", error);
    else
        printf("TestTree successful!!");
}
