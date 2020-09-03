#include "unity.h"
#include "Exception.h"
#include "AssembleStrings.h"
#include "DoubleLinkedList.h"
#include "MemAlloc.h"
#include "LabelInfo.h"
#include "Assembler8051.h"
#include "ExceptionThrowing.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;


extern uint8_t codeMemory[];
extern char *instructionLines[];

void test_getNextInstructionLineInString_expect_next_instruction_line_is_read_from_array_of_strings_and_is_returned_everytime_the_function_is_called() {
  char *instructionLine;
  instructionLines[0] = "add a, R0\n";
  instructionLines[1] = "da A\n";
  instructionLines[2] = "Cjne a, #23, HERE";
  instructionLines[3] = NULL;
  
  Try {
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("add a, R0\n", instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("da A\n", instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("Cjne a, #23, HERE", instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_PTR(NULL, instructionLine);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}