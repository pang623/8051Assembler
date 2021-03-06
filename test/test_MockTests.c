#include "unity.h"
#include "Exception.h"
#include "ExceptionThrowing.h"
#include "TokenizerExceptionThrowing.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "AssembleFile.h"
#include "AssembleStrings.h"
#include "SaveCodeToBin.h"
#include "MemAlloc.h"
#include "mock_DummyLineReader.h"
#include "mock_MemoryAlloc.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern uint8_t codeMemory[];

void test_assembleInstructions_through_mocking_expect_expected_functions_are_called_and_assembled_correctly_into_code_memory() {
  int totalBytes;
  char *lines[] = {
    "\tadd a, r0",
    "  xchd a, @r0\n",
    "\t\t\n",
    "xrl 0x56, A",
    NULL
  };

  Try{
    dummyLineReader_IgnoreAndReturn(lines[0]);
    memoryFree_Expect(lines[0]);
    dummyLineReader_IgnoreAndReturn(lines[1]);
    memoryFree_Expect(lines[1]);
    dummyLineReader_IgnoreAndReturn(lines[2]);
    memoryFree_Expect(lines[2]);
    dummyLineReader_IgnoreAndReturn(lines[3]);
    memoryFree_Expect(lines[3]);
    dummyLineReader_IgnoreAndReturn(lines[4]);
    totalBytes = assembleInstructions(dummyLineReader);
    TEST_ASSERT_EQUAL(4, totalBytes);
    TEST_ASSERT_EQUAL_HEX8(0x28, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xD6, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x62, codeMemory[2]);
    TEST_ASSERT_EQUAL_HEX8(0x56, codeMemory[3]);
    for(int i = 4; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
}

void test_assembleInstructions_given_initialised_code_memory_expect_code_memory_is_rewritten_with_new_machine_code() {
  int totalBytes;
  char *lines[] = {
    "DA a",
    "CPL A",
    NULL
  };

  //initialises code memory
  for(int i = 0; i < 1000; i++)
    codeMemory[i] = 0xAA;

  Try{
    dummyLineReader_IgnoreAndReturn(lines[0]);
    memoryFree_Expect(lines[0]);
    dummyLineReader_IgnoreAndReturn(lines[1]);
    memoryFree_Expect(lines[1]);
    dummyLineReader_IgnoreAndReturn(lines[2]);
    totalBytes = assembleInstructions(dummyLineReader);
    TEST_ASSERT_EQUAL(2, totalBytes);
    
    //initialised code memory is rewritten with new machine code according to the instructions given
    TEST_ASSERT_EQUAL_HEX8(0xD4, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xF4, codeMemory[1]);
    for(int i = 2; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
}
