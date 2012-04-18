#ifndef TREE_H__
#define TREE_H__

#include "utility.h"
#include <stdbool.h>

struct BinaryTree;

typedef struct BinaryTreeNode
{
    struct BinaryTreeNode *parent_p;
    struct BinaryTreeNode *left_child_p;
    struct BinaryTreeNode *right_child_p;
    int value;
} BinaryTreeNode;

typedef struct BinaryTreeItor
{
    struct BinaryTreeNode *ptr;
    struct BinaryTree *tree_p;
} BinaryTreeItor;

typedef struct BinaryTree
{
    BinaryTreeItor root_itor;
    void *(*NewNodePtr)(); // Polymophism!!
} BinaryTree;

typedef struct BinarySortTreeItor
{
    BinaryTreeItor SUPER_MEMBER; /* extends BinaryTreeItor */
} BinarySortTreeItor;

typedef struct BinarySortTree
{
    BinaryTree SUPER_MEMBER; /* extends BinaryTree */
} BinarySortTree;

typedef enum RedBlackTreeNodeColor
{
    RBTNC_RED,
    RBTNC_BLACK,
} RedBlackTreeNodeColor;

typedef struct RedBlackTreeNode
{
    BinaryTreeNode SUPER_MEMBER; /* extends BinaryTreeNode */
    RedBlackTreeNodeColor color;
} RedBlackTreeNode;

typedef struct RedBlackTree
{
    BinarySortTree SUPER_MEMBER; /* extends BinarySortTree */
} RedBlackTree;

typedef struct RedBlackTreeItor
{
    BinarySortTreeItor SUPER_MEMBER; /* extends BinarySortTree */
} RedBlackTreeItor;

/* If tree walker returns false then
 * then the walk terminates.
 */
typedef bool (*TreeWalkerPtr)(int value);

//==================================
// Binary Tree Functions
//==================================

/***********************************
 * Preorder tree walk
 **********************************/
bool BTPreorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp); 

/***********************************
 * Inorder tree walk
 **********************************/
bool BTInorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp); 

/***********************************
 * Postorder tree walk
 **********************************/
bool BTPostorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp); 

/*********************************** 
 * Must init a tree before using it.
 **********************************/
bool BTInit(BinaryTree *treep);

/***********************************
 * After freeing a tree you must init
 * it again to use it.
 ***********************************/
bool BTFree(BinaryTree *treep);

/***********************************
 * Check whether a binary tree is empty.
 ***********************************/
bool BTEmpty(BinaryTree *treep);

/***********************************
 * Return the root of a binary tree.
 ***********************************/
BinaryTreeItor BTRoot(BinaryTree *treep);

/***********************************
 * Set the root of a binary tree with value. 
 * It will fail if the tree is not empty. 
 ***********************************/
BinaryTreeItor BTAddRoot(BinaryTree *treep, int value);

/***********************************
 * After freeing a tree you must init
 * it again to use it.
 ***********************************/
bool BTDeleteAll(BinaryTree *treep);

/***********************************
 * Get the height of treep
 * A tree with only one node (the root) has a depth of zero.
 * while one with no node has a depth of -1.
 **********************************/
int BTHeight(BinaryTree *treep);

/***********************************
 * Check whether a binrary tree itor is null.
 ***********************************/
bool BTINull(BinaryTreeItor itor);

/***********************************
 * Check whether itor1 equals to itor2.
 ***********************************/
bool BTIEqual(BinaryTreeItor itor1, BinaryTreeItor itor2);

/***********************************
 * Return the left child of a binary tree itor.
 ***********************************/
BinaryTreeItor BTILeftChild(BinaryTreeItor itor);

/***********************************
 * Add a left child to itor.
 * It will fail if itor already has a left child.
 ***********************************/
BinaryTreeItor BTIAddLeftChild(BinaryTreeItor itor, int value);

/***********************************
 * Return the right child of a binary tree itor.
 ***********************************/
BinaryTreeItor BTIRightChild(BinaryTreeItor itor);

/***********************************
 * Add a right child to itor.
 * It will fail if itor already has a right child.
 ***********************************/
BinaryTreeItor BTIAddRightChild(BinaryTreeItor itor, int value);

/***********************************
 * Return the parent of a binary tree itor.
 ***********************************/
BinaryTreeItor BTIParent(BinaryTreeItor itor);

/***********************************
 * Return the value of a binary tree itor.
 ***********************************/
int BTIValue(BinaryTreeItor itor);

/***********************************
 * Set the value of a binary tree itor.
 ***********************************/
bool BTISetValue(BinaryTreeItor itor, int value);

