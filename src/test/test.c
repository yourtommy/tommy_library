#include "testset.h"
#include <stdio.h>
#include <string.h>

typedef struct TestCases
{
    char *case_name;
    void (*test_func)();
} TestCases;


static TestCases cases[] = {
    { "sort", &TestSort },
    { "datastructure", &TestDataStructure },
    { "utility", &TestUtility },
};

void
PrintUsage(const char *exename)
{
    printf("Usage: %s <feature>\n"
            "Available features: ", exename);
    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++)
        printf("%s, ", cases[i].case_name);
    printf("\b\b  \n");
}

int
main(int argc, char *argv[])
{
    if (argc != 2) {
        PrintUsage(argv[0]);
        return -1;
    }
    const char *const feature = argv[1];

    for (unsigned i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        if (strcmp(feature, cases[i].case_name) == 0) {
            cases[i].test_func();
            return 0;
        }
    }

    printf("Unsupported feature: %s\n", feature);
    PrintUsage(argv[0]);
    return 1;
}
