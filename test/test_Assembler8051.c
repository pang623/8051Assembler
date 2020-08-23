#include "unity.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include <stdio.h>
#include <stdlib.h>
#include "Error.h"
#include "Flags.h"
#include "CException.h"
#include "Exception.h"
#include "ExceptionThrowing.h"

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern uint8_t codeMemory[];
extern FILE *fileHandler;

void test_assembleInFileAndWriteToOutFile_given_asm_testCode_as_input_file_expect_opcode_written_to_bin_file() {
  char *inFile = "./test/data/asm_testCode.txt";
  char *outFile = "./test/data/asm_testCode.bin";
  
  assembleInFileAndWriteToOutFile(inFile, outFile);
}

void test_assembleInFileAndWriteToOutFile_given_test_as_input_file_expect_opcode_written_to_bin_file() {
  char *inFile = "./test/data/test.txt";
  char *outFile = "./test/data/test.bin";
  
  assembleInFileAndWriteToOutFile(inFile, outFile);
}

void test_assembleFile_given_filename_expect_instructions_in_file_are_read_and_written_into_code_memory() {
  int totalBytes;
  char *filename = "./test/data/asm_testCode.txt";
  Try{
    totalBytes = assembleFile(filename);
    TEST_ASSERT_EQUAL(21, totalBytes);
    TEST_ASSERT_EQUAL(0x74, codeMemory[0]);
    TEST_ASSERT_EQUAL(0x8A, codeMemory[1]);
    TEST_ASSERT_EQUAL(0x75, codeMemory[2]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[3]);
    TEST_ASSERT_EQUAL(0x12, codeMemory[4]);
    TEST_ASSERT_EQUAL(0x85, codeMemory[5]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[6]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[7]);
    TEST_ASSERT_EQUAL(0x95, codeMemory[8]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[9]);
    TEST_ASSERT_EQUAL(0x62, codeMemory[10]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[11]);
    TEST_ASSERT_EQUAL(0xC3, codeMemory[12]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[13]);
    TEST_ASSERT_EQUAL(0xD3, codeMemory[14]);
    TEST_ASSERT_EQUAL(0xDB, codeMemory[15]);
    TEST_ASSERT_EQUAL(0xF8, codeMemory[16]);
    TEST_ASSERT_EQUAL(0x2B, codeMemory[17]);
    TEST_ASSERT_EQUAL(0xB6, codeMemory[18]);
    TEST_ASSERT_EQUAL(0xE9, codeMemory[19]);
    TEST_ASSERT_EQUAL(0xF8, codeMemory[20]);
    for(int i = 21; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_assembleInstructions_given_func_ptr_expect_all_lines_in_asm_file_are_assembled_and_return_total_bytes_of_instructions() {
  int totalBytes;
  char *filename = "./test/data/asm_testCode.txt";
  fileHandler = fopen(filename, "r");
  Try{
    totalBytes = assembleInstructions(getNextInstructionLine);
    TEST_ASSERT_EQUAL(21, totalBytes);
    TEST_ASSERT_EQUAL(0x74, codeMemory[0]);
    TEST_ASSERT_EQUAL(0x8A, codeMemory[1]);
    TEST_ASSERT_EQUAL(0x75, codeMemory[2]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[3]);
    TEST_ASSERT_EQUAL(0x12, codeMemory[4]);
    TEST_ASSERT_EQUAL(0x85, codeMemory[5]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[6]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[7]);
    TEST_ASSERT_EQUAL(0x95, codeMemory[8]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[9]);
    TEST_ASSERT_EQUAL(0x62, codeMemory[10]);
    TEST_ASSERT_EQUAL(0x83, codeMemory[11]);
    TEST_ASSERT_EQUAL(0xC3, codeMemory[12]);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[13]);
    TEST_ASSERT_EQUAL(0xD3, codeMemory[14]);
    TEST_ASSERT_EQUAL(0xDB, codeMemory[15]);
    TEST_ASSERT_EQUAL(0xF8, codeMemory[16]);
    TEST_ASSERT_EQUAL(0x2B, codeMemory[17]);
    TEST_ASSERT_EQUAL(0xB6, codeMemory[18]);
    TEST_ASSERT_EQUAL(0xE9, codeMemory[19]);
    TEST_ASSERT_EQUAL(0xF8, codeMemory[20]);
    for(int i = 21; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  fclose(fileHandler);
}

void test_assembleInstructions_given_different_txt_file_containing_instruction_expect_codeMemory_is_written_with_new_machine_code() {
  int totalBytes;
  char *filename = "./test/data/test.txt";
  fileHandler = fopen(filename, "r");
  Try{
    totalBytes = assembleInstructions(getNextInstructionLine);
    TEST_ASSERT_EQUAL(2, totalBytes);
    TEST_ASSERT_EQUAL(0x95, codeMemory[0]);
    TEST_ASSERT_EQUAL(0xf4, codeMemory[1]);
    for(int i = 2; i < 65536; i++)
      TEST_ASSERT_EQUAL(0, codeMemory[i]);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  fclose(fileHandler);
}

void test_getNextInstructionLine_expect_next_instruction_line_is_returned_everytime_the_function_is_called() {
  char *filename = "./test/data/asm_testCode.txt";
  fileHandler = fopen(filename, "r");
  char *instructionLine;
  Try{
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("MOV A, #0x8A	 ;0x748A\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("MOV 0x83, #0x12	 ;0x758312\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("MOV 0xF4, 0x83	 ;0x8583F4\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("SUBB A, 0xF4	 ;0x95F4\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("XRL 0x83, A	 ;0x6283\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("CLR C		 ;0xC3\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("CPL A		 ;0xF4\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("SETB C		 ;0xD3\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("DJNZ R3, -8	 ;0xDBF8\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("ADD A, R3\n", instructionLine);
    
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING("CJNE @R0, #-23, -8\n", instructionLine);
    
    //no more lines left
    instructionLine = getNextInstructionLine();
    TEST_ASSERT_EQUAL_STRING(NULL, instructionLine);
  }Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  fclose(fileHandler);
}

void test_writeCodeToCodeMemory_given_opcode_0x7B_expect_opcode_stored_in_code_memory_and_return_the_size() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 10;                     //codePtr is pointing at code memory with the abs address 10
  Try{
    len = writeCodeToCodeMemory(0x7B, codePtr);
    TEST_ASSERT_EQUAL(1, len);                            //opcode is 1 byte
    TEST_ASSERT_EQUAL(0x7B, codeMemory[10]);              //opcode is stored at address 10
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_writeCodeToCodeMemory_given_opcode_0xABCD_expect_opcode_stored_in_code_memory_and_return_the_size() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xAB;                   //codePtr is pointing at code memory with the abs address 0xAB
  Try{
    len = writeCodeToCodeMemory(0xABCD, codePtr);
    TEST_ASSERT_EQUAL(2, len);                            //opcode is 2 bytes
    TEST_ASSERT_EQUAL(0xAB, codeMemory[0xAB]);            //opcode is stored at address 0xAB
    TEST_ASSERT_EQUAL(0xCD, codeMemory[0xAC]);            //opcode is stored at address 0xAC
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_writeCodeToCodeMemory_given_opcode_0x9BA12C_expect_opcode_stored_in_code_memory_and_return_the_size() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 200;                    //codePtr is pointing at code memory with the abs address 200
  Try{
    len = writeCodeToCodeMemory(0x9BA12C, codePtr);       //the opcode is encoded based on the sequence in the manual (LSB --> MSB)
    TEST_ASSERT_EQUAL(3, len);                            //opcode is 3 bytes
    TEST_ASSERT_EQUAL(0x9B, codeMemory[200]);             //opcode is stored at address 200 (little endian, LSB stored at lower address)
    TEST_ASSERT_EQUAL(0xA1, codeMemory[201]);             //opcode is stored at address 201
    TEST_ASSERT_EQUAL(0x2C, codeMemory[202]);             //opcode is stored at address 202
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_assembleInstruction_given_add_A_with_r7_expect_opcode_0x2f() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("add A, R7");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0x2f, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_direct_0xba_expect_opcode_0x25ba() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("add A, 0xBA");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0x25, codeMemory[100]);
    TEST_ASSERT_EQUAL(0xBA, codeMemory[101]);
    TEST_ASSERT_EQUAL(102, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_dir_with_imm_expect_opcode_0x75669A() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 250;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("Mov 0x66, #0x9A");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0x75, codeMemory[250]);
    TEST_ASSERT_EQUAL(0x66, codeMemory[251]);
    TEST_ASSERT_EQUAL(0x9A, codeMemory[252]);
    TEST_ASSERT_EQUAL(253, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_R0_expect_opcode_0x26() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 55;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("add A, @R0");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0x26, codeMemory[55]);
    TEST_ASSERT_EQUAL(56, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_movx_indirect_with_A_expect_opcode_0xf3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 123;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("MovX @r1 ,a");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xf3, codeMemory[123]);
    TEST_ASSERT_EQUAL(124, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_movc_a_with_aDPTR_expect_opcode_0x93() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x6A;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("MOvc a , @A+dPtR");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0x93, codeMemory[0x6A]);
    TEST_ASSERT_EQUAL(0x6B, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_c_with_bit_expect_opcode_0xA27B() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xBFCC;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("mOv c, 0x7b");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xA2, codeMemory[0xBFCC]);
    TEST_ASSERT_EQUAL(0x7B, codeMemory[0xBFCD]);
    TEST_ASSERT_EQUAL(0xBFCE, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_indirect_with_direct_expect_opcode_0xA7BA() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x65A;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("mOv @r1  , 0xBA  ;this will be seen as comment");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xA7, codeMemory[0x65A]);
    TEST_ASSERT_EQUAL(0xBA, codeMemory[0x65B]);
    TEST_ASSERT_EQUAL(0x65C, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_reg_with_imm_expect_opcode_0x78dd() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xD3D;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("mOv r0  , #0xDD");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0x78, codeMemory[0xD3D]);
    TEST_ASSERT_EQUAL(0xDD, codeMemory[0xD3E]);
    TEST_ASSERT_EQUAL(0xD3F, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_dptr_with_imm_expect_opcode_0x90EEFF() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xFFE;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("mOv dPtR  , #0xEEFF");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0x90, codeMemory[0xFFE]);
    TEST_ASSERT_EQUAL(0xEE, codeMemory[0xFFF]);
    TEST_ASSERT_EQUAL(0xFF, codeMemory[0x1000]);
    TEST_ASSERT_EQUAL(0x1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cjne_ind_with_imm_expect_opcode_stored_in_code_memory_is_0xB7179C() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x100;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("CjNe @r1 ,  #23 , -100");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0xB7, codeMemory[0x100]);
    TEST_ASSERT_EQUAL(0x17, codeMemory[0x101]);
    TEST_ASSERT_EQUAL(0x9C, codeMemory[0x102]);
    TEST_ASSERT_EQUAL(0x103, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cjne_reg_with_imm_expect_opcode_0xBD9EC8() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xF00F;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("cJnE R5 ,  #-98 , +200");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0xBD, codeMemory[0xF00F]);
    TEST_ASSERT_EQUAL(0x9E, codeMemory[0xF010]);
    TEST_ASSERT_EQUAL(0xC8, codeMemory[0xF011]);
    TEST_ASSERT_EQUAL(0xF012, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_djnz_dir_with_rel_expect_opcode_0xD5FA80() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 30000;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("DjNz   0xFA  ,  -128");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0xD5, codeMemory[30000]);
    TEST_ASSERT_EQUAL(0xFA, codeMemory[30001]);
    TEST_ASSERT_EQUAL(0x80, codeMemory[30002]);
    TEST_ASSERT_EQUAL(30003, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_djnz_reg_with_rel_expect_opcode_0xDEFF() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xF;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("DjNz   r6 ,  +255 ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xDE, codeMemory[15]);
    TEST_ASSERT_EQUAL(0xFF, codeMemory[16]);
    TEST_ASSERT_EQUAL(17, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_xch_a_with_reg_expect_opcode_0xC9() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xFF;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("   xCh a ,r1");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xC9, codeMemory[0xFF]);
    TEST_ASSERT_EQUAL(0x100, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_xchd_a_with_indirect_reg_expect_opcode_0xD7() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xCD;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("   xChD   a ,   @R1");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xD7, codeMemory[0xCD]);
    TEST_ASSERT_EQUAL(0xCE, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_DIV_AB_expect_opcode_0x84() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x99;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  dIv   aB ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0x84, codeMemory[0x99]);
    TEST_ASSERT_EQUAL(0x9A, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_setb_C_expect_opcode_0xD3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 21;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  sEtb c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xD3, codeMemory[21]);
    TEST_ASSERT_EQUAL(22, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_setb_bit_expect_opcode_0xD2B1() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 999;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  SetB  0xB1  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xD2, codeMemory[999]);
    TEST_ASSERT_EQUAL(0xB1, codeMemory[1000]);
    TEST_ASSERT_EQUAL(1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_A_expect_opcode_0xE4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  cLr a  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xE4, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_C_expect_opcode_0xC3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1000;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  clr c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xC3, codeMemory[1000]);
    TEST_ASSERT_EQUAL(1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_bit_expect_opcode_0xC294() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  clr 0x94  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xC2, codeMemory[2]);
    TEST_ASSERT_EQUAL(0x94, codeMemory[3]);
    TEST_ASSERT_EQUAL(4, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_A_expect_opcode_0xF4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 3003;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  cpL A  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xF4, codeMemory[3003]);
    TEST_ASSERT_EQUAL(3004, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_C_expect_opcode_0x84() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 4321;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  cpl c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0xB3, codeMemory[4321]);
    TEST_ASSERT_EQUAL(4322, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_bit_expect_opcode_0xB2DD() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 56789;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  cpl 0xDD  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xB2, codeMemory[56789]);
    TEST_ASSERT_EQUAL(0xDD, codeMemory[56790]);
    TEST_ASSERT_EQUAL(56791, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ajmp_addr11_expect_opcode_0xC1AB() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xBFF;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  aJMp 0x6AB  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xC1, codeMemory[0xBFF]);
    TEST_ASSERT_EQUAL(0xAB, codeMemory[0xC00]);
    TEST_ASSERT_EQUAL(0xC01, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_acall_addr11_expect_opcode_0xC1AB() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x765;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  aCAll 0x79a  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL(0xF1, codeMemory[0x765]);
    TEST_ASSERT_EQUAL(0x9A, codeMemory[0x766]);
    TEST_ASSERT_EQUAL(0x767, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_jnb_bit_rel_expect_opcode_0x305A95() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 365;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  jNb 0x5A   , -107  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0x30, codeMemory[365]);
    TEST_ASSERT_EQUAL(0x5A, codeMemory[366]);
    TEST_ASSERT_EQUAL(0x95, codeMemory[367]);
    TEST_ASSERT_EQUAL(368, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_LCALL_addr16_expect_opcode_0x12CD0A() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 800;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  lCaLL  0xCD0A ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL(0x12, codeMemory[800]);
    TEST_ASSERT_EQUAL(0xCD, codeMemory[801]);
    TEST_ASSERT_EQUAL(0x0A, codeMemory[802]);
    TEST_ASSERT_EQUAL(803, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ret_expect_opcode_0x22() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 8888;
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  ReT ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL(0x22, codeMemory[8888]);
    TEST_ASSERT_EQUAL(8889, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_indR0_token_expect_number_extracted_is_0_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int number = 8;
  Try{
    tokenizer = createTokenizer(" @r0 , A");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, number);                           //token is indirect, so number is extracted
    TEST_ASSERT_EQUAL_STRING("," , token->str);             //token is indirect, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                           //token is indirect, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_not_indirect_expect_number_is_not_extracted_and_token_is_pushed_back() {
  Tokenizer* tokenizer;
  Token *token;
  int number = 8;
  Try{
    tokenizer = createTokenizer("r3 #0x55");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(8, number);                           //number is not extracted since token is not indirect
    TEST_ASSERT_EQUAL_STRING("r3", token->str);             //token is not consumed since its not indirect
    TEST_ASSERT_EQUAL(0, isTrue);                           //function returns false since token is not indirect
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_is_indReg_but_invalidReg_expect_exception_ERR_INVALID_REGISTER_to_be_thrown() {
  Tokenizer* tokenizer;
  int number;
  Try{
    tokenizer = createTokenizer("    @ratatouille ");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_is_indReg_but_outOfRange_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer* tokenizer;
  int number;
  Try{
    tokenizer = createTokenizer("  @r3 ");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

//@A+DPTR, @A+PC and @DPTR operands are only found in MOVX and MOVC instructions,
//all other instructions have only @Ri as operands, which also means when @ token is detected,
//automatically the token next to it must be a register,
//this function is not used in assembling MOVX and MOVC instruction (@Ri is not checked using this function for MOVX and MOVC)
void test_isIndRegisterThenGetItsNumberAndConsume_given_is_ind_but_not_register_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Tokenizer* tokenizer;
  int number;
  Try{
    tokenizer = createTokenizer("  @hello ");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_negative_imm8bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer("   #-123@123");
    //-123 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -128, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(-123, value);                         //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("@" , token->str);             //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                           //token is immediate, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_imm16bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xB0;
  Try{
    tokenizer = createTokenizer(" #50000 happy");
    //50000 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -32768, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(50000, value);                       //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("happy" , token->str);        //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                          //token is immediate, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_negative_imm16bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xB1;
  Try{
    tokenizer = createTokenizer(" #-20000 sad");
    //-20000 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -32768, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(-20000, value);
    TEST_ASSERT_EQUAL_STRING("sad" , token->str);
    TEST_ASSERT_EQUAL(1, isTrue);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_imm8bit_with_plus_sign_expect_value_extracted_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0x05;
  Try{
    tokenizer = createTokenizer(" #+200 bye");
    //200 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -128, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(200, value);
    TEST_ASSERT_EQUAL_STRING("bye" , token->str);
    TEST_ASSERT_EQUAL(1, isTrue);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_not_imm_expect_value_is_not_extracted_and_token_is_pushed_back() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0x88;
  Try{
    tokenizer = createTokenizer(" 0xA0 haha");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0x88, value);                         //value is not extracted since token is not immediate
    TEST_ASSERT_EQUAL_STRING("0xA0", token->str);           //token is not consumed since its not immediate
    TEST_ASSERT_EQUAL(0, isTrue);                           //function returns false since token is not immediate
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_is_imm_but_outOfRange_expect_exception_ERR_INTEGER_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer* tokenizer;
  int value;
  Try{
    tokenizer = createTokenizer("#0xFFA  ");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);      //maximum range is set to 0xFF
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

//when token # is detected, it indicates it is immediate,
//automatically the token next to it must be integer
void test_isImmediateThenGetsItsValueAndConsume_given_is_imm_but_not_integer_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer* tokenizer;
  int value;
  Try{
    tokenizer = createTokenizer("  #r7  ");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsImmediateThenGetsItsValueAndConsume_given_is_imm_expect_value_is_extracted_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0x70;
  Try{
    tokenizer = createTokenizer("     #-85 hehe");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, -128, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(-85, value);
    TEST_ASSERT_EQUAL_STRING("hehe", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsImmediateThenGetsItsValueAndConsume_given_not_imm_expect_exception_ERR_EXPECTING_IMMEDIATE_to_be_thrown() {
  Tokenizer* tokenizer;
  int value;
  Try{
    tokenizer = createTokenizer(" @r0  ");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IMMEDIATE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_is_correct_operator_expect_token_is_consumed_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" @r0, r1");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "@");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("r0", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_wrong_operator_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" $hello  ");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "#");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("$", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_not_operator_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" doom  ");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "%");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("doom", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_is_correct_operator_expect_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" +#@ret");
    verifyIsOperatorTokenThenConsume(tokenizer, "+");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("#", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_wrong_operator_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer(" -A+B  ");
    verifyIsOperatorTokenThenConsume(tokenizer, "=");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_not_operator_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer(" 123456+  654321  ");
    verifyIsOperatorTokenThenConsume(tokenizer, "+");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_is_correct_identifier_expect_token_is_consumed_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" abcd123 x1y2z");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "abcd123");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("x1y2z", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_wrong_identifier_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer("     good bad  ");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "bad");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("good", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_not_identifier_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" 9987@#1  ");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "byebye321");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("9987", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_is_correct_identifier_expect_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" boba isgood2");
    verifyIsIdentifierTokenThenConsume(tokenizer, "boba");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("isgood2", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_wrong_identifier_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer(" McDonalds Or KFC");
    verifyIsIdentifierTokenThenConsume(tokenizer, "KFC");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_not_identifier_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer(" @!# done");
    verifyIsIdentifierTokenThenConsume(tokenizer, "fries");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_neg_integer_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -32678 6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -32768, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("6699876", token->str);
    TEST_ASSERT_EQUAL(-32678, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_integer_with_plus_sign_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" +12345 6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, 0, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("6699876", token->str);
    TEST_ASSERT_EQUAL(12345, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_integer_without_plus_sign_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 120 !6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 127);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("!", token->str);
    TEST_ASSERT_EQUAL(120, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_neg_integer_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer* tokenizer;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -129 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_pos_integer_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer* tokenizer;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" +300 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_pos_integer_without_sign_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer* tokenizer;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 300 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_not_integer_expect_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer* tokenizer;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -abcdef xyz  ");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIntegerTokenThenConsume_given_is_integer_expect_token_is_consumed_and_value_is_extracted() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 0x789 muthu");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0, 4095);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("muthu", token->str);
    TEST_ASSERT_EQUAL(0x789, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIntegerTokenThenConsume_given_not_integer_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer* tokenizer;
  int value;
  Try{
    tokenizer = createTokenizer("  +beef  or chicken");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_of_valid_range_expect_token_is_consumed_and_value_is_extracted_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  int regNum = 0;
  Try{
    tokenizer = createTokenizer(" r5 C");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("C", token->str);
    TEST_ASSERT_EQUAL(5, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_not_register_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  int regNum = 7;
  Try{
    tokenizer = createTokenizer(" @abcd");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("@", token->str);
    TEST_ASSERT_EQUAL(7, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_with_indirect_addressing_mode_but_out_of_range_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer* tokenizer;
  int regNum;
  Try{
    tokenizer = createTokenizer(" r5 bb");
    //Addressing mode is indirect addressing, only can receive register R0 and R1
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_with_register_addressing_mode_but_out_of_range_expect_exception_ERR_REG_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer* tokenizer;
  int regNum;
  Try{
    tokenizer = createTokenizer(" r21 bb");
    //Addressing mode is register addressing, only can receive register from R0 to R7
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_REG_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_invalid_register_expect_exception_ERR_INVALID_REGISTER_to_be_thrown() {
  Tokenizer* tokenizer;
  int regNum;
  Try{
    tokenizer = createTokenizer(" R5starry night");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsRegisterConsumeAndGetItsNumber_given_is_register_of_valid_range_expect_token_is_consumed_and_value_is_extracted() {
  Tokenizer* tokenizer;
  Token *token;
  int regNum = 5;
  Try{
    tokenizer = createTokenizer(" r1 register");
    verifyIsRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("register", token->str);
    TEST_ASSERT_EQUAL(1, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsRegisterConsumeAndGetItsNumber_given_not_register_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Tokenizer* tokenizer;
  int regNum;
  Try{
    tokenizer = createTokenizer(" 0x13 r2 ");
    verifyIsRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_null_token_expect_no_exception_is_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("     ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_newline_token_expect_no_exception_is_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  \n   ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_tab_token_expect_no_exception_is_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  \t   ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_semicolon_token_expect_no_exception_is_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer("  ;this is a comment   ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_not_whitespace_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer* tokenizer;
  Try{
    tokenizer = createTokenizer(" A");
    checkExtraToken(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_3668_expect_number_3668_is_extracted() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer("tree3668");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 4, token);
    TEST_ASSERT_EQUAL(3668, num);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_1tree_expect_exception_ERR_INVALID_REGISTER_is_thrown() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" R1tree");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1, token);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_t321_expect_exception_ERR_INVALID_REGISTER_is_thrown() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer(" Rt321");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1, token);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_null_expect_exception_ERR_INVALID_REGISTER_is_thrown() {
  Tokenizer* tokenizer;
  Token *token;
  Try{
    tokenizer = createTokenizer("R");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1, token);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithIndirect_expect_opcode_0x46() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("    A, @r0");
    //0x40 is the first nibble of the opcode which is passed into the function from the table
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_ASSERT_EQUAL(0x46, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithRegister_expect_opcode_0x9E() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a    , r6");
    opcode = assembleAWithOperands(tokenizer, 0x90, A_REG);
    TEST_ASSERT_EQUAL(0x9E, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithImmediate_expect_opcode_0x84C0() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a,    #0xC0");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_IMM);
    TEST_ASSERT_EQUAL(0x84C0, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithDirect_expect_opcode_0x7538() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, 0x38");
    opcode = assembleAWithOperands(tokenizer, 0x70, A_DIR);
    TEST_ASSERT_EQUAL(0x7538, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a r3");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_REG);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_first_operand_not_Acc_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x56 ,@r1");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_REG);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_correct_operand_combi_but_flag_is_not_set_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , 0x54 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_indirect_but_not_register_expect_exception_ERR_EXPECTING_REGISTER_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , @0x55 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a     , HELLO ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_ind_aPlusDPTR_expect_exception_ERR_EXPECTING_REGISTER_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    //although @a+dptr as second operand is valid, but it is not assembled using this function
    //@a+dptr, @a+pc, and @dptr as second operand all are assembled in another function(as they are only found in movx, movc instructions)
    //this function is only used to assemble instruction (add, subb, orl, ...)
    //with common operands (Rn, @Ri, imm, dir)
    //so this function just allows @Ri, if it sees '@'
    tokenizer = createTokenizer(" a     , @A+dptr ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_extra_operand_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a , @r0 r1 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_DirectWithImm_expect_opcode_0x93DEAA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xDE, #0xAA");
    opcode = assembleDirectWithOperands(tokenizer, 0x90, DIR_IMM);
    TEST_ASSERT_EQUAL(0x93DEAA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_DirectWithA_expect_opcode_0x7294() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x94 , a");
    opcode = assembleDirectWithOperands(tokenizer, 0x70, DIR_A);
    TEST_ASSERT_EQUAL(0x7294, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_correct_operand_combi_but_flag_not_set_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0xD2, #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_first_operand_not_direct_expect_exception_ERR_EXPECTING_INTEGER_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A, #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_correct_operands_but_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x88 #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_dir_with_dir_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    //although dir with dir is valid, but it is not assembled in this function,
    //it is the same case as the assembleAWithOperands function
    //this function is used just for assembling common operands (dir, A; dir, Imm)
    //and are used by instruction such as (anl, xrl, subb, ...)
    tokenizer = createTokenizer(" 0xD2, 0xB3 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_CWithBarBit_expect_opcode_0xD0CA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("C, /0xCA");
    opcode = assembleCWithOperands(tokenizer, 0xA0, C_BARBIT);
    TEST_ASSERT_EQUAL(0xD0CA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_CWithBit_expect_opcode_0x829B() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("C, 0x9B");
    opcode = assembleCWithOperands(tokenizer, 0x80, C_BIT);
    TEST_ASSERT_EQUAL(0x829B, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_correct_operand_combi_but_flag_not_set_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C, /0xBB ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_first_operand_not_C_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" R1, /0xBB ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BARBIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_correct_operands_but_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C  0x30 ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C, @R0 ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}