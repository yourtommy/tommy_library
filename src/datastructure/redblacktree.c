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

bool
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

    // parent is red, then it can't be the root, so itor must have a grandparent
    BinaryTreeItor grandparent_bt_itor = BTIParent(BTIParent(bt_itor));
    RedBlackTreeItor grandparent_itor = BTI_TO_RBTI(grandparent_bt_itor);

    BinaryTreeItor uncle_bt_itor;
    RedBlackTreeItor uncle_itor;

    // parent is a left child
    if (BTIEqual(parent_bt_itor, BTILeftChild(grandparent_bt_itor))) {
        uncle_bt_itor = BTIRightChild(grandparent_bt_itor);
        uncle_itor = BTI_TO_RBTI(uncle_bt_itor);

        /***************** Precondition ***************
         * if parent is red, then
         * bother and grandparent are both black
         ***********************************************/

         if (RBTIBlack(uncle_itor)) {
         /************* CASE 1 **********
         * { black uncle, right itor }
         *
         *  grandparent-> B 
         *               / \
         *     parent-> R   B <-uncle
         *             / \
         *  brother-> B   R <-itor
         *
         *               ||
         *  left rotate \||/ 
         *               \/
         *
         *  grandparent-> B
         *               / \
         *       itor-> R   B <-uncle
         *             / \
         *   parent-> R    
         *           /
         *brother-> B
         *
         * { continue: itor => parent (case 2) }
         ******************************/
            if (BTIEqual(bt_itor, BTIRightChild(parent_bt_itor))) {
                if (!RBTILeftRotate(parent_itor)) return false;
                return RBTIInsertFixup(parent_itor);
            }

         /************* CASE 2 **********
         * { black uncle, left itor }
         *
         *  grandparent-> B 
         *               / \
         *     parent-> R   B <-uncle
         *             / \
         *     itor-> R   B <-brother 
         *
         *               ||
         * right rotate \||/ 
         *               \/
         *
         *       parent-> R
         *               / \
         *       itor-> R   B <-grandparent
         *                 / \
         *      brother-> B   B <-uncle    
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *       parent-> B~
         *               / \
         *       itor-> R   R~ <-grandparent
         *                 / \
         *      brother-> B   B <-uncle    
         *
         * { fixup complete }
         ******************************/
            if (!RBTIRightRotate(grandparent_itor)) return false;
            if (!RBTISetColor(grandparent_itor, RBTNC_RED)) return false;
            if (!RBTISetColor(parent_itor, RBTNC_BLACK)) return false;
            return true;
         }

        /************* CASE 3 **********
         * { red uncle, arbitary itor }
         *
         *  grandparent-> B 
         *               / \
         *     parent-> R   R <-uncle
         *             / \
         *     itor-> R   B <-brother 
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *  grandparent-> R~ 
         *               / \
         *     parent-> B~  B~ <-uncle
         *             / \
         *     itor-> R   B <-brother 
         *
         * OR
         *
         *  grandparent-> B 
         *               / \
         *     parent-> R   R <-uncle
         *             / \
         *  brother-> B   R <-itor 
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *  grandparent-> R~
         *               / \
         *     parent-> B~  B~ <-uncle
         *             / \
         *  brother-> B   R <-itor 
         *
         * { continue: itor => grandparent }
         ******************************/
         if (!RBTISetColor(grandparent_itor, RBTNC_RED)) return false;
         if (!RBTISetColor(parent_itor, RBTNC_BLACK)) return false;
         if (!RBTISetColor(uncle_itor, RBTNC_BLACK)) return false;
         return RBTIInsertFixup(grandparent_itor);
    }
    // parent is a right child
    else {
        uncle_bt_itor = BTILeftChild(grandparent_bt_itor);
        uncle_itor = BTI_TO_RBTI(uncle_bt_itor);

        /***************** Precondition ***************
         * if parent is red, then
         * bother and grandparent are both black
         ***********************************************/

        if (RBTIBlack(uncle_itor)) {
        /************* CASE 1 **********
         * { black uncle, left itor }
         *
         *  grandparent-> B 
         *               / \
         *      uncle-> B   R <-parent
         *                 / \
         *         itor-> R   B <-brother
         *
         *               ||
         * right rotate \||/ 
         *               \/
         *
         *  grandparent-> B
         *               / \
         *      uncle-> B   R <-itor
         *                 / \
         *                    R <-parent
         *                     \
         *                      B <-brother
         *
         * { continue: itor => parent (case 2) }
         ******************************/
            if (BTIEqual(bt_itor, BTILeftChild(parent_bt_itor))) {
                if (!RBTIRightRotate(parent_itor)) return false;
                return RBTIInsertFixup(parent_itor);
            }

        /************* CASE 2 **********
         * { black uncle, right itor }
         *
         *  grandparent-> B 
         *               / \
         *      uncle-> B   R <-parent
         *                 / \
         *      brother-> B   R <-itor 
         *
         *               ||
         *  left rotate \||/ 
         *               \/
         *
         *       parent-> R
         *               / \
         *grandparent-> B   R <-itor
         *             / \
         *    uncle-> B   B <-brother
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *       parent-> B~
         *               / \
         *grandparent-> R~  R <-itor
         *             / \
         *    uncle-> B   B <-brother    
         *
         * { fixup complete }
         ******************************/
            if (!RBTILeftRotate(grandparent_itor)) return false;
            if (!RBTISetColor(grandparent_itor, RBTNC_RED)) return false;
            if (!RBTISetColor(parent_itor, RBTNC_BLACK)) return false;
            return true;
        }

        /************* CASE 3 **********
         * { red uncle, arbitary itor }
         *
         *  grandparent-> B 
         *               / \
         *      uncle-> R   R <-parent
         *                 / \
         *      brother-> B   R <-itor
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *  grandparent-> R~
         *               / \
         *      uncle-> B~  B~ <-parent
         *                 / \
         *      brother-> B   R <-itor
         *
         * OR
         *
         *  grandparent-> B 
         *               / \
         *      uncle-> R   R <-parent
         *                 / \
         *         itor-> R   B <-brother
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *  grandparent-> R~
         *               / \
         *      uncle-> B~  B~ <-parent
         *                 / \
         *         itor-> R   B <-brother
         *
         * { continue: itor => grandparent }
         ******************************/
        if (!RBTISetColor(grandparent_itor, RBTNC_RED)) return false;
        if (!RBTISetColor(parent_itor, RBTNC_BLACK)) return false;
        if (!RBTISetColor(uncle_itor, RBTNC_BLACK)) return false;
        return RBTIInsertFixup(grandparent_itor);
    }
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
