#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

struct BinaryTree;

typedef struct BinaryTreeItor
{
    struct BinaryTreeItor *parent_p;
    struct BinaryTreeItor *left_child_p;
    struct BinaryTreeItor *right_child_p;
    struct BinaryTree *tree_p;
} BinaryTreeItor;

typedef struct BinaryTree
{
    BinaryTreeItor root_itor;
} BinaryTree;

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
 * Return the root that is just set.
 ***********************************/
BinaryTreeItor BTSetRoot(BinaryTree *treep, int value);

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
 * Return the left child of a binary tree itor.
 ***********************************/
BinaryTreeItor BTILeftChild(BinaryTreeItor itor);

/***********************************
 * Return the right child of a binary tree itor.
 ***********************************/
BinaryTreeItor BTIRightChild(BinaryTreeItor itor);

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
 * Delete the node pointed by itor.
 * Cannot use the itor after deleting it.
 ***********************************/
bool BTIDelete(BinaryTreeItor itor);

#endif

