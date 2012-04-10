int Partition(int *a, int begin, int end);

void
QuickSortArray(int *a, int begin, int end)
{
    if (begin >= end)
        return;
    int mid = Partition(a, begin, end);
    QuickSortArray(a, begin, mid);
    QuickSortArray(a, mid+1, end);
}

