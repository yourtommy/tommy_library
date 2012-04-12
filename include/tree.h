#ifndef TREE_H
#define TREE_H

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
} BinaryTree;

extern BinaryTreeItor BTNullItor;

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
bool BTAddRoot(BinaryTree *treep, int value);

/***********************************
 * After freeing a tree you must init
 * it again to use it.
 ***********************************/
bool BTDeleteAll(BinaryTree *treep);

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
bool BTIAddLeftChild(BinaryTreeItor itor, int value);

/***********************************
 * Return the right child of a binary tree itor.
 ***********************************/
BinaryTreeItor BTIRightChild(BinaryTreeItor itor);

/***********************************
 * Add a right child to itor.
 * It will fail if itor already has a right child.
 ***********************************/
bool BTIAddRightChild(BinaryTreeItor itor, int value);

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

#endif

