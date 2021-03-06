#include "tree.h"
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

#define BTI_TO_BSTI(bt_itor) \
    (BinarySortTreeItor) { \
        .SUPER_MEMBER = bt_itor \
    }

static BinarySortTreeItor BSTNullItor = { { NULL, NULL } };


bool 
BSTInit(BinarySortTree *treep)
{
    return BTInit(SUPER_PTR(treep));
}

bool 
BSTFree(BinarySortTree *treep)
{
    return BTFree(SUPER_PTR(treep));
}

bool 
BSTEmpty(BinarySortTree *treep)
{
    return BTEmpty(SUPER_PTR(treep));
}

BinarySortTreeItor 
BSTInsert(BinarySortTree *treep, int value)
{
    BinaryTree *btp = SUPER_PTR(treep);
    if (BTEmpty(btp)) {
        return BTI_TO_BSTI(BTAddRoot(btp, value));
    }
    
    BinaryTreeItor itor = BTRoot(btp);
    bool added = false;
    while (!added) {
        if (value < BTIValue(itor)) {
            if (BTINull(BTILeftChild(itor))) {
                itor = BTIAddLeftChild(itor, value);
                added = true;
            } else
                itor = BTILeftChild(itor);
        } else {
            if (BTINull(BTIRightChild(itor))) {
                itor = BTIAddRightChild(itor, value);
                added = true;
            } else
                itor = BTIRightChild(itor);
        }
    }
            
    return BTI_TO_BSTI(itor);
}

bool
BSTDeleteAll(BinarySortTree *treep)
{
    return BTDeleteAll(SUPER_PTR(treep));
}

BinarySortTreeItor 
BSTSearch(BinarySortTree *treep, int value)
{
    if (treep == NULL || BTEmpty(SUPER_PTR(treep)))
        return BSTNullItor;
    BinaryTreeItor itor = BTRoot(SUPER_PTR(treep));
    while (!BTINull(itor) && BTIValue(itor) != value)
        if (value < BTIValue(itor))
            itor = BTILeftChild(itor);
        else
            itor = BTIRightChild(itor);
    return (BinarySortTreeItor) { .SUPER_MEMBER = itor };
}

bool 
BSTInorderWalk(BinarySortTree *treep, TreeWalkerPtr walkerp) 
{
    if (treep == NULL)
        return false;
    return BTInorderWalk(SUPER_PTR(treep), walkerp);
}

// child_itorp is the child of the deleted node. This is used by RBT.
bool
BSTIDeleteInternal(BinarySortTreeItor itor, 
        bool (*deleted_itor_handler)(BinarySortTreeItor deleted, BinarySortTreeItor child_of_deleted))
{
    if (BSTINull(itor))
        return false;

    BinarySortTreeItor to_delete = itor;
    // two children
    if (!BTINull(BTILeftChild(SUPER(itor)))
            && !BTINull(BTIRightChild(SUPER(itor))))
        to_delete = BSTISuccessor(itor); // Its successor must be in its right child but not its ancestor, and only have one or zero child.

    BinaryTreeItor child_itor = BTILeftChild(SUPER(to_delete));
    if (BTINull(child_itor))
        child_itor = BTIRightChild(SUPER(to_delete));

    if (!BTINull(child_itor))
        child_itor.ptr->parent_p = SUPER(to_delete).ptr->parent_p;

    if (SUPER(to_delete).ptr->parent_p == NULL) // root
        SUPER(to_delete).tree_p->root_itor.ptr = child_itor.ptr;
    else if (SUPER(to_delete).ptr == SUPER(to_delete).ptr->parent_p->left_child_p) // left child
        SUPER(to_delete).ptr->parent_p->left_child_p = child_itor.ptr;
    else
        SUPER(to_delete).ptr->parent_p->right_child_p = child_itor.ptr;

    if (!BTIEqual(SUPER(itor), SUPER(to_delete)))
        BTISetValue(SUPER(itor), BSTIValue(to_delete));

    if (deleted_itor_handler != NULL
        && !(*deleted_itor_handler)(to_delete, BTI_TO_BSTI(child_itor)))
        return false; // The tree is broken after fixup falied! I can't rever it now.
    
    free(SUPER(to_delete).ptr);
    return true;
}

bool 
BSTIDelete(BinarySortTreeItor itor)
{
    return BSTIDeleteInternal(itor, NULL);
}

int 
BSTIValue(BinarySortTreeItor itor)
{
    return BTIValue(SUPER(itor));
}

bool 
BSTINull(BinarySortTreeItor itor)
{
    return BTINull(SUPER(itor));
}

BinarySortTreeItor 
BSTISuccessor(BinarySortTreeItor itor)
{
    if (BSTINull(itor))
        return BSTNullItor;
    BinaryTreeItor bt_itor = BTIRightChild(SUPER(itor));
    if (!BTINull(bt_itor)) {
        // the left-most descendant (minimum) of its right child
        while (!BTINull(BTILeftChild(bt_itor))) 
            bt_itor = BTILeftChild(bt_itor);
    }
    else {
        // the first parent whose left child it is.
        bt_itor = SUPER(itor);
        while (!BTINull(BTIParent(bt_itor)) && !BTIEqual(bt_itor, BTILeftChild(BTIParent(bt_itor))))
            bt_itor = BTIParent(bt_itor);
        bt_itor = BTIParent(bt_itor);
    }
    return (BinarySortTreeItor) { .SUPER_MEMBER=bt_itor };
}

BinarySortTreeItor 
BSTIPredecessor(BinarySortTreeItor itor)
{
    if (BSTINull(itor))
        return BSTNullItor;
    BinaryTreeItor bt_itor = BTILeftChild(SUPER(itor));
    if (!BTINull(bt_itor)) {
        // the right-most descendant (maximum) of its left child
        while (!BTINull(BTIRightChild(bt_itor))) 
            bt_itor = BTIRightChild(bt_itor);
    }
    else {
        bt_itor = SUPER(itor);
        // the first parent whose right child it is.
        while (!BTINull(BTIParent(bt_itor)) && !BTIEqual(bt_itor, BTIRightChild(BTIParent(bt_itor))))
            bt_itor = BTIParent(bt_itor);
        bt_itor = BTIParent(bt_itor);
    }
    return (BinarySortTreeItor) { .SUPER_MEMBER=bt_itor };
}
