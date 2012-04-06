#include "testset.h"
#include <stdio.h>
#include <string.h>

void
PrintUsage(const char *exename)
{
    printf("Usage: %s <feature>\n"
            "Available features: sort, datastructure\n", exename);
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        PrintUsage(argv[0]);
        return -1;
    }
    const char *const feature = argv[1];
    if (strcmp(feature, "sort") == 0) {
        TestSort();
        return 0;
    }
    if (strcmp(feature, "datastructure") == 0) {
        TestDataStructure();
        return 0;
    }

    printf("Unsupported feature: %s\n", argv[1]);
    PrintUsage(argv[0]);
    return -1;
}
