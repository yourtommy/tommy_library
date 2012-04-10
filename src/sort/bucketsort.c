#include "sortprivate.h"
#include "list.h"
#include <stdlib.h>
#include <assert.h>

#ifdef TRACE
#include <stdio.h>
#endif

// TODO: move to list sort
void InsertionSortList(List *listp);

void 
BucketSortArray(int *a, int begin, int end)
{
    int len = end - begin;
    int min, max;
    min = max = a[begin];
    int i;
    for (i = begin + 1; i < end; i++) {
        if (a[i] < min)
            min = a[i];
        else if (a[i] > max)
            max = a[i];
    }

    List *bucket = alloca(sizeof(List) * len);
    for (i = 0; i < len; i++)
        ListInit(&bucket[i], LT_DLS);
    
    // Insert elements into bucket
    for (i = begin; i < end; i++) {
        int checksum = len * (a[i]-min) / (max-min+1); // range [0 ~ len*(n-1)/n], n = max-min+1
        ListAppend(&bucket[checksum], a[i]);
    }

#ifdef TRACE
    for (i = 0; i < len; i++) 
        printf("[%d]: %s\n", i, (ListEmpty(&bucket[i]) ? "" : "COOL"));
#endif

    // Sort each slot
    for (i = 0; i < len; i++) 
        InsertionSortList(&bucket[i]);

    // copy data from bucket into array
    int count = 0;
    for (i = 0; i < len; i++) {
        ListItor itor = ListHead(&bucket[i]);
        while (!ListItorNull(itor)) {
            a[begin + count++] = ListValue(itor);
            itor = ListItorNext(itor);
        }
    }
}

// TODO: move to list sort
void
InsertionSortList(List *listp)
{
    ListItor itor = ListHead(listp);
    while (!ListItorNull(itor)) {
        ListItor before = ListItorPrev(itor);
        int value = ListValue(itor);
        while (!ListItorNull(before) && ListValue(before) > value) {
            ListSetValue(ListItorNext(before), ListValue(before));
            before = ListItorPrev(before);
        }
        if (!ListItorNull(before)) 
            ListSetValue(ListItorNext(before), value);
        else
            ListSetValue(ListHead(listp), value);
        itor = ListItorNext(itor);
    }
}
