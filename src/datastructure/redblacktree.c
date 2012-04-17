#include "tree.h"
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>

#define BSTI_TO_RBTI(bst_itor) \
    (RedBlackTreeItor) { \
        .SUPER_MEMBER = bst_itor \
    }

#define BTI_TO_RBTI(bst_itor) \
    (RedBlackTreeItor) { { bst_itor } }

#define RBTI_PTR(itor)  ((struct RedBlackTreeNode *)(SUPER(SUPER(itor)).ptr))

static RedBlackTreeItor RBTNullItor = { 
    { 
        {NULL, NULL} 
    }, 
}; 

static void *
RBTNewNode()
{
    return malloc(sizeof(RedBlackTreeNode));
}

static bool
RBTIColor(RedBlackTreeItor itor)
{
    return ((RedBlackTreeNode *)SUPER(SUPER(itor)).ptr)->color;
}

static bool 
RBTISetColor(RedBlackTreeItor itor, RedBlackTreeNodeColor color)
{
    if (RBTINull(itor))
        return false;
    ((RedBlackTreeNode *)SUPER(SUPER(itor)).ptr)->color = color;
    return true;
}

static bool 
RBTIBlack(RedBlackTreeItor itor)
{
    return RBTINull(itor) || RBTIColor(itor) == RBTNC_BLACK;
}

static bool
RBTIInsertFixup(RedBlackTreeItor itor)
{
    if (RBTINull(itor)) 
        return false;

    BinaryTreeItor bt_itor = SUPER(SUPER(itor));

    // is root, fill it in black and return
    if (BTIEqual(bt_itor, BTRoot(bt_itor.tree_p))) {
        RBTISetColor(itor, RBTNC_BLACK);
        return true;
    }

    BinaryTreeItor parent_bt_itor = BTIParent(bt_itor);
    RedBlackTreeItor parent_itor = BTI_TO_RBTI(parent_bt_itor);

    // parent is black
    if (RBTIBlack(parent_itor))
        return true;

    BinaryTreeItor brother_bt_itor;
    RedBlackTreeItor brother_itor;
    if (BTIEqual(bt_itor, BTILeftChild(parent_bt_itor))) {       // left child
        brother_bt_itor = BTIRightChild(parent_bt_itor);
        brother_itor = BTI_TO_RBTI(brother_bt_itor);

        BinaryTreeItor brother_lc_bt_itor = BTILeftChild(brother_bt_itor);
        BinaryTreeItor brother_rc_bt_itor = BTIRightChild(brother_bt_itor);
        RedBlackTreeItor brother_lc_itor = BTI_TO_RBTI(brother_lc_bt_itor);
        RedBlackTreeItor brother_rc_itor = BTI_TO_RBTI(brother_rc_bt_itor);

        // both red
        if (!RBTIBlack(brother_lc_itor) && !RBTIBlack(brother_rc_itor)) {
            RBTILeftRotate(parent_itor);
            RBTISetColor(brother_itor, RBTNC_RED); // brother is now parent's parent
            RBTISetColor(parent_itor, RBTNC_BLACK); // brother's left child is now parent's right child
            RBTISetColor(brother_rc_itor, RBTNC_BLACK); // brother's right child is now parent's brother.
            return RBTIInsertFixup(brother_itor);
        }

        // TODO
    } else {                                      // right child
        brother_bt_itor = BTILeftChild(parent_bt_itor);
        // TODO
    }

    // TODO
    return true;
}

bool 
RBTInit(RedBlackTree *treep)
{
    if (!BSTInit(SUPER_PTR(treep)))
        return false;

    ((BinaryTree *)treep)->NewNodePtr = &RBTNewNode;

    return true;
}

bool 
RBTFree(RedBlackTree *treep)
{
    return BSTFree(SUPER_PTR(treep));
}

bool 
RBTEmpty(RedBlackTree *treep)
{
    return BSTEmpty(SUPER_PTR(treep));
}

RedBlackTreeItor 
RBTInsert(RedBlackTree *treep, int value)
{
    BinarySortTree *bstp = SUPER_PTR(treep);
    RedBlackTreeItor itor = BSTI_TO_RBTI(
            BSTInsert(bstp, value));
    if (!RBTISetColor(itor, RBTNC_RED) // Newly added node is a leaf and red 
            || !RBTIInsertFixup(itor)) { 
        BSTIDelete(SUPER(itor));
        return RBTNullItor;
    }

    return itor;
}

