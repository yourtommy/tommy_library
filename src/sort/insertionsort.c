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
    for (p1 = a + begin + 1; p1 < a + begin + len; p1++) {
        int key = *p1;
        p2 = p1 - 1;
        while (p2 >= a + begin && *p2 > key) {
            *(p2+1) = *p2;
            p2--;
        }
        *(p2+1) = key; 
    } 
}
