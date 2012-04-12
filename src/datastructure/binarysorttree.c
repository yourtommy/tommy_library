#include "tree.h"
#include <stddef.h>

#define UNUSED(p) (void)(p)

static BinarySortTreeItor BSTNullItor = { 
    .bt_itor = (BinaryTreeItor) {
        .ptr = NULL,
        .tree_p = NULL,
    },
};

bool BSTInit(BinarySortTree *treep)
{
    UNUSED(treep);
    return false;
}

bool BSTFree(BinarySortTree *treep)
{
    UNUSED(treep);
    return false;
}

bool BSTEmpty(BinarySortTree *treep)
{
    UNUSED(treep);
    return false;
}

bool BSTInsert(BinarySortTree *treep, int value)
{
    UNUSED(treep);
    UNUSED(value);
    return false;
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
    UNUSED(treep);
    UNUSED(walkerp);
    return false;
}

int BSTIValue(BinarySortTreeItor itor)
{
    UNUSED(itor);
    return (int)((~(unsigned)0) >> 1);
}

bool BSTINull(BinarySortTreeItor itor)
{
    UNUSED(itor);
    return false;
}
