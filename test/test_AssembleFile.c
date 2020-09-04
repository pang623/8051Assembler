#include "unity.h"
#include "Exception.h"
#include "AssembleFile.h"
#include "DoubleLinkedList.h"
#include "MemAlloc.h"
#include "LabelInfo.h"
#include "CustomTestAssertion.h"
#include "Assembler8051.h"
#include "SaveCodeToBin.h"
#include "ExceptionThrowing.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern FILE *fileHandler;
extern uint8_t codeMemory[];
extern int lineNumber;

void test_assembleInFileAndWriteToOutFile_given_asm_testCode_as_input_file_expect_opcode_written_to_bin_file() {
  int totalBytes;
  char *inFile = "./test/data/asm_testCode1.txt";
  char *outFile = "./test/data/asm_testCode1.bin";

  Try{
    totalBytes = assembleInFileAndWriteToOutFile(inFile, outFile);
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, outFile, totalBytes);
}

//assemble another assembly txt file to see whether codeMemory will be reinitialized with new machine codes
void test_assembleInFileAndWriteToOutFile_given_test_as_input_file_expect_opcode_written_to_bin_file() {
  int totalBytes;
  char *inFile = "./test/data/asm_testCode2.txt";
  char *outFile = "./test/data/asm_testCode2.bin";

  Try{
    totalBytes = assembleInFileAndWriteToOutFile(inFile, outFile);
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, outFile, totalBytes);
}

//test fwd jump, backward jump, jump w/o label and absolute addressing with and w/o label
void test_assembleFile_given_filename_expect_instructions_in_file_are_read_and_written_into_code_memory() {
  int totalBytes;
  char *filename = "./test/data/AssembleFile/test_assembleFile.txt";
  Try{
    totalBytes = assembleFile(filename);
    TEST_ASSERT_EQUAL(20, totalBytes);
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
    TEST_ASSERT_EQUAL_HEX8(0x11, codeMemory[12]);
    TEST_ASSERT_EQUAL_HEX8(0x0E, codeMemory[13]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[14]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[15]);
    TEST_ASSERT_EQUAL_HEX8(0xDB, codeMemory[16]);
    TEST_ASSERT_EQUAL_HEX8(0xFC, codeMemory[17]);
    TEST_ASSERT_EQUAL_HEX8(0x01, codeMemory[18]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[19]);
    for(int i = 20; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_getNextInstructionLineInFile_expect_next_instruction_line_is_read_from_file_and_is_returned_everytime_the_function_is_called() {
  char *filename = "./test/data/AssembleFile/test_getNextInstructionLineInFile.txt";
  fileHandler = fopen(filename, "r");
  char *instructionLine;
  Try{
    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("CJNE @r0, #0x66, +10\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("SETB C		 ;0xD3\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("DJNZ R3, HERE	 ;0xDBF4\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("\t\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("DJNZ R4, -6\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("DA A\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("LJMP HERE\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("MUL AB\n", instructionLine);
    free(instructionLine);

    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_STRING("THERE: 	MOVX A, @DPTR ;comment", instructionLine);
    free(instructionLine);

    //no more lines left
    instructionLine = getNextInstructionLineInFile();
    TEST_ASSERT_EQUAL_PTR(NULL, instructionLine);
    free(instructionLine);
  }Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  fclose(fileHandler);
}
