#include "hash.h"
#include "utilitytest.h"
#include <stdio.h>

static const int TestMinArrayLen = 1;
static const int TestMaxArrayLen = 1200;
static const int TestMinValue = -58902;
static const int TestMaxValue = 58292;

static void *const invalid_ptr = NULL; 

typedef struct Arguments
{
    HashType type;
    unsigned slot_num;
    void *hashing_func;
    int *values;
    int values_length;
} Arguments;

static const char *
HashTypeString(HashType type)
{
    switch (type) {
        case HT_DA:
            return "Direct Addressing";
        case HT_CA:
            return "Closed Addressing";
        case HT_OA: 
            return "Open Addressing";
        case HT_PH: 
            return "Perfect Hashing";
        default:
            return "Unknown Tyep";
    }
}

void
TestHash()
{
    const unsigned int length = GenerateRandomArrayLength(TestMinArrayLen, TestMaxArrayLen);
    int numbers[length];
    GenerateUniqueRandomArrayInt(numbers, length, TestMinValue, TestMaxValue);
    const int slot_num = length / 3;

    const int length_direct = 100;
    int numbers_direct[length];
    for (int i = 0; i < length_direct; i++)
        numbers_direct[i] = i;
    ShuffleArrayInt(numbers_direct, length_direct);

    Arguments TestCases[] = {
        { HT_DA, length_direct, invalid_ptr, numbers_direct, length_direct },
        { HT_CA, slot_num, &CAHashingMod, numbers, length },
        { HT_CA, slot_num, &CAHashingMult, numbers, length },
        { HT_OA, slot_num, &OAHashingLinearProbing, numbers, length },
        { HT_OA, slot_num, &OAHashingQuadraticProbing, numbers, length },
        { HT_PH, slot_num, invalid_ptr, numbers, length },
    };

    bool succeeded = true;

    for (unsigned i = 0; i < sizeof(TestCases) / sizeof(TestCases[0]); i++) {
	printf("Begin to test %s Hash:\n", HashTypeString(TestCases[i].type));
        Hash h;
        bool b;
        if (TestCases[i].hashing_func != invalid_ptr)
            b = HashInit(&h, TestCases[i].type, TestCases[i].slot_num, TestCases[i].hashing_func);
        else
            b = HashInit(&h, TestCases[i].type, TestCases[i].slot_num);
        if (!b) {
            printf("HashInit failed for type: %s\n\n", HashTypeString(TestCases[i].type));
            succeeded = false;
            continue;
        }
        for (int j = 0; j < TestCases[i].values_length; j++) {
            if (HashSearch(&h, TestCases[i].values[j])) {
                printf("Found [%d] before inserting for type: %s\n\n", j, HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
            if (!HashInsert(&h, TestCases[i].values[j])) {
                printf("HashInsert falied for type: %s\n\n", HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
            if (!HashSearch(&h, TestCases[i].values[j])) {
                printf("Cannot find [%d] after inserting for type: %s\n\n", j, HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
        }
        if (!b)
            continue;
        if (!b)
            continue;
        for (int j = 0; j < TestCases[i].values_length; j++) {
            if (!HashSearch(&h, TestCases[i].values[j])) {
                printf("Cannot find [%d] before deleting for type: %s\n\n", j, HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
            if (!HashDelete(&h, TestCases[i].values[j])) {
                printf("HashDelete falied for type: %s\n\n", HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
            if (HashSearch(&h, TestCases[i].values[j])) {
                printf("Can find [%d] after deleting for type: %s\n\n", j, HashTypeString(TestCases[i].type));
                b = succeeded = false;
                break;
            }
        }
        if (!b)
            continue;
        if (!HashFree(&h)) {
            printf("HashFree failed for type: %s\n\n", HashTypeString(TestCases[i].type));
            b = succeeded = false;
            continue;
        }
	if (b)
	    printf("Test %s Hash succeeded!!\n\n", HashTypeString(TestCases[i].type));
    }

    /*
    if (!succeeded)
	PrintArrayInt("Hash Test failed", numbers, length);
    */
}

