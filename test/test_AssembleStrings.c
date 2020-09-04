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
extern char **instructionLines;
extern int lineIndex;
extern int lineNumber;

//forward jump and backward jump tested
void test_assembleStrings_given_strings_of_instruction_expect_all_of_them_are_assembled_correctly() {
  int len;
  char *lines[] = {
    "MOV R0, #0x10",
    "MOV A, #0xFF",
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
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//repeated use of label
void test_assembleStrings_given_strings_of_instruction_with_repeated_label_expect_ERR_DUPLICATE_LABEL_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, R3",
    "JMP @A+DPTR",
    "AGAIN: RRC A",
    "JNB 0x65, HERE   ;unknown label 'HERE'",
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
    TEST_ASSERT_EQUAL(ERR_DUPLICATE_LABEL, e->errorCode);
  }
}

//illegal label used (instruction mnemonics used as label)
void test_assembleStrings_given_strings_of_instruction_with_instruction_mnemonic_used_as_label_expect_ERR_ILLEGAL_LABEL_is_thrown() {
  int len;
  char *lines[] = {
    "MOV A, R3",
    "JMP @A+DPTR",
    "AGAIN: RRC A",
    "JNB: JB 0x65, HERE   ;unknown label 'HERE'",
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
    "DJNZ R3, -5  ;jump to DELAY",
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
  }
}

//given colon after an instruction mnemonic
void test_assembleStrings_given_strings_of_instruction_with_colon_after_an_instruction_mnemonic_expect_ERR_INVALID_OPERAND_is_thrown() {
  int len;
  char *lines[] = {
    //at first line, MOV is seen as an instruction mnemonic, not a label, thus the colon after the MOV is considered an invalid opernad
    "START: MOV: A, #0xFF",
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
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
}

void test_getNextInstructionLineInString_expect_next_instruction_line_is_read_from_array_of_strings_and_is_returned_everytime_the_function_is_called() {
  char *instructionLine;
  char *lines[] = {
    "add a, R0\n",
    "da A\n",
    "Cjne a, #23, HERE",
    NULL
  };
  
  lineIndex = 0;
  instructionLines = lines;
  
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