bool
RBTDeleteAll(RedBlackTree *treep)
{
    return BSTDeleteAll(SUPER_PTR(treep));
}

bool 
RBTIDelete(RedBlackTreeItor itor)
{
    return BSTIDelete(SUPER(itor));
}

RedBlackTreeItor 
RBTSearch(RedBlackTree *treep, int value)
{
    return BSTI_TO_RBTI(BSTSearch(SUPER_PTR(treep), value));
}    

bool 
RBTInorderWalk(RedBlackTree *treep, TreeWalkerPtr walkerp)
{
    return BSTInorderWalk(SUPER_PTR(treep), walkerp);
}

int 
RBTIValue(RedBlackTreeItor itor)
{
    return BSTIValue(SUPER(itor));
}

bool 
RBTINull(RedBlackTreeItor itor)
{
    return BSTINull(SUPER(itor));
}

RedBlackTreeItor 
RBTISuccessor(RedBlackTreeItor itor)
{
    return BSTI_TO_RBTI(BSTISuccessor(SUPER(itor)));
}

RedBlackTreeItor 
RBTIPredecessor(RedBlackTreeItor itor)
{
    return BSTI_TO_RBTI(BSTIPredecessor(SUPER(itor)));
}

bool 
RBTILeftRotate(RedBlackTreeItor itor)
{
    if (RBTINull(itor))
        return false;

    BinaryTreeItor bt_itor = SUPER(SUPER(itor));
    BinaryTreeItor right_child_itor = BTIRightChild(bt_itor);

    if (BTINull(right_child_itor))
        return false;

    // parent <-> right child
    if (BTIEqual(bt_itor, BTRoot(bt_itor.tree_p)))
        bt_itor.tree_p->root_itor = right_child_itor; // reset root
    else {
        if (bt_itor.ptr == bt_itor.ptr->parent_p->left_child_p)
            bt_itor.ptr->parent_p->left_child_p = right_child_itor.ptr;
        else
            bt_itor.ptr->parent_p->right_child_p = right_child_itor.ptr;
    }
    right_child_itor.ptr->parent_p = bt_itor.ptr->parent_p;

    // this <-> right's left
    bt_itor.ptr->right_child_p = right_child_itor.ptr->left_child_p;
    if (right_child_itor.ptr->left_child_p != NULL)
        right_child_itor.ptr->left_child_p->parent_p = bt_itor.ptr;
    bt_itor.ptr->right_child_p = right_child_itor.ptr->left_child_p;

    // this <-> right
    bt_itor.ptr->parent_p = right_child_itor.ptr;
    right_child_itor.ptr->left_child_p = bt_itor.ptr;

    // reset root
    if (BTIEqual(bt_itor, BTRoot(bt_itor.tree_p)))
        bt_itor.tree_p->root_itor = right_child_itor;
    return true;
}

bool 
RBTIRightRotate(RedBlackTreeItor itor)
{
    if (RBTINull(itor))
        return false;

    BinaryTreeItor bt_itor = SUPER(SUPER(itor));
    BinaryTreeItor left_child_itor = BTILeftChild(bt_itor);

    if (BTINull(left_child_itor))
        return false;

    // parent <-> left child
    if (BTIEqual(bt_itor, BTRoot(bt_itor.tree_p)))
        bt_itor.tree_p->root_itor = left_child_itor; // reset root
    else {
        if (bt_itor.ptr == bt_itor.ptr->parent_p->right_child_p)
            bt_itor.ptr->parent_p->right_child_p = left_child_itor.ptr;
        else
            bt_itor.ptr->parent_p->left_child_p = left_child_itor.ptr;
    }
    left_child_itor.ptr->parent_p = bt_itor.ptr->parent_p;

    // this <-> left's right
    bt_itor.ptr->left_child_p = left_child_itor.ptr->right_child_p;
    if (left_child_itor.ptr->right_child_p != NULL)
        left_child_itor.ptr->right_child_p->parent_p = bt_itor.ptr;
    bt_itor.ptr->left_child_p = left_child_itor.ptr->right_child_p;

    // this <-> left
    bt_itor.ptr->parent_p = left_child_itor.ptr;
    left_child_itor.ptr->right_child_p = bt_itor.ptr;

    return true;
}
