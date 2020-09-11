#include "unity.h"
#include "Exception.h"
#include "ExceptionThrowing.h"
#include "TokenizerExceptionThrowing.h"
#include "Token.h"
#include "mock_Tokenizer.h"
#include "Assembler8051.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "AssembleFile.h"
#include "AssembleStrings.h"
#include "SaveCodeToBin.h"
#include "mock_DummyLineReader.h"
#include "mock_MemAlloc.h"
#include "CException.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern uint8_t codeMemory[];
/*
void test_assembleInstructions_through_mocking_expect_expected_functions_are_called_and_assembled_correctly_into_code_memory() {
  int totalBytes;
  char *lines[] = {
    "\tadd a, r0",
    "  xchd a, @r0\n",
    "\t\t\n",
    "xrl 0x56, A",
    NULL
  };
  
  char *trimmedLines[] = {
    "add a, r0",
    "xchd a, @r0\n",
    "",
    "xrl 0x56, A",
    NULL
  };
  
  Tokenizer tokenizer1 = {trimmedLines[0], 0, 0, strlen(trimmedLines[0]), 0, NULL};
  Tokenizer tokenizer2 = {trimmedLines[1], 0, 0, strlen(trimmedLines[1]), 0, NULL};
  Tokenizer tokenizer3 = {trimmedLines[3], 0, 0, strlen(trimmedLines[3]), 0, NULL};

  Try{
    dummyLineReader_IgnoreAndReturn(lines[0]);
    skipWhiteSpaces_ExpectAndReturn(lines[0], trimmedLines[0]);
    createTokenizer_ExpectAndReturn(trimmedLines[0], &tokenizer1);
    memFree_Expect(lines[0]);
    freeTokenizer_Expect(&tokenizer1);
    
    dummyLineReader_IgnoreAndReturn(lines[1]);
    skipWhiteSpaces_ExpectAndReturn(lines[1], trimmedLines[1]);
    createTokenizer_ExpectAndReturn(trimmedLines[1], &tokenizer2);
    memFree_Expect(lines[1]);
    freeTokenizer_Expect(&tokenizer2);
    
    dummyLineReader_IgnoreAndReturn(lines[2]);
    skipWhiteSpaces_ExpectAndReturn(lines[2], trimmedLines[2]);
    memFree_Expect(lines[2]);
    
    dummyLineReader_IgnoreAndReturn(lines[3]);
    skipWhiteSpaces_ExpectAndReturn(lines[3], trimmedLines[3]);
    createTokenizer_ExpectAndReturn(trimmedLines[3], &tokenizer3);
    memFree_Expect(lines[3]);
    freeTokenizer_Expect(&tokenizer3);
    
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
  codeMemory[0] = 0xAB;
  codeMemory[1] = 0xCD;
  char *lines[] = {
    "DA a",
    "CPL A",
    NULL
  };
  
  Tokenizer tokenizer = {NULL, 0, 0, 0, 0, NULL};

  Try{
    dummyLineReader_IgnoreAndReturn(lines[0]);
    memAlloc_ExpectAndReturn(sizeof(Tokenizer), &tokenizer);
    memFree_Expect(lines[0]);
    dummyLineReader_IgnoreAndReturn(lines[1]);
    memAlloc_ExpectAndReturn(sizeof(Tokenizer), &tokenizer);
    memFree_Expect(lines[1]);
    dummyLineReader_IgnoreAndReturn(lines[2]);
    totalBytes = assembleInstructions(dummyLineReader);
    TEST_ASSERT_EQUAL(2, totalBytes);
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
*/