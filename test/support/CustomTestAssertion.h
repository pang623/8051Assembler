#ifndef CUSTOMTESTASSERTION_H
#define CUSTOMTESTASSERTION_H

#define TEST_ASSERT_EQUAL_BIN_FILE(expectedBinArr, filename, length)      \
    testAssertEqualBinFile(expectedBinArr, filename, length, __LINE__)

#include "unity.h"
#include <stdio.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>

char *createMessage(char *message, ...);
void testAssertEqualBinFile(uint8_t *expectedBinArr, char *filename, int length, int lineNum);

#endif // CUSTOMTESTASSERTION_H
