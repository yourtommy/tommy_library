#include "tree.h"
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>

static BinaryTreeItor 
BTNullItor = {
    .ptr = NULL,
    .tree_p = NULL,
};

bool 
BTInit(BinaryTree *treep)
{
    if (treep == NULL)
        return false;
    treep->root_itor = BTNullItor;
    return true;
}

bool 
BTFree(BinaryTree *treep)
{
    return BTDeleteAll(treep);
}

bool 
BTEmpty(BinaryTree *treep)
{
    return treep == NULL || BTINull(treep->root_itor);
}

BinaryTreeItor 
BTRoot(BinaryTree *treep)
{
    return treep->root_itor;
}

BinaryTreeItor 
BTAddRoot(BinaryTree *treep, int value)
{
    if (treep == NULL || !BTINull(BTRoot(treep)))
        return BTNullItor;
    BinaryTreeNode *node_p = malloc(sizeof(BinaryTreeNode));
    node_p->parent_p = NULL;
    node_p->left_child_p = NULL;
    node_p->right_child_p = NULL;
    node_p->value = value;

    treep->root_itor = (BinaryTreeItor) {
        .ptr = node_p,
        .tree_p = treep,
    };
    return BTRoot(treep);
}

bool 
BTDeleteAll(BinaryTree *treep)
{
    if (treep == NULL)
        return false;
    return BTIDelete(treep->root_itor);
}

bool 
BTINull(BinaryTreeItor itor)
{
    return itor.tree_p == NULL || itor.ptr == NULL;
}

bool 
BTIEqual(BinaryTreeItor itor1, BinaryTreeItor itor2)
{
    return itor1.ptr == itor2.ptr && itor1.tree_p == itor2.tree_p;
}

BinaryTreeItor 
BTILeftChild(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->left_child_p,
        .tree_p = itor.tree_p,
    };
}

BinaryTreeItor 
BTIAddLeftChild(BinaryTreeItor itor, int value)
{
    if (BTINull(itor) || !BTINull(BTILeftChild(itor)))
        return BTNullItor;
    BinaryTreeNode *nodep = malloc(sizeof(BinaryTreeNode));
    nodep->parent_p = itor.ptr;
    nodep->left_child_p = nodep->right_child_p = NULL;
    nodep->value = value;
    itor.ptr->left_child_p = nodep;
    return BTILeftChild(itor);
}

BinaryTreeItor 
BTIRightChild(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->right_child_p,
        .tree_p = itor.tree_p,
    };
}

BinaryTreeItor 
BTIAddRightChild(BinaryTreeItor itor, int value)
{
    if (BTINull(itor) || !BTINull(BTIRightChild(itor)))
        return BTNullItor;
    BinaryTreeNode *nodep = malloc(sizeof(BinaryTreeNode));
    nodep->parent_p = itor.ptr;
    nodep->left_child_p = nodep->right_child_p = NULL;
    nodep->value = value;
    itor.ptr->right_child_p = nodep;
    return BTIRightChild(itor);
}

BinaryTreeItor 
BTIParent(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return BTNullItor;
    return (BinaryTreeItor) {
        .ptr = itor.ptr->parent_p,
        .tree_p = itor.tree_p,
    };
}

int 
BTIValue(BinaryTreeItor itor)
{
    if (BTINull(itor))
        return INFINITY_INT;
    return itor.ptr->value;
}

bool 
BTISetValue(BinaryTreeItor itor, int value)
{
    if (BTINull(itor))
        return false;
    itor.ptr->value = value;
    return true;
}

bool 
BTIDelete(BinaryTreeItor itor)
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

bool 
BTIAncestor(BinaryTreeItor itor, BinaryTreeItor descendant)
{
    if (BTINull(itor) || BTINull(descendant) 
            || itor.tree_p != descendant.tree_p)
        return false;
    BinaryTreeItor ancestor = descendant;
    while (!BTINull(ancestor) && !BTIEqual(itor, ancestor))
        ancestor = BTIParent(ancestor);
    return !BTINull(ancestor);
}

