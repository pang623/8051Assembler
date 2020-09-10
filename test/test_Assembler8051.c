#include "Exception.h"
#include "ExceptionThrowing.h"
#include "unity.h"
#include "CException.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include "Errors.h"
#include "Error8051.h"
#include "Flags.h"
#include "TokenizerExceptionThrowing.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "AssembleFile.h"
#include "MemAlloc.h"
#include "SaveCodeToBin.h"
#include "CustomTestAssertion.h"
#include <stdio.h>
#include <stdlib.h>

void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

extern DoubleLinkedList *listPtr;
extern uint8_t codeMemory[];
extern int muteOnNoLabel;
extern int lineNumber;

void test_assembleInFileAndWriteToOutFile_given_asm_testCode1_as_input_file_expect_opcode_written_to_bin_file() {
  int totalBytes;
  char *inFile = "./test/data/asm_testCode1.txt";
  char *outFile = "./test/data/asm_testCode1.bin";

  Try{
    totalBytes = assembleInFileAndWriteToOutFile(inFile, outFile);
    TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, outFile, totalBytes);
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//assemble another assembly txt file to see whether codeMemory will be reinitialized with new machine codes
void test_assembleInFileAndWriteToOutFile_given_allInstructions_test_as_input_file_expect_opcode_written_to_bin_file() {
  int totalBytes;
  char *inFile = "./test/data/allInstructions_test.txt";
  char *outFile = "./test/data/allInstructions_test.bin";

  Try{
    totalBytes = assembleInFileAndWriteToOutFile(inFile, outFile);
    TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, outFile, totalBytes);
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//assemble another assembly txt file to see whether codeMemory will be reinitialized with new machine codes
void test_assembleInFileAndWriteToOutFile_given_asm_testCode2_as_input_file_expect_opcode_written_to_bin_file() {
  int totalBytes;
  char *inFile = "./test/data/asm_testCode2.txt";
  char *outFile = "./test/data/asm_testCode2.bin";

  Try{
    totalBytes = assembleInFileAndWriteToOutFile(inFile, outFile);
    TEST_ASSERT_EQUAL_BIN_FILE(codeMemory, outFile, totalBytes);
  } Catch(e){
    dumpTokenErrorMessage(e, lineNumber);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

//This function only take opcode up to 3 bytes as 8051 instructions is 3 bytes at most
void test_getInstructionBytes_given_one_byte_opcode_expect_size_is_one_byte() {
  int bytes;
  Try{
    bytes = getInstructionBytes(0x6A);
    TEST_ASSERT_EQUAL(1, bytes);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_getInstructionBytes_given_two_byte_opcode_expect_size_is_two_byte() {
  int bytes;
  Try{
    bytes = getInstructionBytes(0x6AAB);
    TEST_ASSERT_EQUAL(2, bytes);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_getInstructionBytes_given_three_byte_opcode_expect_size_is_three_byte() {
  int bytes;
  Try{
    bytes = getInstructionBytes(0x6A12BD);
    TEST_ASSERT_EQUAL(3, bytes);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_recordLabel_given_labelinfo_expect_it_is_stored_into_list_correctly() {
  LabelInfo *infoPtr;
  listPtr = linkedListCreateList();
  
  Try{
    //current list is empty
    recordLabel("SKIP", 1, 2);

    infoPtr = listPtr->head->data;
    TEST_ASSERT_NOT_NULL(infoPtr);
    TEST_ASSERT_EQUAL_STRING("SKIP", infoPtr->name);
    TEST_ASSERT_EQUAL(1, infoPtr->indexNo);
    TEST_ASSERT_EQUAL(2, infoPtr->lineNo);
    TEST_ASSERT_EQUAL(1, listPtr->count);

    //current list has one item
    recordLabel("HERE", 4, 9);

    infoPtr = listPtr->head->data;
    TEST_ASSERT_NOT_NULL(infoPtr);
    TEST_ASSERT_EQUAL_STRING("HERE", infoPtr->name);
    TEST_ASSERT_EQUAL(4, infoPtr->indexNo);
    TEST_ASSERT_EQUAL(9, infoPtr->lineNo);
    TEST_ASSERT_EQUAL(2, listPtr->count);
    
    //two items in the list
    TEST_ASSERT_EQUAL_PTR(NULL, listPtr->head->prev);
    TEST_ASSERT_EQUAL_PTR(NULL, listPtr->tail->next);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  linkedListFreeList(listPtr, freeLabelInfo);
}

void test_recordLabel_given_duplicate_label_expect_ERR_DUPLICATE_LABEL_is_thrown() {
  listPtr = linkedListCreateList();

  Try{
    recordLabel("SKIP", 1, 2);
    recordLabel("HERE", 4, 9);
    recordLabel("SKIP", 3, 13);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_DUPLICATE_LABEL, e->errorCode);
    freeException(e);
  }
  linkedListFreeList(listPtr, freeLabelInfo);
}

void test_getLabelIndex_given_label_and_label_exists_in_list_expect_index_of_the_label_is_returned() {
  int index;
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 3, 4};
  LabelInfo info2 = {"END", 5, 6};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);
  
  Try{
    index = getLabelIndex("NOW");
    TEST_ASSERT_EQUAL(3, index);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
}

void test_getLabelIndex_given_label_and_but_label_not_exists_in_list_expect_MINUSone_is_returned() {
  int index;
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 1, 2};
  LabelInfo info2 = {"END", 5, 6};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);
  
  Try{
    index = getLabelIndex("SKIP");
    TEST_ASSERT_EQUAL(-1, index);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
}

void test_getLabelIndex_given_label_and_but_empty_list_expect_MINUSone_is_returned() {
  int index;
  listPtr = linkedListCreateList();
  
  Try{
    index = getLabelIndex("THERE");
    TEST_ASSERT_EQUAL(-1, index);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
}

void test_getRelativeAddress_given_label_as_token_and_label_exists_expect_relative_is_computed_and_returned() {
  int relative;
  Tokenizer *tokenizer = NULL;

  //Set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 1, 2};
  LabelInfo info2 = {"END", 5, 6};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  //label "NOW" is read
  tokenizer = createTokenizer("NOW");
  
  Try{
    relative = getRelativeAddress(tokenizer, 30, -128, 127);
    TEST_ASSERT_EQUAL(-29, relative);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_integer_as_token_expect_relative_is_computed_and_returned() {
  int relative;
  Tokenizer *tokenizer = NULL;

  //if number is directly inputted as relative, the number itself is returned back as relative address
  //provided the number is in range of -127 to 128
  tokenizer = createTokenizer("+100");
  
  Try{
    relative = getRelativeAddress(tokenizer, 0x90A, -128, 127);
    TEST_ASSERT_EQUAL(100, relative);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_integer_as_token_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_is_thrown() {
  int relative;
  Tokenizer *tokenizer = NULL;
  
  tokenizer = createTokenizer("200");

  Try {
    relative = getRelativeAddress(tokenizer, 50, -128, 127);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_non_existent_label_as_token_expect_ERR_UNKNOWN_LABEL_is_thrown() {
  int relative;
  Tokenizer *tokenizer = NULL;

  //Set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 1, 2};
  LabelInfo info2 = {"END", 5, 6};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);
  
  tokenizer = createTokenizer("HERE");

  Try{
    relative = getRelativeAddress(tokenizer, 0x50, -128, 127);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_LABEL, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_label_as_token_but_out_of_branching_range_backwards_branch_expect_ERR_TARGET_OUT_OF_RANGE_is_thrown() {
  int relative;
  Tokenizer *tokenizer = NULL;

  //Set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 50, 100};
  LabelInfo info2 = {"END", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);
  
  tokenizer = createTokenizer("END");

  Try{
    relative = getRelativeAddress(tokenizer, 152, -128, 127);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_TARGET_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_label_as_token_but_out_of_branching_range_forward_branch_expect_ERR_TARGET_OUT_OF_RANGE_is_thrown() {
  int relative;
  Tokenizer *tokenizer = NULL;

  //Set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 150, 200};
  LabelInfo info2 = {"END", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);
  
  tokenizer = createTokenizer("NOW");

  Try{
    relative = getRelativeAddress(tokenizer, 22, -128, 127);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_TARGET_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_getRelativeAddress_given_token_but_is_neither_label_nor_integer_expect_ERR_INVALID_OPERAND_is_thrown() {
  int relative;
  Tokenizer *tokenizer = NULL;
  
  tokenizer = createTokenizer("@r1");

  Try{
    relative = getRelativeAddress(tokenizer, 30, -128, 127);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_getAbsoluteAddress_given_label_as_token_and_label_exists_expect_absolute_is_computed_and_returned() {
  int absolute;
  Tokenizer *tokenizer = NULL;

  //Set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 1, 2};
  LabelInfo info2 = {"HERE", 0x9A9B, 500};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  tokenizer = createTokenizer("HERE");
  
  Try{
    absolute = getAbsoluteAddress(tokenizer, 0, 65535);
    TEST_ASSERT_EQUAL_HEX16(0x9A9B, absolute);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_writeCodeToCodeMemory_given_opcode_0x7B_expect_opcode_stored_in_code_memory_and_return_the_size() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 10;                     //codePtr is pointing at code memory with the abs address 10
  Try{
    len = writeCodeToCodeMemory(0x7B, codePtr);
    TEST_ASSERT_EQUAL(1, len);                            //opcode is 1 byte
    TEST_ASSERT_EQUAL_HEX8(0x7B, codeMemory[10]);         //opcode is stored at address 10
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
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
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[0xAB]);       //opcode is stored at address 0xAB
    TEST_ASSERT_EQUAL_HEX8(0xCD, codeMemory[0xAC]);       //opcode is stored at address 0xAC
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
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
    TEST_ASSERT_EQUAL_HEX8(0x9B, codeMemory[200]);        //opcode is stored at address 200 (little endian, LSB stored at lower address)
    TEST_ASSERT_EQUAL_HEX8(0xA1, codeMemory[201]);        //opcode is stored at address 201
    TEST_ASSERT_EQUAL_HEX8(0x2C, codeMemory[202]);        //opcode is stored at address 202
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_indR0_token_expect_number_extracted_is_0_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int number = 8;
  Try{
    tokenizer = createTokenizer(" @r0 , A");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, number);                         //token is indirect, so number is extracted
    TEST_ASSERT_EQUAL_STRING("," , token->str);           //token is indirect, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                         //token is indirect, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_not_indirect_expect_number_is_not_extracted_and_token_is_pushed_back() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int number = 8;
  Try{
    tokenizer = createTokenizer("r3 #0x55");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(8, number);                         //number is not extracted since token is not indirect
    TEST_ASSERT_EQUAL_STRING("r3", token->str);           //token is not consumed since its not indirect
    TEST_ASSERT_EQUAL(0, isTrue);                         //function returns false since token is not indirect
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_is_indReg_but_invalidReg_expect_exception_ERR_INVALID_REGISTER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int number;
  Try{
    tokenizer = createTokenizer("    @ratatouille ");     //token is indirect but register is invalid
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIndRegisterThenGetItsNumberAndConsume_given_is_indReg_but_outOfRange_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int number;
  Try{
    tokenizer = createTokenizer("  @r3 ");                //token is indirect but register is out of range
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

//@A+DPTR, @A+PC and @DPTR operands are only found in JMP, MOVX and MOVC instructions,
//all other instructions have only @Ri as operands, which also means when @ token is detected,
//automatically the token next to it must be a register,
//this function is not used in assembling JMP, MOVX and MOVC instruction (@Ri is not checked using this function for JMP, MOVX and MOVC)
void test_isIndRegisterThenGetItsNumberAndConsume_given_is_ind_but_not_register_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int number;
  Try{
    tokenizer = createTokenizer("  @hello ");
    int isTrue = isIndRegisterThenGetItsNumberAndConsume(tokenizer, &number);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_negative_imm8bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer("   #-123@123");
    //-123 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -128, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(-123, value);                       //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("@" , token->str);           //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                         //token is immediate, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_imm16bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xB0;
  Try{
    tokenizer = createTokenizer(" #50000 happy");
    //50000 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, -32768, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(50000, value);                      //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("happy" , token->str);       //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                         //token is immediate, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_negative_imm16bit_expect_value_extracted_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
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
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_imm8bit_with_plus_sign_expect_value_extracted_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
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
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_not_imm_expect_value_is_not_extracted_and_token_is_pushed_back() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0x88;
  Try{
    tokenizer = createTokenizer(" 0xA0 haha");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0x88, value);                       //value is not extracted since token is not immediate
    TEST_ASSERT_EQUAL_STRING("0xA0", token->str);         //token is not consumed since its not immediate
    TEST_ASSERT_EQUAL(0, isTrue);                         //function returns false since token is not immediate
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_is_imm_but_outOfRange_expect_exception_ERR_INTEGER_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value;
  Try{
    tokenizer = createTokenizer("#0xFFA  ");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);      //maximum range is set to 0xFF
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

//when token # is detected, it indicates it is immediate,
//automatically the token next to it must be integer
//if it is not integer it will throw exception
void test_isImmediateThenGetsItsValueAndConsume_given_is_imm_but_not_integer_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value;
  Try{
    tokenizer = createTokenizer("  #r7  ");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsImmediateThenGetsItsValueAndConsume_given_is_imm_expect_value_is_extracted_and_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0x70;
  Try{
    tokenizer = createTokenizer("     #-85 hehe");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, -128, 255);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(-85, value);
    TEST_ASSERT_EQUAL_STRING("hehe", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsImmediateThenGetsItsValueAndConsume_given_not_imm_expect_exception_ERR_EXPECTING_IMMEDIATE_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value;
  Try{
    tokenizer = createTokenizer(" @r0  ");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IMMEDIATE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_is_correct_operator_expect_token_is_consumed_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" @r0, r1");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "@");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("r0", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_wrong_operator_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" $hello  ");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "#");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("$", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isOperatorTokenThenConsume_given_not_operator_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" doom  ");
    int isTrue = isOperatorTokenThenConsume(tokenizer, "%");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("doom", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_is_correct_operator_expect_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" +#@ret");
    verifyIsOperatorTokenThenConsume(tokenizer, "+");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("#", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_wrong_operator_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" -A+B  ");
    verifyIsOperatorTokenThenConsume(tokenizer, "=");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsOperatorTokenThenConsume_given_not_operator_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 123456+  654321  ");
    verifyIsOperatorTokenThenConsume(tokenizer, "+");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_is_correct_identifier_expect_token_is_consumed_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" abcd123 x1y2z");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "abcd123");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("x1y2z", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_wrong_identifier_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer("     good bad  ");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "bad");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("good", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIdentifierTokenThenConsume_given_not_identifier_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" 9987@#1  ");
    int isTrue = isIdentifierTokenThenConsume(tokenizer, "byebye321");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("9987", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_is_correct_identifier_expect_token_is_consumed() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" boba isgood2");
    verifyIsIdentifierTokenThenConsume(tokenizer, "boba");
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("isgood2", token->str);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_wrong_identifier_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" McDonalds Or KFC");
    verifyIsIdentifierTokenThenConsume(tokenizer, "KFC");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIdentifierTokenThenConsume_given_not_identifier_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @!# done");
    verifyIsIdentifierTokenThenConsume(tokenizer, "fries");
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_neg_integer_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -32678 6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -32768, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("6699876", token->str);
    TEST_ASSERT_EQUAL(-32678, value);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_integer_with_plus_sign_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" +12345 6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, 0, 65535);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("6699876", token->str);
    TEST_ASSERT_EQUAL(12345, value);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_integer_without_plus_sign_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 120 !6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 127);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("!", token->str);
    TEST_ASSERT_EQUAL(120, value);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_not_integer_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 1000;
  Try{
    tokenizer = createTokenizer(" R3 integer");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 127);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("R3", token->str);
    TEST_ASSERT_EQUAL(1000, value);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_neg_integer_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -129 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_pos_integer_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" +300 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_pos_integer_without_sign_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 300 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_negative_sign_but_not_integer_expect_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" -abcdef xyz  ");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_positive_sign_but_not_integer_expect_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" +number abc  ");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, -128, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIntegerTokenThenConsume_given_is_integer_expect_token_is_consumed_and_value_is_extracted() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 0x789 muthu");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0, 4095);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("muthu", token->str);
    TEST_ASSERT_EQUAL(0x789, value);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsIntegerTokenThenConsume_given_not_integer_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int value;
  Try{
    tokenizer = createTokenizer("  beef  or chicken");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0, 255);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_of_valid_range_expect_token_is_consumed_and_value_is_extracted_and_return_1() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int regNum = 0;
  Try{
    tokenizer = createTokenizer(" r5 C");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("C", token->str);
    TEST_ASSERT_EQUAL(5, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_not_register_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int regNum = 7;
  Try{
    //token is not even an identifier, definitely is not a register
    tokenizer = createTokenizer(" @abcd");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("@", token->str);
    TEST_ASSERT_EQUAL(7, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_identifier_that_not_begin_with_r_expect_token_is_pushed_back_and_return_0() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int regNum = 5;
  Try{
    //token is identifier but do not begin with 'r', indicating it is not register
    tokenizer = createTokenizer(" hexagon A");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("hexagon", token->str);
    TEST_ASSERT_EQUAL(5, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_with_indirect_addressing_mode_but_out_of_range_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int regNum;
  Try{
    tokenizer = createTokenizer(" r5 bb");
    //Addressing mode is indirect addressing, only can receive register R0 and R1
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_is_register_with_register_addressing_mode_but_out_of_range_expect_exception_ERR_REG_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int regNum;
  Try{
    tokenizer = createTokenizer(" r21 bb");
    //Addressing mode is register addressing, only can receive register from R0 to R7
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_REG_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_isRegisterConsumeAndGetItsNumber_given_invalid_register_expect_exception_ERR_INVALID_REGISTER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int regNum;
  Try{
    tokenizer = createTokenizer(" R5starry night");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsRegisterConsumeAndGetItsNumber_given_is_register_of_valid_range_expect_token_is_consumed_and_value_is_extracted() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  int regNum = 5;
  Try{
    tokenizer = createTokenizer(" r1 register");
    verifyIsRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL_STRING("register", token->str);
    TEST_ASSERT_EQUAL(1, regNum);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_verifyIsRegisterConsumeAndGetItsNumber_given_not_register_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Tokenizer *tokenizer = NULL;
  int regNum;
  Try{
    tokenizer = createTokenizer(" 0x13 r2 ");
    verifyIsRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_null_token_expect_no_exception_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("     ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_newline_token_expect_no_exception_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("\n");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_semicolon_token_expect_no_exception_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  ;this is a comment   ");
    checkExtraToken(tokenizer);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_not_whitespace_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" A");
    checkExtraToken(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_checkExtraToken_given_extra_token_after_tab_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Try{
    //consider this scenario, "mov a, r0\t\tHELLO"
    //tokenizer skip whitespaces, in this case it skip the tab
    //when get token, token 'HELLO' will be fetched
    //thus it is considered an extra token
    tokenizer = createTokenizer("  \t\tHELLO   ");
    checkExtraToken(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_3668_expect_number_3668_is_extracted() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer("tree3668");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 4);
    TEST_ASSERT_EQUAL(3668, num);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_1tree_expect_MINUSone_is_returned() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" R1tree");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1);
    TEST_ASSERT_EQUAL(-1, num);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_t321_expect_MINUSone_is_returned() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" Rt321");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1);
    TEST_ASSERT_EQUAL(-1, num);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_extractNum_given_string_null_expect_MINUSone_is_returned() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer("R");
    token = getToken(tokenizer);
    int num = extractNum(token->str + 1);
    TEST_ASSERT_EQUAL(-1, num);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

//just to test exception throwing
void test_throwUnsupportedOperandException_given_token_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  Token *token = NULL;
  Try{
    tokenizer = createTokenizer(" hi");
    token = getToken(tokenizer);
    throwUnsupportedOperandException(token);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  }Catch(e) {
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeToken(token);
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithIndirect_expect_opcode_0x46() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("    A, @r0");
    //0x40 is the first nibble of the opcode which is passed into the function from the table
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_ASSERT_EQUAL_HEX8(0x46, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithRegister_expect_opcode_0x9E() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("a    , r6");
    opcode = assembleAWithOperands(tokenizer, 0x90, A_REG);
    TEST_ASSERT_EQUAL_HEX8(0x9E, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithImmediate_expect_opcode_0x84C0() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("a,    #0xC0");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_IMM);
    TEST_ASSERT_EQUAL_HEX16(0x84C0, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_AWithDirect_expect_opcode_0x7538() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, 0x38");
    opcode = assembleAWithOperands(tokenizer, 0x70, A_DIR);
    TEST_ASSERT_EQUAL_HEX16(0x7538, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a r3");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_REG);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_first_operand_not_Acc_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x56 ,@r1");
    opcode = assembleAWithOperands(tokenizer, 0x80, A_REG);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_a_with_dir_but_flag_is_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , 0x54 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_a_with_reg_but_flag_is_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , R4 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_a_with_ind_but_flag_is_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , @R1 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_a_with_imm_but_flag_is_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , #56 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_REG);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_indirect_but_not_register_expect_exception_ERR_EXPECTING_REGISTER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A , @0x55 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a     , HELLO ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_ind_aPlusDPTR_expect_exception_ERR_EXPECTING_REGISTER_is_thrown() {
  Tokenizer *tokenizer = NULL;
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
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAWithOperands_given_extra_operand_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" a , @r0 r1 ");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_DirectWithImm_expect_opcode_0x93DEAA() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xDE, #0xAA");
    opcode = assembleDirectWithOperands(tokenizer, 0x90, DIR_IMM);
    TEST_ASSERT_EQUAL(0x93DEAA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_DirectWithA_expect_opcode_0x7294() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x94 , a");
    opcode = assembleDirectWithOperands(tokenizer, 0x70, DIR_A);
    TEST_ASSERT_EQUAL_HEX16(0x7294, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_dir_with_imm_but_flag_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0xD2, #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_dir_with_A_but_flag_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x55, A ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_first_operand_not_direct_expect_exception_ERR_EXPECTING_INTEGER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" A, #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_correct_operands_but_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x88 #98 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x88, ferrari789 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_IMM);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_correct_operands_but_with_extra_parameter_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" 0x88, A   R3");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_dir_with_dir_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    //although operands like "dir, dir" is valid, but it is not assembled in this function,
    //it is the same case as the assembleAWithOperands function
    //this function is used just for assembling common operands (dir, A; dir, Imm)
    //and are used by instruction such as (anl, xrl, subb, ...)
    tokenizer = createTokenizer(" 0xD2, 0xB3 ");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIR_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_CWithBarBit_expect_opcode_0xD0CA() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("C, /0xCA");
    opcode = assembleCWithOperands(tokenizer, 0xA0, C_BARBIT);
    TEST_ASSERT_EQUAL_HEX16(0xD0CA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_CWithBit_expect_opcode_0x829B() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer("C, 0x9B");
    opcode = assembleCWithOperands(tokenizer, 0x80, C_BIT);
    TEST_ASSERT_EQUAL_HEX16(0x829B, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_c_barbit_but_flag_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C, /0xBB ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_c_bit_but_flag_not_set_expect_exception_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" c , 0x12 ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BARBIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_first_operand_not_C_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" R1, /0xBB ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BARBIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_correct_operands_but_missing_comma_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C  0x30 ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C, @R0 ");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCWithOperands_given_correct_operands_but_with_extra_parameter_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
  Tokenizer *tokenizer = NULL;
  int opcode;
  Try{
    tokenizer = createTokenizer(" C, 0x30 r3");
    opcode = assembleCWithOperands(tokenizer, 0x40, C_BIT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_A_with_R3_expect_opcode_0xEB_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("A  , r3");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xEB, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_A_with_0x56_expect_opcode_0xE556_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , 0x56");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xE5, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x56, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_A_with_indR0_expect_opcode_0xE6_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , @R0 ;this is a comment ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xE6, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_A_with_imm0xAB_expect_opcode_0x74AB_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , #0xAB");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x74, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_R7_with_A_expect_opcode_0xFF_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  r7 , a ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xFF, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_R0_with_0xBA_expect_opcode_0xA8BA_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  r0 , 0xbA   ;COMMENT! ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA8, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBA, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_R6_with_immMINUS120_expect_opcode_0xA8BA_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  R6   , #-120 ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x7E, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x88, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_0xFA_with_A_expect_opcode_0xF5FA_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0xFA   , a; comment here ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xF5, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xFA, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_0x5B_with_R7_expect_opcode_0x8F5B_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0x5B   , r7; comment here ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x8F, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x5B, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_0x90_with_0xB1_expect_opcode_0x85B190_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0x90   , 0xB1");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x85, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xB1, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x90, codeMemory[2]);
    TEST_ASSERT_EQUAL(3, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_0xA0_with_indR1_expect_opcode_0x87A0_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0xA0   , @r1  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x87, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xA0, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_0x12_with_imm156_expect_opcode_0x75129C_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0x12   , #156  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x75, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x12, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0x9C, codeMemory[2]);
    TEST_ASSERT_EQUAL(3, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_indR0_with_a_expect_opcode_0xF6_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @r0   , a  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF6, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_indR1_with_0xBB_expect_opcode_0xA7BB_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @R1   , 0xBb  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA7, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xBB, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_indR1_with_immPLUS100_expect_opcode_0x7764_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @r1   , #+100 ;  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x77, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x64, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_C_with_directBitAddr_expect_opcode_0xA22F_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  c   , 0x2F ;  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA2, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x2F, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_directBitAddr_with_C_expect_opcode_0x922F_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0x2F  , C  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x92, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x2F, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_DPTR_with_imm16bit_expect_opcode_0x90DCBA_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  dPtr  , #0xDCBA  ");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x90, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xDC, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0xBA, codeMemory[2]);
    TEST_ASSERT_EQUAL(3, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_A_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  A, Abu");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_reg_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  R6 , DPTR");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_dir_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  0x66 , AB");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_ind_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @r1 , R5");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_c_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  c , A");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_DPTR_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  DPTR , @r1");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IMMEDIATE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_correct_operands_but_no_comma_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  DPTR  #0xABCD");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVInstruction_given_invalid_first_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  hi, r3");
    len = assembleMOVInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_A_with_indR1_expect_opcode_0xE3_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , @R1  ");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xE3, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_A_with_indDPTR_expect_opcode_0xE0_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , @dpTR  ");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xE0, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_indR0_with_A_expect_opcode_0xF2_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @r0 , a  ");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF2, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_indDPTR_with_A_expect_opcode_0xF0_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  @DPtR , a  ");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF0, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_invalid_first_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  R3, A");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_A_with_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  A , 0x56");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_ind_but_is_not_dptr_or_reg_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @money , #63");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_ind_with_invalid_second_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @dptr , #63");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVXInstruction_given_valid_operand_but_no_comma_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @dptr  A");
    len = assembleMOVXInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVCInstruction_given_A_with_indAplusDPTR_expect_opcode_0x93_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  a  , @A +dpTR  ");
    len = assembleMOVCInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x93, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVCInstruction_given_A_with_indAplusPC_expect_opcode_0x83_stored_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  A  , @A + pC  ");
    len = assembleMOVCInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x83, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVCInstruction_given_A_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" a , R3");
    len = assembleMOVCInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVCInstruction_given_invalid_first_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0x65 , @A+DPTR");
    len = assembleMOVCInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleMOVCInstruction_given_valid_operand_but_no_comma_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" A  @A+PC");
    len = assembleMOVCInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstructionWithOnlyAccAsFirstOperand_given_instruction_xchd_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"xchd", assembleInstructionWithOnlyAccAsFirstOperand, {0xD0, A_IND}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 23;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("A  , @r1");
    len = assembleInstructionWithOnlyAccAsFirstOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xD7, codeMemory[23]);
    TEST_ASSERT_EQUAL(24, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstructionWithOnlyAccAsFirstOperand_given_instruction_subb_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"subb", assembleInstructionWithOnlyAccAsFirstOperand,
  {0x90, A_DIR | A_IMM | A_IND | A_REG}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 5656;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" a  , 0x86 ;comment");
    len = assembleInstructionWithOnlyAccAsFirstOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x95, codeMemory[5656]);
    TEST_ASSERT_EQUAL_HEX8(0x86, codeMemory[5657]);
    TEST_ASSERT_EQUAL(5658, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleLogicalInstructionWithoutXRL_given_instruction_orl_C_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"orl", assembleLogicalInstructionWithoutXRL,
  {0x40, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" C , /0x43  ");
    len = assembleLogicalInstructionWithoutXRL(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA0, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x43, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleLogicalInstructionWithoutXRL_given_instruction_anl_A_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"anl", assembleLogicalInstructionWithoutXRL,
  {0x50, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" a, @r1  ");
    len = assembleLogicalInstructionWithoutXRL(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x57, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleLogicalInstructionWithoutXRL_given_instruction_anl_dir_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"anl", assembleLogicalInstructionWithoutXRL,
  {0x50, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0x56, #200  ");
    len = assembleLogicalInstructionWithoutXRL(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x53, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0x56, codeMemory[1]);
    TEST_ASSERT_EQUAL_HEX8(0xC8, codeMemory[2]);
    TEST_ASSERT_EQUAL(3, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleLogicalInstructionWithoutXRL_given_invalid_first_operand_expect_ERR_INVALID_OPERAND_is_thrown() {
  _8051Instructions table = {"anl", assembleLogicalInstructionWithoutXRL,
  {0x50, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" R3 , #-109  ");
    len = assembleLogicalInstructionWithoutXRL(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleXRLinstruction_given_instruction_xrl_dir_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"xrl", assembleXRLinstruction,
  {0x60, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2000;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0xBA , #-120  ");
    len = assembleXRLinstruction(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x63, codeMemory[2000]);
    TEST_ASSERT_EQUAL_HEX8(0xBA, codeMemory[2001]);
    TEST_ASSERT_EQUAL_HEX8(0x88, codeMemory[2002]);
    TEST_ASSERT_EQUAL(2003, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleXRLinstruction_given_instruction_xrl_A_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"xrl", assembleXRLinstruction,
  {0x60, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2000;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" A , #+56  ");
    len = assembleXRLinstruction(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x64, codeMemory[2000]);
    TEST_ASSERT_EQUAL_HEX8(0x38, codeMemory[2001]);
    TEST_ASSERT_EQUAL(2002, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleXRLinstruction_given_invalid_first_operand_expect_ERR_INVALID_OPERAND_is_thrown() {
  _8051Instructions table = {"xrl", assembleXRLinstruction,
  {0x60, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" C , 0x88  ");
    len = assembleXRLinstruction(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleJMPInstruction_given_ind_APlusDPTR_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @a + dPTr ; comment  ");
    len = assembleJMPInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x73, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleJMPInstruction_given_ind_APlusDPTR_with_extra_token_expect_ERR_EXTRA_PARAMETER_is_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @a + dPTr   EXTRA");
    len = assembleJMPInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleJMPInstruction_given_invalid_operand_expect_ERR_INVALID_OPERAND_is_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @a + PC ; comment  ");
    len = assembleJMPInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jb_bit_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  _8051Instructions table = {"jb", assembleBitWithRel, {0x20, 0}};
  int len;
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer *tokenizer = NULL;
  //second pass (Label is only assembled in second pass)
  muteOnNoLabel = 0;

  //Set up labels in linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"REPEAT", 200, 100};
  LabelInfo info2 = {"HERE", 2, 4};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" 0x5B, REPEAT ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x20, codeMemory[100]);
    TEST_ASSERT_EQUAL_HEX8(0x5B, codeMemory[101]);
    TEST_ASSERT_EQUAL_HEX8(0x61, codeMemory[102]);
    TEST_ASSERT_EQUAL(103, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jbc_bit_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  _8051Instructions table = {"jbc", assembleBitWithRel, {0x10, 0}};
  int len;
  uint8_t *codePtr = codeMemory + 400;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"REPEAT", 20, 10};
  LabelInfo info2 = {"LOOP", 275, 150};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" 0xDA, LOOP ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x10, codeMemory[400]);
    TEST_ASSERT_EQUAL_HEX8(0xDA, codeMemory[401]);
    TEST_ASSERT_EQUAL_HEX8(0x80, codeMemory[402]);
    TEST_ASSERT_EQUAL(403, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jnb_bit_with_label_on_first_pass_expect_it_is_assembled_with_relative_address_left_blank_and_written_into_codeMemory() {
  _8051Instructions table = {"jnb", assembleBitWithRel, {0x30, 0}};
  int len;
  uint8_t *codePtr = codeMemory + 500;
  Tokenizer *tokenizer = NULL;
  //at first pass, opcode for relative is left blank, as only label recording is done at first pass
  muteOnNoLabel = 1;

  Try{
    tokenizer = createTokenizer(" 0x56, DELAY ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x30, codeMemory[500]);
    TEST_ASSERT_EQUAL_HEX8(0x56, codeMemory[501]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[502]);      //relative address left blank at first pass
    TEST_ASSERT_EQUAL(503, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jbc_but_first_operand_is_NOT_integer_expect_ERR_EXPECTING_INTEGER_is_thrown() {
  _8051Instructions table = {"jbc", assembleBitWithRel, {0x10, 0}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" A, +12 ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jnb_valid_operands_but_no_comma_expect_ERR_INVALID_OPERAND_is_thrown() {
  _8051Instructions table = {"jnb", assembleBitWithRel, {0x30, 0}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  
  Try{
    tokenizer = createTokenizer("  0x56 -100 ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleBitWithRel_given_jnb_valid_operands_but_with_extra_token_expect_ERR_EXTRA_PARAMETER_is_thrown() {
  _8051Instructions table = {"jnb", assembleBitWithRel, {0x30, 0}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  //extra token is only checked at second pass
  muteOnNoLabel = 0;
  
  Try{
    tokenizer = createTokenizer("  0x56, -100 TURING ");
    len = assembleBitWithRel(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_reg_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 0x3D;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"AGAIN", 54, 30};
  LabelInfo info2 = {"LOOP", 275, 150};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" r7, AGAIN ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xDF, codeMemory[0x3D]);
    TEST_ASSERT_EQUAL_HEX8(0xF7, codeMemory[0x3E]);
    TEST_ASSERT_EQUAL(0x3F, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_direct_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 0x95;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"DELAY", 0x79, 100};
  LabelInfo info2 = {"LOOP", 275, 150};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" 0x5B, DELAY ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xD5, codeMemory[0x95]);
    TEST_ASSERT_EQUAL_HEX8(0x5B, codeMemory[0x96]);
    TEST_ASSERT_EQUAL_HEX8(0xE1, codeMemory[0x97]);
    TEST_ASSERT_EQUAL(0x98, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_reg_with_label_on_first_pass_expect_it_is_assembled_with_relative_address_left_blank_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 199;
  Tokenizer *tokenizer = NULL;
  //at first pass, opcode for relative is left blank, as only label recording is done at first pass
  muteOnNoLabel = 1;

  Try{
    tokenizer = createTokenizer(" r5, NOW ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xDD, codeMemory[199]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[200]);      //relative address left blank at first pass
    TEST_ASSERT_EQUAL(201, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_invalid_first_operand_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" A , DELAY ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_missing_comma_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" R0  DELAY\n ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDJNZInstruction_given_extra_parameter_expect_ERR_EXTRA_PARAMETER_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"DELAY", 0x79, 100};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" R1, DELAY AB ");
    len = assembleDJNZInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_A_DIR_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 0x0D;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"STOP", 0x58, 50};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" a, 0x6F, STOP ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xB5, codeMemory[0x0D]);
    TEST_ASSERT_EQUAL_HEX8(0x6F, codeMemory[0x0E]);
    TEST_ASSERT_EQUAL_HEX8(0x48, codeMemory[0x0F]);
    TEST_ASSERT_EQUAL(0x10, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_A_IMM_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 0xE5;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"HALT", 0xDD, 170};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" a, #-55, HALT ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xB4, codeMemory[0xE5]);
    TEST_ASSERT_EQUAL_HEX8(0xC9, codeMemory[0xE6]);
    TEST_ASSERT_EQUAL_HEX8(0xF5, codeMemory[0xE7]);
    TEST_ASSERT_EQUAL(0xE8, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_reg_imm_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 0xAA;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"DELAY2", 0x99, 100};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" R7, #99, DELAY2 ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xBF, codeMemory[0xAA]);
    TEST_ASSERT_EQUAL_HEX8(0x63, codeMemory[0xAB]);
    TEST_ASSERT_EQUAL_HEX8(0xEC, codeMemory[0xAC]);
    TEST_ASSERT_EQUAL(0xAD, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_ind_imm_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 120;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NEXT", 43, 100};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" @r1, #+200, NEXT ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xB7, codeMemory[120]);
    TEST_ASSERT_EQUAL_HEX8(0xC8, codeMemory[121]);
    TEST_ASSERT_EQUAL_HEX8(0xB0, codeMemory[122]);
    TEST_ASSERT_EQUAL(123, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_ind_imm_with_label_on_first_pass_expect_it_is_assembled_with_relative_address_left_blank_and_written_into_codeMemory() {
  int len;
  uint8_t *codePtr = codeMemory + 120;
  Tokenizer *tokenizer = NULL;
  //at first pass, opcode for relative is left blank, as only label recording is done at first pass
  muteOnNoLabel = 1;

  Try{
    tokenizer = createTokenizer(" @r1, #200, NEXT ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0xB7, codeMemory[120]);
    TEST_ASSERT_EQUAL_HEX8(0xC8, codeMemory[121]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[122]);      //relative address left blank at first pass
    TEST_ASSERT_EQUAL(123, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_invalid_first_operand_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" DPTR, #20, HERE ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_missing_second_comma_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" A, 0x55 HERE ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_missing_first_comma_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;

  Try{
    tokenizer = createTokenizer(" R3 0x55, THERE ");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleCJNEInstruction_given_extra_parameter_expect_ERR_EXTRA_PARAMETER_to_be_thrown() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"DELAY", 0x79, 100};
  ListItem item1 = {NULL, NULL, &info1};
  linkedListAddItemToHead(&item1, listPtr);

  Try{
    tokenizer = createTokenizer(" A, #0xE1, DELAY R5");
    len = assembleCJNEInstruction(tokenizer, NULL, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_jz_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  _8051Instructions table = {"jz", assembleSingleOperandWithLabel,
  {0x60, OPERAND_REL}};
  int len;
  uint8_t *codePtr = codeMemory + 3;
  Tokenizer *tokenizer = NULL;
  //set to second pass (assembling is done at second pass, first pass is just to record the label into list)
  muteOnNoLabel = 0;

  //set up linked list
  listPtr = linkedListCreateList();
  LabelInfo info1 = {"NOW", 50, 100};
  LabelInfo info2 = {"END", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer("NOW ;comment ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x60, codeMemory[3]);
    TEST_ASSERT_EQUAL_HEX8(0x2D, codeMemory[4]);
    TEST_ASSERT_EQUAL(5, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_acall_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  _8051Instructions table = {"acall", assembleSingleOperandWithLabel,
  {0x11, OPERAND_DIR11}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 50;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"WHERE", 0x51A, 100};
  LabelInfo info2 = {"HERE", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" WHERE ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xB1, codeMemory[50]);
    TEST_ASSERT_EQUAL_HEX8(0x1A, codeMemory[51]);
    TEST_ASSERT_EQUAL(52, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_ljmp_with_label_and_label_exists_expect_it_is_assembled_and_written_into_codeMemory() {
  _8051Instructions table = {"ljmp", assembleSingleOperandWithLabel,
  {0x02, OPERAND_DIR16}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"LOOP", 0x678A, 200};
  LabelInfo info2 = {"HERE", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" LOOP ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x02, codeMemory[100]);
    TEST_ASSERT_EQUAL_HEX8(0x67, codeMemory[101]);
    TEST_ASSERT_EQUAL_HEX8(0x8A, codeMemory[102]);
    TEST_ASSERT_EQUAL(103, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_lcall_with_label_on_first_pass_expect_it_is_assembled_with_relative_address_left_blank_and_written_into_codeMemory() {
  _8051Instructions table = {"lcall", assembleSingleOperandWithLabel, {0x10, OPERAND_DIR16}};
  int len;
  uint8_t *codePtr = codeMemory + 250;
  Tokenizer *tokenizer = NULL;
  //at first pass, opcode for relative is left blank, as only label recording is done at first pass
  muteOnNoLabel = 1;

  Try{
    tokenizer = createTokenizer(" LOOP ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x12, codeMemory[250]);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[251]);      //relative address left blank at first pass
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[252]);      //relative address left blank at first pass
    TEST_ASSERT_EQUAL(253, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_ajmp_with_label_but_with_extra_parameter_expect_ERR_EXTRA_PARAMETER_is_thrown() {
  _8051Instructions table = {"ajmp", assembleSingleOperandWithLabel,
  {0x01, OPERAND_DIR11}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"LOOP", 0x678A, 200};
  LabelInfo info2 = {"HERE", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    tokenizer = createTokenizer(" HERE R1 ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperandWithLabel_given_ajmp_A_expect_ERR_UNKNOWN_LABEL_is_thrown() {
  _8051Instructions table = {"ajmp", assembleSingleOperandWithLabel,
  {0x01, OPERAND_DIR11}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer *tokenizer = NULL;
  muteOnNoLabel = 0;

  listPtr = linkedListCreateList();
  LabelInfo info1 = {"LOOP", 0x678A, 200};
  LabelInfo info2 = {"HERE", 23, 50};
  ListItem item1 = {NULL, NULL, &info1};
  ListItem item2 = {NULL, NULL, &info2};
  linkedListAddItemToHead(&item1, listPtr);
  linkedListAddItemToHead(&item2, listPtr);

  Try{
    //A is considered a label here, as absolute branch instructions can only accept label or integer
    //thus, this A considered an invalid label
    tokenizer = createTokenizer(" A ");
    len = assembleSingleOperandWithLabel(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNKNOWN_LABEL, e->errorCode);
    freeException(e);
  }
  free(listPtr);
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_ind_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"inc", assembleSingleOperand,
  {0x00, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND | OPERAND_DPTR}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2123;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @r1 ; comment ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x07, codeMemory[2123]);
    TEST_ASSERT_EQUAL(2124, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_ind_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"da", assembleSingleOperand, {0xD0, OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" @r0  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_reg_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"dec", assembleSingleOperand,
  {0x10, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" r7 ; ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x1F, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_reg_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"div", assembleSingleOperand, {0x80, OPERAND_AB}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" r5  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_A_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"swap", assembleSingleOperand,
  {0xC0, OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" A  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xC4, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_A_for_rotate_instructions_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"rrc", assembleSingleOperand,
  {0x10, OPERAND_A_ROT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" a  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x13, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_A_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"div", assembleSingleOperand, {0x80, OPERAND_AB}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" a  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_AB_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"mul", assembleSingleOperand,
  {0xA0, OPERAND_AB}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" aB  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xA4, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_AB_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"cpl", assembleSingleOperand, {0xF0, OPERAND_C | OPERAND_BIT | OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" AB  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_C_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"clr", assembleSingleOperand, {0xE0, OPERAND_C | OPERAND_BIT | OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" c  ;comment  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xC3, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_C_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"push", assembleSingleOperand, {0xC0, OPERAND_DIR_STACK}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" C ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_DPTR_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"inc", assembleSingleOperand,
  {0x00, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND | OPERAND_DPTR}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" dPtr  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xA3, codeMemory[2]);
    TEST_ASSERT_EQUAL(3, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_DPTR_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_is_thrown() {
  _8051Instructions table = {"da", assembleSingleOperand, {0xD0, OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" DPTR ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_direct_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"dec", assembleSingleOperand,
  {0x10, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0xAB  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x15, codeMemory[0]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[1]);
    TEST_ASSERT_EQUAL(2, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_direct_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_to_be_thrown() {
  _8051Instructions table = {"dec", assembleSingleOperand,
  {0x10, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0xABC  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_bit_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"setb", assembleSingleOperand,
  {0x90, OPERAND_C | OPERAND_BIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 200;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0x2F  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xD2, codeMemory[200]);
    TEST_ASSERT_EQUAL_HEX8(0x2F, codeMemory[201]);
    TEST_ASSERT_EQUAL(202, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_bit_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_to_be_thrown() {
  _8051Instructions table = {"setb", assembleSingleOperand,
  {0x90, OPERAND_C | OPERAND_BIT}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0xDEF  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_dir_for_stack_instructions_expect_it_is_assembled_correctly() {
  _8051Instructions table = {"pop", assembleSingleOperand, {0xD0, OPERAND_DIR_STACK}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1979;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0xC0  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xD0, codeMemory[1979]);
    TEST_ASSERT_EQUAL_HEX8(0xC0, codeMemory[1980]);
    TEST_ASSERT_EQUAL(1981, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    freeException(e);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_dir_for_stack_instructions_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_to_be_thrown() {
  _8051Instructions table = {"pop", assembleSingleOperand, {0xD0, OPERAND_DIR_STACK}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0x100  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_operand_dir_but_flag_not_set_expect_ERR_UNSUPPORTED_OPERAND_to_be_thrown() {
  _8051Instructions table = {"da", assembleSingleOperand, {0xD0, OPERAND_A}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" 0x56  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_UNSUPPORTED_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleSingleOperand_given_invalid_single_operand_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  _8051Instructions table = {"pop", assembleSingleOperand, {0xD0, OPERAND_DIR_STACK}};
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer(" zzzSleeping  ");
    len = assembleSingleOperand(tokenizer, &table, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

//sample tests
void test_assembleInstruction_given_add_A_with_r7_expect_opcode_0x2f() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("add A, R7");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x2f, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_direct_0xba_expect_opcode_0x25ba() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 100;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("add A, 0xBA");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x25, codeMemory[100]);
    TEST_ASSERT_EQUAL_HEX8(0xBA, codeMemory[101]);
    TEST_ASSERT_EQUAL(102, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_dir_with_imm_expect_opcode_0x75669A() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 250;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("Mov 0x66, #0x9A");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x75, codeMemory[250]);
    TEST_ASSERT_EQUAL_HEX8(0x66, codeMemory[251]);
    TEST_ASSERT_EQUAL_HEX8(0x9A, codeMemory[252]);
    TEST_ASSERT_EQUAL(253, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_R0_expect_opcode_0x26() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 55;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("add A, @R0");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x26, codeMemory[55]);
    TEST_ASSERT_EQUAL(56, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_movx_indirect_with_A_expect_opcode_0xf3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 123;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("MovX @r1 ,a");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xf3, codeMemory[123]);
    TEST_ASSERT_EQUAL(124, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_movc_a_with_aDPTR_expect_opcode_0x93() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x6A;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("MOvc a , @A+dPtR");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x93, codeMemory[0x6A]);
    TEST_ASSERT_EQUAL(0x6B, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_c_with_bit_expect_opcode_0xA27B() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xBFCC;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("mOv c, 0x7b");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA2, codeMemory[0xBFCC]);
    TEST_ASSERT_EQUAL_HEX8(0x7B, codeMemory[0xBFCD]);
    TEST_ASSERT_EQUAL(0xBFCE, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_indirect_with_direct_expect_opcode_0xA7BA() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x65A;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("mOv @r1  , 0xBA  ;this will be seen as comment");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xA7, codeMemory[0x65A]);
    TEST_ASSERT_EQUAL_HEX8(0xBA, codeMemory[0x65B]);
    TEST_ASSERT_EQUAL(0x65C, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_reg_with_imm_expect_opcode_0x78dd() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xD3D;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("mOv r0  , #0xDD");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0x78, codeMemory[0xD3D]);
    TEST_ASSERT_EQUAL_HEX8(0xDD, codeMemory[0xD3E]);
    TEST_ASSERT_EQUAL(0xD3F, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_dptr_with_imm_expect_opcode_0x90EEFF() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xFFE;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("mOv dPtR  , #0xEEFF");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x90, codeMemory[0xFFE]);
    TEST_ASSERT_EQUAL_HEX8(0xEE, codeMemory[0xFFF]);
    TEST_ASSERT_EQUAL_HEX8(0xFF, codeMemory[0x1000]);
    TEST_ASSERT_EQUAL(0x1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_xch_a_with_reg_expect_opcode_0xC9() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xFF;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("   xCh a ,r1");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xC9, codeMemory[0xFF]);
    TEST_ASSERT_EQUAL(0x100, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_xchd_a_with_indirect_reg_expect_opcode_0xD7() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xCD;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("   xChD   a ,   @R1");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xD7, codeMemory[0xCD]);
    TEST_ASSERT_EQUAL(0xCE, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_swap_a_expect_opcode_0xC4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 5321;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  sWAp A ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xC4, codeMemory[5321]);
    TEST_ASSERT_EQUAL(5322, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_DA_a_expect_opcode_0xD4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  da a ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xD4, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_MUL_AB_expect_opcode_0xA4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xAA;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  mUL   Ab  ;multiply a with b ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xA4, codeMemory[0xAA]);
    TEST_ASSERT_EQUAL(0xAB, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_DIV_AB_expect_opcode_0x84() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x99;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  dIv   aB ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x84, codeMemory[0x99]);
    TEST_ASSERT_EQUAL(0x9A, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_setb_C_expect_opcode_0xD3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 21;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  sEtb c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xD3, codeMemory[21]);
    TEST_ASSERT_EQUAL(22, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_setb_bit_expect_opcode_0xD2B1() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 999;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  SetB  0xB1  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xD2, codeMemory[999]);
    TEST_ASSERT_EQUAL_HEX8(0xB1, codeMemory[1000]);
    TEST_ASSERT_EQUAL(1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_A_expect_opcode_0xE4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  cLr a  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xE4, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_C_expect_opcode_0xC3() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1000;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  clr c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xC3, codeMemory[1000]);
    TEST_ASSERT_EQUAL(1001, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_clr_bit_expect_opcode_0xC294() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 2;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  clr 0x94  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xC2, codeMemory[2]);
    TEST_ASSERT_EQUAL_HEX8(0x94, codeMemory[3]);
    TEST_ASSERT_EQUAL(4, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_A_expect_opcode_0xF4() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 3003;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  cpL A  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF4, codeMemory[3003]);
    TEST_ASSERT_EQUAL(3004, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_C_expect_opcode_0x84() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 4321;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  cpl c  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xB3, codeMemory[4321]);
    TEST_ASSERT_EQUAL(4322, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_cpl_bit_expect_opcode_0xB2DD() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 56789;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  cpl 0xDD  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xB2, codeMemory[56789]);
    TEST_ASSERT_EQUAL_HEX8(0xDD, codeMemory[56790]);
    TEST_ASSERT_EQUAL(56791, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ajmp_addr11_expect_opcode_0xC1AB() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0xBFF;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  aJMp 0x6AB  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xC1, codeMemory[0xBFF]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[0xC00]);
    TEST_ASSERT_EQUAL(0xC01, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_acall_addr11_expect_opcode_0xC1AB() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 0x765;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  aCAll 0x79a  ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(2, len);
    TEST_ASSERT_EQUAL_HEX8(0xF1, codeMemory[0x765]);
    TEST_ASSERT_EQUAL_HEX8(0x9A, codeMemory[0x766]);
    TEST_ASSERT_EQUAL(0x767, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_LCALL_addr16_expect_opcode_0x12CD0A() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 800;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  lCaLL  0xCD0A ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x12, codeMemory[800]);
    TEST_ASSERT_EQUAL_HEX8(0xCD, codeMemory[801]);
    TEST_ASSERT_EQUAL_HEX8(0x0A, codeMemory[802]);
    TEST_ASSERT_EQUAL(803, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_LJMP_addr16_expect_opcode_0x02ABCD() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 1000;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  LjmP   0xABcd ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(3, len);
    TEST_ASSERT_EQUAL_HEX8(0x02, codeMemory[1000]);
    TEST_ASSERT_EQUAL_HEX8(0xAB, codeMemory[1001]);
    TEST_ASSERT_EQUAL_HEX8(0xCD, codeMemory[1002]);
    TEST_ASSERT_EQUAL(1003, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ret_expect_opcode_0x22_written_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory + 8888;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  ReT ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x22, codeMemory[8888]);
    TEST_ASSERT_EQUAL(8889, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_nop_expect_opcode_0x00_written_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  nOp ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x00, codeMemory[0]);
    TEST_ASSERT_EQUAL(0x01, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_reti_expect_opcode_0x32_written_in_code_memory() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  rETi    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0x32, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_first_token_not_identifier_expect_ERR_EXPECTING_IDENTIFIER_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  !!XYZ: cpl a    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_instruction_expect_ERR_INVALID_INSTRUCTION_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  LEA 0x56    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_instruction_mnemonic_used_as_label_expect_ERR_ILLEGAL_LABEL_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    tokenizer = createTokenizer("  MOV: ADD A, R0    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_ILLEGAL_LABEL, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_two_label_is_used_in_one_instruction_expect_ERR_INVALID_INSTRUCTION_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    //in this case, AGAIN is seen as an invalid instruction, because token after the first colon is always an instruction mnemonic
    tokenizer = createTokenizer("  START: AGAIN: ADD A, R0    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_colon_after_an_instruction_expect_ERR_INVALID_OPERAND_to_be_thrown() {
  int len;
  uint8_t codeMemory[65536];
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  Try{
    //in this case, ADD is a valid instruction, the colon after it is considered an invalid operand
    //ADD is not seen as a label here, thus illegal label is definitely not thrown
    tokenizer = createTokenizer("  AGAIN: ADD: A, R0    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
    freeException(e);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_instruction_with_label_on_first_pass_expect_label_is_recorded_to_list_and_instruction_is_assembled() {
  int len;
  LabelInfo *infoPtr = NULL;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  listPtr = linkedListCreateList();
  //on the first pass, recording of the label is done
  muteOnNoLabel = 1;
  //this instruction is assumed as in line 1
  lineNumber = 1;
  Try{
    tokenizer = createTokenizer("  START: CPL A    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF4, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
    
    infoPtr = listPtr->head->data;
    TEST_ASSERT_EQUAL_STRING("START", infoPtr->name);
    TEST_ASSERT_EQUAL(0, infoPtr->indexNo);
    TEST_ASSERT_EQUAL(1, infoPtr->lineNo);
    TEST_ASSERT_EQUAL(1, listPtr->count);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  linkedListFreeList(listPtr, freeLabelInfo);
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_instruction_with_label_on_second_pass_expect_no_recording_of_label_is_done_and_instruction_is_assembled() {
  int len;
  uint8_t *codePtr = codeMemory;
  Tokenizer *tokenizer = NULL;
  listPtr = linkedListCreateList();
  //on the second pass, no recording of label is done
  muteOnNoLabel = 0;

  Try{
    tokenizer = createTokenizer("  START: CPL A    ");
    len = assembleInstruction(tokenizer, &codePtr);
    TEST_ASSERT_EQUAL(1, len);
    TEST_ASSERT_EQUAL_HEX8(0xF4, codeMemory[0]);
    TEST_ASSERT_EQUAL(1, getCurrentAbsoluteAddr());
    
    //label is not recorded on second pass, list is empty
    TEST_ASSERT_NULL(listPtr->head);
    TEST_ASSERT_NULL(listPtr->tail);
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  free(listPtr);
  freeTokenizer(tokenizer);
}