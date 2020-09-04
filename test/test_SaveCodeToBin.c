#include "unity.h"
#include "SaveCodeToBin.h"
#include "CustomTestAssertion.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_saveCodeToBin_given_binary_file_expect_code_memory_is_written_into_file() {
  char *filename = "./test/data/SaveCodeToBin/saveBin_test1.bin";
  uint8_t codeMemory[65536] = {0xAB, 0x03, 0xB5};
  int len = 3;
  
  saveCodeToBin(filename, codeMemory, len);
  TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, filename, len);
}

void test_saveCodeToBin_given_binary_file_expect_contents_of_file_is_same_as_code_memory() {
  char *filename = "./test/data/SaveCodeToBin/saveBin_test2.bin";
  uint8_t codeMemory[65536] = {0x23, 0x01, 0x00, 0x9A, 0x00};
  int len = 5;
  
  saveCodeToBin(filename, codeMemory, len);
  TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, filename, len);
}
