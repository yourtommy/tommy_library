#include "tree.h"
#include <stddef.h>
#include <stdlib.h>

#define UNUSED(p)       (void)(p)
#define INFINITY        (int)((~(unsigned)0) >> 1)

BinaryTreeItor BTNullItor = {
    .ptr = NULL,
    .tree_p = NULL,
};

bool BTInit(BinaryTree *treep)
{
    if (treep == NULL)
        return false;
    treep->root_itor = BTNullItor;
    return true;
}

bool BTFree(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return false;
}

bool BTEmpty(BinaryTree *treep)
{
    return treep == NULL || BTINull(treep->root_itor);
}

BinaryTreeItor BTRoot(BinaryTree *treep)
{
    return treep->root_itor;
}

bool BTAddRoot(BinaryTree *treep, int value)
{
    BinaryTreeNode *node_p = malloc(sizeof(BinaryTreeNode));
    node_p->parent_p = NULL;
    node_p->left_child_p = NULL;
    node_p->right_child_p = NULL;
    node_p->value = value;

    treep->root_itor = (BinaryTreeItor) {
        .ptr = node_p,
        .tree_p = treep,
    };
    return true;
}

bool BTDeleteAll(BinaryTree *treep)
{
    if (treep == NULL)
        return false;
    return BTIDelete(treep->root_itor);
}

bool BTINull(BinaryTreeItor itor)
{
    return itor.tree_p == NULL || itor.ptr == NULL;
}

bool BTIEqual(BinaryTreeItor itor1, BinaryTreeItor itor2)
{
    return itor1.ptr == itor2.ptr && itor1.tree_p == itor2.tree_p;
}

BinaryTreeItor BTILeftChild(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->left_child_p,
        .tree_p = itor.tree_p,
    };
}

bool BTIAddLeftChild(BinaryTreeItor itor, int value)
{
    if (BTINull(itor) || !BTINull(BTILeftChild(itor)))
        return false;
    BinaryTreeNode *nodep = malloc(sizeof(BinaryTreeNode));
    nodep->parent_p = itor.ptr;
    nodep->left_child_p = nodep->right_child_p = NULL;
    nodep->value = value;
    itor.ptr->left_child_p = nodep;
    return true;
}

BinaryTreeItor BTIRightChild(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->right_child_p,
        .tree_p = itor.tree_p,
    };
}

bool BTIAddRightChild(BinaryTreeItor itor, int value)
{
    if (BTINull(itor) || !BTINull(BTIRightChild(itor)))
        return false;
    BinaryTreeNode *nodep = malloc(sizeof(BinaryTreeNode));
    nodep->parent_p = itor.ptr;
    nodep->left_child_p = nodep->right_child_p = NULL;
    nodep->value = value;
    itor.ptr->right_child_p = nodep;
    return true;
}

BinaryTreeItor BTIParent(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->parent_p,
        .tree_p = itor.tree_p,
    };
}

int BTIValue(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return INFINITY;
    return itor.ptr->value;
}

bool BTISetValue(BinaryTreeItor itor, int value)
{
    if (BTINull(itor))
        return false;
    itor.ptr->value = value;
    return true;
}

bool BTIDelete(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return true;
    if (!BTIDelete(BTILeftChild(itor)))
        return false;
    if (!BTIDelete(BTIRightChild(itor)))
        return false;
    if (itor.ptr->parent_p == NULL) // root
        itor.tree_p->root_itor = BTNullItor;
    else
        if (itor.ptr == itor.ptr->parent_p->left_child_p)
            itor.ptr->parent_p->left_child_p = NULL;
        else
            itor.ptr->parent_p->right_child_p = NULL;
    free(itor.ptr);
    return true;
}

bool BTIMoveAsLeftChild(BinaryTreeItor itor, BinaryTreeItor dest)
{
    // TODO
    UNUSED(itor);
    UNUSED(dest);
    return false;
}

bool BTIMoveAsRightChild(BinaryTreeItor itor, BinaryTreeItor dest)
{
    // TODO
    UNUSED(itor);
    UNUSED(dest);
    return false;
}
