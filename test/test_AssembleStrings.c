#include "Exception.h"
#include "unity.h"
#include "CException.h"
#include "AssembleStrings.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "MemAlloc.h"
#include "SaveCodeToBin.h"
#include "Assembler8051.h"
#include "ExceptionThrowing.h"
#include "AssembleFile.h"
#include "Tokenizer.h"
#include "Token.h"
#include "TokenizerExceptionThrowing.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern char **instructionLines;
extern uint8_t codeMemory[];
extern int lineNumber;
extern int lineIndex;

//forward jump and backward jump tested
void test_assembleStrings_given_strings_of_instruction_expect_all_of_them_are_assembled_correctly() {
  int len;
  char *lines[] = {
    "\t\t   MOV R0, #0x10",
    "\t\t   MOV A, #0xFF",
    "AGAIN: RRC A",
    "JNC UPDATE_COUNT   ;forward jump",
    "INC R1",
    "UPDATE_COUNT: DEC R0",
    "CJNE R0, #0, AGAIN ;backward jump",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_ASSERT_EQUAL(12, len);
    TEST_ASSERT_EQUAL_HEX8(0x78, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x10, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x74, codeMemory[2]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, codeMemory[3]);
    TEST_ASSERT_EQUAL_HEX8(0x13, codeMemory[4]);
    TEST_ASSERT_EQUAL_HEX8(0x50, codeMemory[5]);
    TEST_ASSERT_EQUAL_HEX8(0x01, codeMemory[6]);
    TEST_ASSERT_EQUAL_HEX8(0x09, codeMemory[7]);
    TEST_ASSERT_EQUAL_HEX8(0x18, codeMemory[8]);
    TEST_ASSERT_EQUAL_HEX8(0xB8, codeMemory[9]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[10]);
    TEST_ASSERT_EQUAL_HEX8(0xF8, codeMemory[11]);
    for(int i = 12; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//test relative is zero (base address is the target branch address)
void test_assembleStrings_given_strings_of_instruction_with_base_addr_same_as_branch_addr_expect_all_of_them_are_assembled_correctly() {
  int len;
  char *lines[] = {
    "CLR A",
    "SETB C",
    "ADDC A, #200",
    "MOV DPTR, #0xABCD",
    "MOVC A, @A+DPTR",
    "CJNE @R1, #0x50, AGAIN",
    "AGAIN: RRC A   ;branch to self",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_ASSERT_EQUAL(12, len);
    TEST_ASSERT_EQUAL_HEX8(0xE4, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xD3, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x34, codeMemory[2]);
    TEST_ASSERT_EQUAL_HEX8(0xC8, codeMemory[3]);
    TEST_ASSERT_EQUAL_HEX8(0x90, codeMemory[4]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[5]);
    TEST_ASSERT_EQUAL_HEX8(0xCD, codeMemory[6]);
    TEST_ASSERT_EQUAL_HEX8(0x93, codeMemory[7]);
    TEST_ASSERT_EQUAL_HEX8(0xB7, codeMemory[8]);
    TEST_ASSERT_EQUAL_HEX8(0x50, codeMemory[9]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[10]);
    TEST_ASSERT_EQUAL_HEX8(0x13, codeMemory[11]);
    for(int i = 12; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//label not defined
void test_assembleStrings_given_strings_of_instruction_but_jump_to_an_undefined_label_expect_ERR_UNKNOWN_LABEL_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, R3",
    "JMP @A+DPTR",
    "AGAIN: RRC A",
    "JNB 0x65, HERE   ;unknown label 'HERE'",
    "INC R1",
    "DEC R0",
    "CJNE R0, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_LABEL, e->errorCode);
    freeException(e);
  }
}

//jump without label
void test_assembleStrings_given_strings_of_instruction_and_jump_without_label_expect_it_is_able_to_assemble_correctly() {
  int len;
  char *lines[] = {
    "MOV A, #0xFF",
    "AGAIN: MOV R3, #200",
    "DELAY: NOP",
    "NOP",
    "nop",
    "DJNZ R3, -5  ;jump to DELAY",
    "DEC A",
    "CJNE A, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_ASSERT_EQUAL(13, len);
    TEST_ASSERT_EQUAL_HEX8(0x74, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x7B, codeMemory[2]);
    TEST_ASSERT_EQUAL_HEX8(0xC8, codeMemory[3]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[4]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[5]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[6]);
    TEST_ASSERT_EQUAL_HEX8(0xDB, codeMemory[7]);
    TEST_ASSERT_EQUAL_HEX8(0xFB, codeMemory[8]);
    TEST_ASSERT_EQUAL_HEX8(0x14, codeMemory[9]);
    TEST_ASSERT_EQUAL_HEX8(0xB4, codeMemory[10]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[11]);
    TEST_ASSERT_EQUAL_HEX8(0xF5, codeMemory[12]);
    for(int i = 13; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//repeated use of label
void test_assembleStrings_given_strings_of_instruction_with_repeated_label_expect_ERR_DUPLICATE_LABEL_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, R3",
    "JMP @A+DPTR",
    "AGAIN: RRC A\t",
    "JNB 0x65, HERE",
    "INC R1",
    "AGAIN: DEC R0    ;label 'AGAIN' defined at line 3",
    "CJNE R0, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_DUPLICATE_LABEL, e->errorCode);
    freeException(e);
  }
}

//illegal label used (instruction mnemonics used as label)
void test_assembleStrings_given_strings_of_instruction_with_instruction_mnemonic_used_as_label_expect_ERR_ILLEGAL_LABEL_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, R3",
    "JMP @A+DPTR",
    "AGAIN: RRC A",
    "JNB: JB 0x65, HERE   ;JNB is used as a label",
    "INC R1",
    "AGAIN: DEC R0",
    "CJNE R0, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_LABEL, e->errorCode);
    freeException(e);
  }
}

//given label is used twice in one line of instruction
void test_assembleStrings_given_strings_of_instruction_with_two_label_in_a_line_expect_ERR_INVALID_INSTRUCTION_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, #0xFF",
    "AGAIN: MOV R3, #200",
    "DELAY: HERE: NOP",    //token after the first colon must be an instruction, else it is considered an invalid instruction
    "NOP",
    "nop",
    "DJNZ R3, -5",
    "DEC A",
    "CJNE A, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION, e->errorCode);
    freeException(e);
  }
}

//given colon after an instruction mnemonic
void test_assembleStrings_given_strings_of_instruction_with_colon_after_an_instruction_mnemonic_expect_ERR_INVALID_OPERAND_is_thrown() {
  int len;
  char *lines[] = {
    //at first line, MOV is seen as an instruction mnemonic, not a label, thus the colon after the MOV is considered an invalid operand
    "START: MOV: A, #0xFF",
    "AGAIN: MOV R3, #200",
    "DELAY: NOP",
    "NOP",
    "nop",
    "DJNZ R3, -5",
    "DEC A",
    "CJNE A, #0, AGAIN",
    NULL
  };
  instructionLines = lines;
  
  Try{
    len = assembleStrings();
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
}

void test_getNextInstructionLineInString_expect_next_instruction_line_is_read_from_array_of_strings_and_is_returned_everytime_the_function_is_called() {
  char *instructionLine;
  char *lines[] = {
    "add a, R0\n",
    "da A\n",
    "\t\n",
    "Cjne a, #23, HERE",
    NULL
  };
  
  lineIndex = 0;
  instructionLines = lines;
  
  Try {
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("add a, R0\n", instructionLine);
    free(instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("da A\n", instructionLine);
    free(instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("\t\n", instructionLine);
    free(instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_STRING("Cjne a, #23, HERE", instructionLine);
    free(instructionLine);
    
    instructionLine = getNextInstructionLineInString();
    TEST_ASSERT_EQUAL_PTR(NULL, instructionLine);
    free(instructionLine);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    freeException(e);
  }
}