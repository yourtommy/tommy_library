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
    // TODO
    UNUSED(treep);
    return false;
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
    // TODO
    UNUSED(itor);
    UNUSED(value);
    return false;
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
    // TODO
    UNUSED(itor);
    UNUSED(value);
    return false;
}

BinaryTreeItor BTIParent(BinaryTreeItor itor)
{
    // TODO
    UNUSED(itor);
    return BTNullItor;
}

int BTIValue(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return INFINITY;
    return itor.ptr->value;
}

bool BTISetValue(BinaryTreeItor itor, int value)
{
    // TODO
    UNUSED(itor);
    UNUSED(value);
    return false;
}

bool BTIDelete(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return true;
    if (!BTIDelete(BTILeftChild(itor)))
        return false;
    if (!BTIDelete(BTIRightChild(itor)))
        return false;
    if (BTIEqual(itor, itor.tree_p->root_itor))
        itor.tree_p->root_itor = BTNullItor;
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