/***********************************
 * Delete itor and all its descendants.
 * Cannot use the itor after deleting it.
 ***********************************/
bool BTIDelete(BinaryTreeItor itor);

/***********************************
 * Check whether itor is one of 
 * ancestors (including itself)
 * of descendant.
 **********************************/
bool BTIAncestor(BinaryTreeItor itor, BinaryTreeItor descendant);

/***********************************
 * Move itor with its descendants
 * to be the left child of dest.
 * It will fail if dest already has a left child
 * or itor is dest's ancestor.
 ***********************************/
bool BTIMoveAsLeftChild(BinaryTreeItor itor, BinaryTreeItor dest);

/***********************************
 * Move itor with its descendants
 * to be the right child of dest.
 * It will fail if dest already has a right child
 * or itor is dest's ancestor.
 ***********************************/
bool BTIMoveAsRightChild(BinaryTreeItor itor, BinaryTreeItor dest);


//==================================
// Binary Sort Tree Functions
//==================================

/*********************************** 
 * Must init a tree before using it.
 **********************************/
bool BSTInit(BinarySortTree *treep);

/***********************************
 * After freeing a tree you must init
 * it again to use it.
 **********************************/
bool BSTFree(BinarySortTree *treep);

/***********************************
 * Check whether a binary sort tree is empty.
 **********************************/
bool BSTEmpty(BinarySortTree *treep);

/***********************************
 * Insert value into treep
 **********************************/
BinarySortTreeItor BSTInsert(BinarySortTree *treep, int value);

/***********************************
 * Delete all nodes from BST 
 **********************************/
bool BSTDeleteAll(BinarySortTree *treep);

/***********************************
 * Delete itor from its contained BST 
 **********************************/
BinarySortTreeItor BSTSearch(BinarySortTree *treep, int value);

/***********************************
 * Inorder tree walk
 **********************************/
bool BSTInorderWalk(BinarySortTree *treep, TreeWalkerPtr walkerp); 

/***********************************
 * Delete itor from its contained BST 
 **********************************/
bool BSTIDelete(BinarySortTreeItor itor);

/***********************************
 * Return the value of a binary tree itor.
 ***********************************/
int BSTIValue(BinarySortTreeItor itor);

/***********************************
 * Check whether a binrary tree itor is null.
 ***********************************/
bool BSTINull(BinarySortTreeItor itor);

/***********************************
 * Get itor's successor
 ***********************************/
BinarySortTreeItor BSTISuccessor(BinarySortTreeItor itor);

/***********************************
 * Get itor's predecessor
 ***********************************/
BinarySortTreeItor BSTIPredecessor(BinarySortTreeItor itor);


//==================================
// Red Black Tree Functions
//==================================

/*********************************** 
 * Must init a tree before using it.
 **********************************/
bool RBTInit(RedBlackTree *treep);

/***********************************
 * After freeing a tree you must init
 * it again to use it.
 **********************************/
bool RBTFree(RedBlackTree *treep);

/***********************************
 * Check whether a binary sort tree is empty.
 **********************************/
bool RBTEmpty(RedBlackTree *treep);

/***********************************
 * Insert value into treep
 **********************************/
RedBlackTreeItor RBTInsert(RedBlackTree *treep, int value);

/***********************************
 * Delete itor from its contained RBT 
 **********************************/
bool RBTDeleteAll(RedBlackTree *treep);

/***********************************
 * Delete itor from its contained RBT 
 **********************************/
RedBlackTreeItor RBTSearch(RedBlackTree *treep, int value);

/***********************************
 * Inorder tree walk
 **********************************/
bool RBTInorderWalk(RedBlackTree *treep, TreeWalkerPtr walkerp); 

/***********************************
 * Delete itor from its contained RBT 
 **********************************/
bool RBTIDelete(RedBlackTreeItor itor);

/***********************************
 * Return the value of a binary tree itor.
 ***********************************/
int RBTIValue(RedBlackTreeItor itor);

/***********************************
 * Check whether a binrary tree itor is null.
 ***********************************/
bool RBTINull(RedBlackTreeItor itor);

/***********************************
 * Get itor's successor
 ***********************************/
RedBlackTreeItor RBTISuccessor(RedBlackTreeItor itor);

/***********************************
 * Get itor's predecessor
 ***********************************/
RedBlackTreeItor RBTIPredecessor(RedBlackTreeItor itor);

/***********************************
 * Don't use this API directly, since
 * They are exposed just for test.
 **********************************/
bool RBTILeftRotate(RedBlackTreeItor itor);

/***********************************
 * Don't use this API directly since
 * They are exposed just for test.
 **********************************/
bool RBTIRightRotate(RedBlackTreeItor itor);

#endif

