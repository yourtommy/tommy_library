#include "tree.h"
#include "utility.h"
#include <stddef.h>
#include <stdlib.h>

#define BSTI_TO_RBTI(bst_itor) \
    (RedBlackTreeItor) { \
        .SUPER_MEMBER = bst_itor \
    }

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
    if (!RBTISetColor(itor, RBTNC_BLACK)) { // Newly added node is a leaf and black
        BSTIDelete(SUPER(itor));
        return RBTNullItor;
    }

    // no need to rotate if it's root now
    if (BTIEqual(SUPER(SUPER(itor)), 
                BTRoot(SUPER_PTR(SUPER_PTR(treep))))) {
        RBTIColor(itor); // TODO: remove
        return itor;
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
    // TODO
    UNUSED(itor);
    return false;
}

bool 
RBTIRightRotate(RedBlackTreeItor itor)
{
    // TODO
    UNUSED(itor);
    return false;
}
