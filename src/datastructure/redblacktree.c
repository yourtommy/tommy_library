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

// defined in BinarySortTree
bool BSTIDeleteInternal(BinarySortTreeItor itor, 
        bool (*deleted_itor_handler)(BinarySortTreeItor deleted, BinarySortTreeItor child_of_deleted));

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

static bool
RBTDeleteFixup(RedBlackTreeItor itor)
{
    // Just fill it in black if it's red
    if (!RBTIBlack(itor)) {
        RBTISetColor(itor, RBTNC_BLACK);
        return true;
    }

    // From this on itor is black.
    // If itor is null it's also treated as black.

    BinaryTreeItor bt_itor = SUPER(SUPER(itor));

    // Just return if it's root
    if (BTIEqual(bt_itor, BTRoot(bt_itor.tree_p)))
        return true;
    
    if (BTIEqual(bt_itor, BTILeftChild(BTIParent(bt_itor)))) { // left child
         /************* CASE 1 **********
         * { black brother, 
         *   black left nephew,
         *   black right nephew }
         *
         *       parent-> x 
         *               / \
         *       itor-> B+  B <-brother
         *                 / \
         *   leftnephew-> B   B <-rightnephew
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *       parent-> x+~
         *               / \
         *       itor-> B~  R~ <-brother
         *                 / \
         *   leftnephew-> B   B <-rightnephew
         *
         * { continue: itor=>parent }
         ********************************/

         /************* CASE 2 **********
         * { black brother, 
         *   red left nephew,
         *   black right nephew }
         *
         *       parent-> x 
         *               / \
         *       itor-> B+  B <-brother
         *                 / \
         *   leftnephew-> R   B <-rightnephew
         *
         *               ||
         * right rotate \||/ 
         *               \/
         *
         *       parent-> x 
         *               / \
         *       itor-> B+  R <-leftnephew
         *                 / \
         *                    B <-brother
         *                   / \
         *                      B <-rightnephew
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *       parent-> x 
         *               / \
         *       itor-> B+  B~ <-leftnephew
         *                 / \
         *                    R~ <-brother
         *                   / \
         *                      B <-rightnephew
         *
         * { continue: itor=>itor (case 3) }
         *
         ********************************/

         /************* CASE 3 **********
         * { black brother, 
         *   arbitray left nephew,
         *   red right nephew }
         *
         *       parent-> x 
         *               / \
         *       itor-> B+  B <-brother
         *                 / \
         *   leftnephew-> x   R <-rightnephew
         *
         *               ||
         *  left rotate \||/ 
         *               \/
         *
         *      brother-> B 
         *               / \
         *     parent-> x   R <-rightnephew
         *             / \
         *     itor-> B+  x <-leftnephew
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *      brother-> x~
         *               / \
         *     parent-> B~  B~ <-rightnephew
         *             / \
         *     itor-> B~  x <-leftnephew
         *
         * { fixup complete }
         ********************************/

         /************* CASE 4 **********
         * { red brother, 
         *   black left nephew,
         *   black right nephew }
         *
         *       parent-> B 
         *               / \
         *       itor-> B+  R <-brother
         *                 / \
         *   leftnephew-> B   B <-rightnephew
         *
         *               ||
         *  left rotate \||/ 
         *               \/
         *
         *      brother-> R 
         *               / \
         *     parent-> B   B <-rightnephew
         *             / \
         *     itor-> B+  B <-leftnephew
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *      brother-> B~ 
         *               / \
         *     parent-> R~  B <-rightnephew
         *             / \
         *     itor-> B+  B <-leftnephew
         *
         * { continue: itor=>itor (case 1,2,3) }
         ********************************/
        return true;
    } else {                                    // right child
         /************* CASE 1 **********
         * { black brother, 
         *   black right nephew,
         *   black left nephew }
         *
         *        parent-> x 
         *                / \
         *     brother-> B   B+ <-itor
         *              / \
         *leftnephew-> B   B <-rightnephew
         *
         *                ||
         *  change color \||/ 
         *                \/
         *
         *        parent-> x+~
         *                / \
         *     brother-> R~  B~ <-itor
         *              / \
         *leftnephew-> B   B <-rightnephew
         *
         * { continue: itor=>parent }
         ********************************/

         /************* CASE 2 **********
         * { black brother, 
         *   red right nephew,
         *   black left nephew }
         *
         *        parent-> x 
         *                / \
         *     brother-> B   B+<-itor
         *              / \
         *leftnephew-> B   R <-rightnephew
         *
         *                ||
         *   left rotate \||/ 
         *                \/
         *
         *          parent-> x 
         *                  / \
         *   rightnephew-> R   B+ <-itor
         *                / \
         *     brother-> B 
         *              / \
         *leftnephew-> B 
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *          parent-> x 
         *                  / \
         *   rightnephew-> B~  B+ <-itor
         *                / \
         *     brother-> R~ 
         *              / \
         *leftnephew-> B 
         *
         * { continue: itor=>itor (case 3) }
         *
         ********************************/

         /************* CASE 3 **********
         * { black brother, 
         *   arbitray right nephew,
         *   red left nephew }
         *
         *        parent-> x 
         *                / \
         *     brother-> B   B+ <-itor
         *              / \
         *leftnephew-> R   x <-rightnephew
         *
         *               ||
         * right rotate \||/ 
         *               \/
         *
         *      brother-> B 
         *               / \
         * leftnephew-> R   x <-parent
         *                 / \
         *  rightnephew-> x   B+ <-itor
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *      brother-> x~ 
         *               / \
         * leftnephew-> B~  B~ <-parent
         *                 / \
         *  rightnephew-> x   B~ <-itor
         *
         * { fixup complete }
         ********************************/

         /************* CASE 4 **********
         * { red brother, 
         *   black right nephew,
         *   black left nephew }
         *
         *        parent-> B 
         *                / \
         *     brother-> R   B+ <-itor
         *              / \
         *leftnephew-> B   B <-rightnephew
         *
         *               ||
         * right rotate \||/ 
         *               \/
         *
         *      brother-> R 
         *               / \
         * leftnephew-> B   B <-parent
         *                 / \
         *  rightnephew-> B   B+ <-itor
         *
         *               ||
         * change color \||/ 
         *               \/
         *
         *      brother-> B~ 
         *               / \
         * leftnephew-> B   R~ <-parent
         *                 / \
         *  rightnephew-> B   B+ <-itor
         *
         * { continue: itor=>itor (case 1,2,3) }
         ********************************/
        return true;
    }
}

static bool
RBTDeleteCallback(BinarySortTreeItor deleted_itor, BinarySortTreeItor child_itor)
{
    if (!RBTIBlack(BSTI_TO_RBTI(deleted_itor))) // deleted is red, not need to fixup.
        return true;

    return RBTDeleteFixup(BSTI_TO_RBTI(child_itor));
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
    return BSTIDeleteInternal(SUPER(itor), &RBTDeleteCallback);
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
