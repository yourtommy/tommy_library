#include "tree.h"
#include <stddef.h>

#define UNUSED(p)       (void)(p)

BinaryTreeItor BTNullItor = {
    .parent_p = NULL,
    .left_child_p = NULL,
    .right_child_p = NULL,
    .tree_p = NULL,
};

bool BTInit(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return false;
}

bool BTFree(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return false;
}

bool BTEmpty(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return false;
}

BinaryTreeItor BTRoot(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return BTNullItor;
}

bool BTAddRoot(BinaryTree *treep, int value)
{
    // TODO
    UNUSED(treep);
    UNUSED(value);
    return false;
}

bool BTDeleteAll(BinaryTree *treep)
{
    // TODO
    UNUSED(treep);
    return false;
}

bool BTINull(BinaryTreeItor itor)
{
    // TODO
    UNUSED(itor);
    return false;
}

bool BTIEqual(BinaryTreeItor itor1, BinaryTreeItor itor2)
{
    // TODO
    UNUSED(itor1);
    UNUSED(itor2);
    return false;
}

BinaryTreeItor BTILeftChild(BinaryTreeItor itor)
{
    // TODO
    UNUSED(itor);
    return BTNullItor;
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
    // TODO
    UNUSED(itor);
    return BTNullItor;
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
    // TODO
    UNUSED(itor);
    return false;
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
    // TODO
    UNUSED(itor);
    return false;
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
