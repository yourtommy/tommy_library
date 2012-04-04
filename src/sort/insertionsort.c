/* TODO: Move to a common utility */
void swap(int *ap, int *bp)
{
    int temp = *ap;
    *ap = *bp;
    *bp = temp;
}


void
InsertionSortArray(int *a, int begin, int len)
{
    int *p1, *p2;
    for (p1 = a + begin; p1 < a + begin + len; p1++)
        for (p2 = p1; p2 < a + begin + len; p2++)
            if (*p1 > *p2)
                swap(p1, p2);
}

