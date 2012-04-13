#include "tree.h"
#include "utilitytest.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static const int TestMinArrayLen = 1;
static const int TestMaxArrayLen = 1200;
static const int TestMinValue = -58902;
static const int TestMaxValue = 58292;

static int length;
static int *numbers;
char error[1024];

// for Binary Sort Tree's test
static int *walked_array;
static int walked_count;

bool WalkBST(int value);

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
    if (!VerifyBTINullChildren(itor, "root that is first time added")) {
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
        if (!VerifyBTINullRightChild(itor, "itor that just has a new left child")) {
            return false;
        }
        BinaryTreeItor left_itor = BTILeftChild(itor);
        if (BTINull(left_itor)) {
            sprintf(error, "Left child is still null after adding a left child");
            return false;
        }
        if (!VerifyBTINullChildren(left_itor, "newly added left child")) {
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
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "A left child's value is wrong [%d]", i);
            return false;
        }
    }
    itor = BTILeftChild(itor);
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
        if (!VerifyBTINullChildren(parent, "node whose left child was just deleted"))
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
        if (!VerifyBTINullLeftChild(itor, "itor that just has a new right child")) {
            return false;
        }
        BinaryTreeItor right_itor = BTIRightChild(itor);
        if (BTINull(right_itor)) {
            sprintf(error, "Right child is still null after adding a right child");
            return false;
        }
        if (!VerifyBTINullChildren(right_itor, "newly added right child")) {
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
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "A right child's value is wrong [%d]", i);
            return false;
        }
    }
    itor = BTIRightChild(itor);
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
        if (!VerifyBTINullChildren(parent, "node whose right child was just deleted"))
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
                itor_queue[qtail++] = BTILeftChild(itor); // Enqueue
                added = true;
            } else if (BTINull(BTIRightChild(itor))) {
                if (!BTIAddRightChild(itor, value)) {
                    sprintf(error, "BTIAddRightChild failed [%d]", i);
                    return false;
                }
                itor_queue[qtail++] = BTIRightChild(itor); // Enqueue
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
    while (!BTINull(BTILeftChild(itor)))
        itor = BTILeftChild(itor);
    while (!BTINull(itor)) {
        BinaryTreeItor parent = BTIParent(itor);
        if (!BTIDelete(itor)) {
            sprintf(error, "BTIDelete failed");
            return false;
        }
        if (!BTINull(itor) && !VerifyBTINullLeftChild(parent, "node whose left child was just deleted"))
            return false;
        itor = parent;
        deleted++;
    }
    if (deleted != (int)log2(length) + 1) { // the number of layers
        sprintf(error, "The number of deleted layers [%d] is different than log2(length[%d])+1 [%d]", deleted, length, (int)log2(length)+1);
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
                itor_queue[qtail++] = BTILeftChild(itor); // Enqueue
                added = true;
            } else if (BTINull(BTIRightChild(itor))) {
                BTIAddRightChild(itor, value); // Already tested
                itor_queue[qtail++] = BTIRightChild(itor); // Enqueue
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

    BTAddRoot(btp, -1);
    BinaryTreeItor itor = BTRoot(btp);
    // All insert to the left
    int i;
    for (i = 1; i < length; i++) {
        BTIAddLeftChild(itor, -1); // Already tested
        itor = BTILeftChild(itor);
    }

    itor = BTRoot(btp);
    i = 0;
    while (!BTINull(itor)) {
        if (!BTISetValue(itor, numbers[i])) {
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
    BinaryTreeItor itor = moved_itor;
    // All insert to the left
    int i;
    for (i = 1; i < length; i++) {
        BTIAddLeftChild(itor, numbers[i]); // Already tested
        itor = BTILeftChild(itor);
    }
    // Move from left's left to right's right
    if (!BTIMoveAsRightChild(moved_itor, right)) {
        sprintf(error, "BTIMoveAsRightChild failed");
        return false;
    }
    if (!VerifyBTINullChildren(left, "Left node whose left child is just moved from left's left to right's right"))
        return false;

    // Verify moved data
    moved_itor = BTIRightChild(right);
    itor = moved_itor;
    i = 0;
    while (!BTINull(itor)) {
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "Data is wrong after moving from left's left to right's right [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }
    if (i != length) {
        sprintf(error, "Node number is smaller [%d] than length [%d] after moving from left's left to right's right", i, length);
        return false;
    }

    moved_itor = BTIRightChild(right);
    // Move from right's right to right's left
    if (!BTIMoveAsLeftChild(moved_itor, right)) {
        sprintf(error, "BTIMoveAsRightChild failed");
        return false;
    }
    if (!VerifyBTINullRightChild(right, "Right node whose right child is just moved from right's left to right's left"))
        return false;

    // Verify moved data
    moved_itor = BTILeftChild(right);
    itor = moved_itor;
    i = 0;
    while (!BTINull(itor)) {
        if (BTIValue(itor) != numbers[i]) {
            sprintf(error, "Data is wrong after moving from left's right to right's left [%d]", i);
            return false;
        }
        i++;
        itor = BTILeftChild(itor);
    }
    if (i != length) {
        sprintf(error, "Node number is smaller [%d] than length [%d] after moving from right's left to right's left", i, length);
        return false;
    }
    
    BTDeleteAll(btp);
    return true;
}

bool BuildPreorderTree(BinaryTreeItor itor, int begin, int end)
{
    if (begin >= end)
        return false;
    BTISetValue(itor, numbers[begin]);
    if (begin == end - 1)
        return true;

    int mid = (begin+end+1) / 2; // ceiling(mid(begin+1, end-1))
    if (begin < mid) {
        if (!BTIAddLeftChild(itor, -1))
            return false;
        if (!BuildPreorderTree(BTILeftChild(itor), begin+1, mid+1))
            return false;
    }
    if (mid+1 < end) {
        if (!BTIAddRightChild(itor, -1))
            return false;
        if (!BuildPreorderTree(BTIRightChild(itor), mid+1, end))
            return false;
    }
    return true;
}

bool BuildInorderTree(BinaryTreeItor itor, int begin, int end)
{
    if (begin >= end)
        return false;
    int mid = (begin+end) / 2; // ceiling(mid(begin, end-1))
    BTISetValue(itor, numbers[mid]);
    if (begin < mid) {
        if (!BTIAddLeftChild(itor, -1))
            return false;
        if (!BuildInorderTree(BTILeftChild(itor), begin, mid))
            return false;
    }
    if (mid+1 < end) {
        if (!BTIAddRightChild(itor, -1))
            return false;
        if (!BuildInorderTree(BTIRightChild(itor), mid+1, end))
            return false;
    }
    return true;
}

bool BuildPostorderTree(BinaryTreeItor itor, int begin, int end)
{ 
    if (begin >= end)
        return false;
    
    BTISetValue(itor, numbers[end-1]);
    if (begin == end - 1)
        return true;
    int mid = (begin+end-1) / 2; // ceiling(mid(begin, end-2))
    if (mid > begin) {
        if (!BTIAddLeftChild(itor, -1))
            return false;
        if (!BuildPostorderTree(BTILeftChild(itor), begin, mid))
            return false;
    }
    if (mid < end-1) {
        if (!BTIAddRightChild(itor, -1))
            return false;
        if (!BuildPostorderTree(BTIRightChild(itor), mid, end-1))
            return false;
    }
    return true;
}

bool TestBTWalk(BinaryTree *btp)
{

    bool (*builders[])(BinaryTreeItor, int, int) = {
        &BuildPreorderTree,
        &BuildInorderTree,
        &BuildPostorderTree,
    };

    bool (*walks[])(BinaryTree *, TreeWalkerPtr) = {
        &BTPreorderWalk,
        &BTInorderWalk,
        &BTPostorderWalk,
    };

    const char *walk_names[] = {
        "BTPreorderWalk",
        "BTInorderWalk",
        "BTPostorderWalk",
    };
    
    unsigned t;
    walked_array = alloca(sizeof(int)*length);
    for (t = 0; t < sizeof(builders) / sizeof(builders[0]); t++) {
        if (!BTEmpty(btp)) {
            sprintf(error, "This test requires tree to be empty");
            return false;
        }

        BTAddRoot(btp, -1);
        (*builders[t])(BTRoot(btp), 0, length);

        walked_count = 0;
        if (!(*walks[t])(btp, &WalkBST)) {
            sprintf(error, "%s failed", walk_names[t]);
            return false;
        }
        
        if (walked_count != length) {
            sprintf(error, "%s walked count (%d) is different than length (%d)", walk_names[t], walked_count, length);
            return false;
        }

        int i; 
        for (i = 0; i < length; i++)
            if (walked_array[i] != numbers[i]) {
                sprintf(error, "%s walked array is different than numbers [%d]", walk_names[t], i);
                return false;
            }

        BTDeleteAll(btp);
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

bool TestBinaryTree()
{
    BinaryTree bt;
    bool ret = false;
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
    else if (!TestBTWalk(&bt)) 
        printf("TestBTWalk failed. Error: %s\n", error);
    else if (!TestBTFree(&bt)) 
        printf("TestBTFree failed. Error: %s\n", error);
    else {
        ret = true;
        printf("TestBinaryTree successful!!\n");
    }
    return ret;
}

bool WalkBST(int value)
{
    walked_count++;
    if (walked_count > length)
        return false;
    walked_array[walked_count-1] = value;
    return true;
}

bool TestBinarySortTree()
{
    BinarySortTree bst;
    if (!BSTInit(&bst)) {
        printf("BSTInit failed.\n");
        return false;
    }

    int i;
    for (i = 0; i < length; i++) {
        if (!BSTInsert(&bst, numbers[i])) {
            printf("BSTInsert failed.\n");
            return false;
        }
    }
    
    // Test sort (inorder walk)
    walked_array = alloca(sizeof(int) * length);
    walked_count = 0;
    if (!BSTInorderWalk(&bst, &WalkBST)) {
        printf("BSTInorderWalk failed.\n");
        return false;
    }
    if (walked_count != length) {
        printf("The number of BST's values [%d] is different than length [%d].", walked_count, length);
        return false;
    }
            
    int unsorted = -1;
    if ((unsorted = UnsortedIndex(walked_array, 0, walked_count) != -1)) {
        printf("Error: Tree's values are not sorted [%d] after inorder walk!!\n", unsorted);
        return false;
    }
    
    int missing = -1;
    if ((missing = FindMissingElement(numbers, walked_array, 0, walked_count)) != -1) {
        printf("Error: Element %d (index %d) is missing after inorder walk!!\n", numbers[missing], missing);
        return false;
    }

    // Test search
    for (i = 0; i < length; i++) {
        BinarySortTreeItor itor = BSTSearch(&bst, numbers[i]);
        if (BSTINull(itor)) {
            printf("Search failed: cannot find value [%d] in tree.", i);
            return false;
        }
        if (BSTIValue(itor) != numbers[i]) {
            printf("Found value from BST is wrong [%d].", i);
            return false;
        }
    }
    if (!BSTINull(BSTSearch(&bst, TestMaxValue+1))) {
        printf("Found a nonexisting value (bigger than max).");
        return false;
    }
    if (!BSTINull(BSTSearch(&bst, TestMinValue-1))) {
        printf("Found a nonexisting value (smaller than min).");
        return false;
    }

    // Test successor & predecessor
    for (i = 2; i < length-1; i++) {
        BinarySortTreeItor itor = BSTSearch(&bst, walked_array[i]); // Already tested
        BinarySortTreeItor successor = BSTISuccessor(itor);
        BinarySortTreeItor predecessor = BSTIPredecessor(itor);
        if (walked_array[i-1] != walked_array[i] && walked_array[i] != walked_array[i+1]) {
            if (BSTIValue(successor) != walked_array[i+1]) {
                printf("Successor returns wrong data!!\n");
                return false;
            }
            else if(BSTIValue(predecessor) != walked_array[i-1]) {
                printf("Predecessor returns wrong data!!\n");
                return false;
            }
        } else {
            if (!BSTINull(successor) && BSTIValue(successor) < walked_array[i]) {
                printf("Successor returns wrong data %d - (%d, %d)(allow equal)!!\n", BSTIValue(successor), walked_array[i], walked_array[i+1]);
                return false;
            }
            else if (!BSTINull(predecessor) && BSTIValue(predecessor) > walked_array[i]) {
                printf("Predecessor returns wrong data %d - (%d, %d) (allow equal)!!\n", BSTIValue(predecessor), walked_array[i-1], walked_array[i]);
                return false;
            }
        }
    }

    // test delete
    int k = 0;
    BinarySortTreeItor itor = BSTSearch(&bst, numbers[k]);
    if (!BSTDelete(itor)) {
        printf("BSTDelete failed\n");
        return false;
    }
    walked_count = 0;
    if (!BSTInorderWalk(&bst, &WalkBST)) {
        printf("BSTInorderWalk failed after delete.\n");
        return false;
    }
    if (walked_count != length-1) {
        printf("The number of BST's values [%d] is different than length-1 [%d] after delete.\n", walked_count, length-1);
        return false;
    }
            
    unsorted = -1;
    if ((unsorted = UnsortedIndex(walked_array, 0, walked_count) != -1)) {
        printf("Error: Tree's values are not sorted [%d] after inorder walk after delete!!\n", unsorted);
        return false;
    }
    
    missing = -1;
    if ((missing = FindMissingElement(numbers, walked_array, 0, walked_count)) != -1 && numbers[missing] != numbers[k]) {
        printf("Error: Element %d (index %d) is missing after inorder walk after delete!!\n", numbers[missing], missing);
        return false;
    }

    if (!BSTFree(&bst)) {
        printf("BSTFree failed.\n");
        return false;
    }

    printf("TestBinarySortTree successful!!\n");
    return true;
}

bool TestRedBlackTree()
{
    RedBlackTree rbt;
    if (!RBTInit(&rbt)) {
        printf("RBTInit failed.\n");
        return false;
    }

    int i;
    for (i = 0; i < length; i++) {
        if (!RBTInsert(&rbt, numbers[i])) {
            printf("RBTInsert failed.\n");
            return false;
        }
    }
    
    // Test sort (inorder walk)
    walked_array = alloca(sizeof(int) * length);
    walked_count = 0;
    if (!RBTInorderWalk(&rbt, &WalkBST)) {
        printf("RBTInorderWalk failed.\n");
        return false;
    }
    if (walked_count != length) {
        printf("The number of RBT's values [%d] is different than length [%d].", walked_count, length);
        return false;
    }
            
    int unsorted = -1;
    if ((unsorted = UnsortedIndex(walked_array, 0, walked_count) != -1)) {
        printf("Error: Tree's values are not sorted [%d] after inorder walk!!\n", unsorted);
        return false;
    }
    
    int missing = -1;
    if ((missing = FindMissingElement(numbers, walked_array, 0, walked_count)) != -1) {
        printf("Error: Element %d (index %d) is missing after inorder walk!!\n", numbers[missing], missing);
        return false;
    }

    // Test search
    for (i = 0; i < length; i++) {
        RedBlackTreeItor itor = RBTSearch(&rbt, numbers[i]);
        if (RBTINull(itor)) {
            printf("Search failed: cannot find value [%d] in tree.", i);
            return false;
        }
        if (RBTIValue(itor) != numbers[i]) {
            printf("Found value from RBT is wrong [%d].", i);
            return false;
        }
    }
    if (!RBTINull(RBTSearch(&rbt, TestMaxValue+1))) {
        printf("Found a nonexisting value (bigger than max).");
        return false;
    }
    if (!RBTINull(RBTSearch(&rbt, TestMinValue-1))) {
        printf("Found a nonexisting value (smaller than min).");
        return false;
    }

    // Test successor & predecessor
    for (i = 2; i < length-1; i++) {
        RedBlackTreeItor itor = RBTSearch(&rbt, walked_array[i]); // Already tested
        RedBlackTreeItor successor = RBTISuccessor(itor);
        RedBlackTreeItor predecessor = RBTIPredecessor(itor);
        if (walked_array[i-1] != walked_array[i] && walked_array[i] != walked_array[i+1]) {
            if (RBTIValue(successor) != walked_array[i+1]) {
                printf("Successor returns wrong data!!\n");
                return false;
            }
            else if(RBTIValue(predecessor) != walked_array[i-1]) {
                printf("Predecessor returns wrong data!!\n");
                return false;
            }
        } else {
            if (!RBTINull(successor) && RBTIValue(successor) < walked_array[i]) {
                printf("Successor returns wrong data %d - (%d, %d)(allow equal)!!\n", RBTIValue(successor), walked_array[i], walked_array[i+1]);
                return false;
            }
            else if (!RBTINull(predecessor) && RBTIValue(predecessor) > walked_array[i]) {
                printf("Predecessor returns wrong data %d - (%d, %d) (allow equal)!!\n", RBTIValue(predecessor), walked_array[i-1], walked_array[i]);
                return false;
            }
        }
    }

    // test delete
    int k = 0;
    RedBlackTreeItor itor = RBTSearch(&rbt, numbers[k]);
    if (!RBTDelete(itor)) {
        printf("RBTDelete failed\n");
        return false;
    }
    walked_count = 0;
    if (!RBTInorderWalk(&rbt, &WalkBST)) {
        printf("RBTInorderWalk failed after delete.\n");
        return false;
    }
    if (walked_count != length-1) {
        printf("The number of RBT's values [%d] is different than length-1 [%d] after delete.\n", walked_count, length-1);
        return false;
    }
            
    unsorted = -1;
    if ((unsorted = UnsortedIndex(walked_array, 0, walked_count) != -1)) {
        printf("Error: Tree's values are not sorted [%d] after inorder walk after delete!!\n", unsorted);
        return false;
    }
    
    missing = -1;
    if ((missing = FindMissingElement(numbers, walked_array, 0, walked_count)) != -1 && numbers[missing] != numbers[k]) {
        printf("Error: Element %d (index %d) is missing after inorder walk after delete!!\n", numbers[missing], missing);
        return false;
    }

    if (!RBTFree(&rbt)) {
        printf("RBTFree failed.\n");
        return false;
    }

    printf("TestRedBlackTree successful!!\n");
    return true;
}

void TestTree()
{
    length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    numbers = alloca(sizeof(int)*length);
    GenerateRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);

    if (!TestBinaryTree() || !TestBinarySortTree() || !TestRedBlackTree()) {
        printf("Test tree falied - length: %d, MinValue: %d, MaxValue: %d\n", length, TestMinValue, TestMaxValue);
        PrintArrayInt("numbers", numbers, length);
    }
}
