#include "tree.h"
#include <stddef.h>

#define UNUSED(p) (void)(p)
#define SUPER_PTR(tp) (&(tp->__super))
#define SUPER(t) (t.__super)

static BinarySortTreeItor BSTNullItor = { 
    .__super = (BinaryTreeItor) {
        .ptr = NULL,
        .tree_p = NULL,
    },
};


bool BSTInit(BinarySortTree *treep)
{
    return BTInit(SUPER_PTR(treep));
}

bool BSTFree(BinarySortTree *treep)
{
    return BTFree(SUPER_PTR(treep));
}

bool BSTEmpty(BinarySortTree *treep)
{
    return BTEmpty(SUPER_PTR(treep));
}

bool BSTInsert(BinarySortTree *treep, int value)
{
    BinaryTree *btp = SUPER_PTR(treep);
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

bool BSTDelete(BinarySortTreeItor itor)
{
    UNUSED(itor);
    return false;
}

BinarySortTreeItor BSTSearch(BinarySortTree *treep, int value)
{
    UNUSED(treep);
    UNUSED(value);
    return BSTNullItor;
}

bool BSTInorderWalk(BinarySortTree *treep, TreeWalkerPtr walkerp) 
{
    // TODO: fix the infinit iteration
#ifdef INFINITY_ITERATION
    if (walkerp == NULL)
        return false;

    BinaryTreeItor walk_stack[4096];
    int walk_stack_top = -1;
    walk_stack[++walk_stack_top] = BTRoot(SUPER_PTR(treep)); // push
    while (walk_stack_top >= 0) {
        BinaryTreeItor cur_itor = walk_stack[walk_stack_top]; // top
        if (!BTINull(BTILeftChild(cur_itor))) {
            walk_stack[++walk_stack_top] = BTILeftChild(cur_itor); // push
            continue;
        }
        --walk_stack_top; // pop
        (*walkerp)(BTIValue(cur_itor));
        if (!BTINull(BTIRightChild(cur_itor))) 
            walk_stack[++walk_stack_top] = BTIRightChild(cur_itor); // push
    }
    return true;
#endif
    UNUSED(treep);
    UNUSED(walkerp);
    return false;
}

int BSTIValue(BinarySortTreeItor itor)
{
    return BTIValue(SUPER(itor));
}

bool BSTINull(BinarySortTreeItor itor)
{
    return BTINull(SUPER(itor));
}
