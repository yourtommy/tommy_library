#include "tree.h"
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>

#define BSTI_TO_RBTI(bst_itor) \
    (RedBlackTreeItor) { \
        .SUPER_MEMBER = (BinarySortTreeItor) { \
            .SUPER_MEMBER = (BinaryTreeItor) { \
                .ptr = SUPER(bst_itor).ptr, \
                .tree_p = SUPER(bst_itor).tree_p, \
            }, \
        }, \
    }

bool RBTInit(RedBlackTree *treep)
{
    if (!BSTInit(SUPER_PTR(treep)))
        return false;

    return true;
}

bool RBTFree(RedBlackTree *treep)
{
    return BSTFree(SUPER_PTR(treep));
}

bool RBTEmpty(RedBlackTree *treep)
{
    return BSTEmpty(SUPER_PTR(treep));
}

bool RBTInsert(RedBlackTree *treep, int value)
{
    BinaryTree *btp = (BinaryTree *)treep;
    if (BTEmpty(btp)) {
        BTAddRoot(btp, value);
        return true;
    }
    BinaryTreeItor itor = BTRoot(btp);
    bool added = false;
    while (!added) {
        if (value < BTIValue(itor)) {
            if (BTINull(BTILeftChild(itor))) {
                if (!BTIAddLeftChild(itor, value))
                    return false;
                added = true;
            } else
                itor = BTILeftChild(itor);
        } else {
            if (BTINull(BTIRightChild(itor))) {
                if (!BTIAddRightChild(itor, value))
                    return false;
                added = true;
            } else
                itor = BTIRightChild(itor);
        }
    }
            
    return true;
}

bool RBTDelete(RedBlackTreeItor itor)
{
    if (RBTINull(itor))
        return false;

    BinarySortTreeItor bsti = SUPER(itor);
    BinarySortTreeItor to_delete = SUPER(itor);
    // two children
    if (!BTINull(BTILeftChild(SUPER(bsti)))
            && !BTINull(BTIRightChild(SUPER(bsti))))
        to_delete = BSTISuccessor(bsti); // Its successor must be in its right child but not its ancestor, and only have one or zero child.

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

    if (!BTIEqual(SUPER(bsti), SUPER(to_delete)))
        BTISetValue(SUPER(bsti), BSTIValue(to_delete));
    free(SUPER(to_delete).ptr);
    return true;
}

RedBlackTreeItor RBTSearch(RedBlackTree *treep, int value)
{
    return BSTI_TO_RBTI(BSTSearch(SUPER_PTR(treep), value));
}    

bool RBTInorderWalk(RedBlackTree *treep, TreeWalkerPtr walkerp)
{
    return BSTInorderWalk(SUPER_PTR(treep), walkerp);
}

int RBTIValue(RedBlackTreeItor itor)
{
    return BSTIValue(SUPER(itor));
}

bool RBTINull(RedBlackTreeItor itor)
{
    return BSTINull(SUPER(itor));
}

RedBlackTreeItor RBTISuccessor(RedBlackTreeItor itor)
{
    return BSTI_TO_RBTI(BSTISuccessor(SUPER(itor)));
}

RedBlackTreeItor RBTIPredecessor(RedBlackTreeItor itor)
{
    return BSTI_TO_RBTI(BSTIPredecessor(SUPER(itor)));
}

bool RBTLeftRotate(RedBlackTreeItor itor)
{
    // TODO
    UNUSED(itor);
    return false;
}