bool 
BTIMoveAsLeftChild(BinaryTreeItor itor, BinaryTreeItor dest)
{
    if (BTINull(itor) || BTINull(dest) || itor.tree_p != dest.tree_p
        || !BTINull(BTILeftChild(dest))
        || BTIAncestor(itor, dest))
        return false;
    // itor's parent can't be null otherwise it must be dest's ancestor
    if (itor.ptr->parent_p->left_child_p == itor.ptr) // left child
        itor.ptr->parent_p->left_child_p = NULL;
    else                                              // right child
        itor.ptr->parent_p->right_child_p = NULL;
    itor.ptr->parent_p = dest.ptr;
    dest.ptr->left_child_p = itor.ptr;
    return true;
}

bool 
BTIMoveAsRightChild(BinaryTreeItor itor, BinaryTreeItor dest)
{
    if (BTINull(itor) || BTINull(dest) || itor.tree_p != dest.tree_p
        || !BTINull(BTIRightChild(dest))
        || BTIAncestor(itor, dest))
        return false;
    // itor's parent can't be null otherwise it must be dest's ancestor
    if (itor.ptr->parent_p->left_child_p == itor.ptr) // left child
        itor.ptr->parent_p->left_child_p = NULL;
    else                                              // right child
        itor.ptr->parent_p->right_child_p = NULL;
    itor.ptr->parent_p = dest.ptr;
    dest.ptr->right_child_p = itor.ptr;
    return true;
}

bool 
BTPreorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp) 
{
    if (walkerp == NULL)
        return false;
    BinaryTreeItor walk_stack[4096];
    int walk_stack_top = -1;
    BinaryTreeItor itor = BTRoot(treep);
    while (!BTINull(itor)) {
        if (!(*walkerp)(BTIValue(itor))) // walk
            return false;
        if (!BTINull(BTIRightChild(itor)))
            walk_stack[++walk_stack_top] = BTIRightChild(itor); // push right
        if (!BTINull(BTILeftChild(itor)))
            walk_stack[++walk_stack_top] = BTILeftChild(itor); // push left
        if (walk_stack_top < 0)
            itor = BTNullItor;
        else
            itor = walk_stack[walk_stack_top--]; // top && pop left (first try) or right 
    }
    return true;
}

bool 
BTInorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp) 
{
    if (walkerp == NULL)
        return false;
    BinaryTreeItor walk_stack[4096];
    int walk_stack_top = -1;
    BinaryTreeItor itor = BTRoot(treep); 
    while (true) {
        // Left
        while (!BTINull(itor)) {
            walk_stack[++walk_stack_top] = itor; // push left
            itor = BTILeftChild(itor);
        }

        // Parent
        if (walk_stack_top < 0) // empty
            break;
        itor = walk_stack[walk_stack_top--]; // top & pop self
        if (!(*walkerp)(BTIValue(itor)))
            return false;
        
        // Right
        if (!BTINull(BTIRightChild(itor))) 
            itor = BTIRightChild(itor); 
        else
            itor = BTNullItor;
    }
    return true;
}

bool 
BTPostorderWalk(BinaryTree *treep, TreeWalkerPtr walkerp) 
{
    if (walkerp == NULL)
        return false;
    if (BTEmpty(treep))
        return true;

    BinaryTreeItor walk_stack[4096];
    int walk_stack_top = -1;
    BinaryTreeItor prev = BTNullItor;
    walk_stack[++walk_stack_top] = BTRoot(treep);
    BinaryTreeItor itor;
    while (true) {
        itor = walk_stack[walk_stack_top]; // top self
        if (BTIEqual(itor, prev)  // back from a child to parent
                || BTIEqual(prev, BTILeftChild(itor)) 
                || BTIEqual(prev, BTIRightChild(itor))) { 
            if (!(*walkerp)(BTIValue(itor))) // walk
                return false;
            if (--walk_stack_top < 0) // pop
                break;
        } else { // go down to a child or jump from a left child to a right child
            if (!BTINull(BTIRightChild(itor)))
                walk_stack[++walk_stack_top] = BTIRightChild(itor); // push right
            if (!BTINull(BTILeftChild(itor)))
                walk_stack[++walk_stack_top] = BTILeftChild(itor);  // push left
        }
        prev = itor;
    }

    return true;
}
