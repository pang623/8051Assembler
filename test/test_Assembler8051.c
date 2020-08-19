#include "unity.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include <stdio.h>
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

void test_assembleAWithOperands_given_AWithIndirect_expect_opcode_0x47() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, @r1");
    opcode = assembleAWithOperands(tokenizer, 0x40, A_IND);
    TEST_ASSERT_EQUAL(0x47, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectwithOperands_given_DirectWithImm_expect_opcode_0x93DEAA() {
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
    tokenizer = createTokenizer("mOv @r1  , 0xBA");
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
    tokenizer = createTokenizer("  dIv   aB");
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

void test_isImmediateThenGetsItsValueAndConsume_given_imm0xFA_token_expect_value_extracted_is_0xFA_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer("   #0xFa@123");
    //maximum range is set to 0xFF, 0xFA is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFF);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0xFA, value);                         //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("@" , token->str);             //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                           //token is immediate, so function returns true
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isImmediateThenGetsItsValueAndConsume_given_imm0xA5A5_token_expect_value_extracted_is_0xA5A5_and_token_is_consumed() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xB0;
  Try{
    tokenizer = createTokenizer(" #0xa5A5 happy");
    //maximum range is set to 0xFFFF, 0xA5A5 is in range
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFFFF);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0xA5A5, value);                       //token is immediate, so value is extracted
    TEST_ASSERT_EQUAL_STRING("happy" , token->str);         //token is immediate, so token is consumed
    TEST_ASSERT_EQUAL(1, isTrue);                           //token is immediate, so function returns true
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
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFF);
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
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFF);      //maximum range is set to 0xFF
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
    tokenizer = createTokenizer("  #R7  ");
    int isTrue = isImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFF);
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
    tokenizer = createTokenizer("     #0x11A hehe");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFFFF);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0x11A, value);
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
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &value, 0xFF);
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

void test_isIntegerTokenThenConsume_given_is_integer_expect_token_is_consumed_value_is_extracted_and_return_1() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 0xBBAF 6699876 A");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, 0xFFFF);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(1, isTrue);
    TEST_ASSERT_EQUAL_STRING("6699876", token->str);
    TEST_ASSERT_EQUAL(0xBBAF, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_is_integer_but_out_of_range_expect_ERR_INTEGER_OUT_OF_RANGE_exception_to_be_thrown() {
  Tokenizer* tokenizer;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 0xFFFF 666");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, 0xFF);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INTEGER_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_isIntegerTokenThenConsume_given_not_integer_expect_token_is_pushed_back_and_return_0() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" abcdef xyz  ");
    int isTrue = isIntegerTokenThenConsume(tokenizer, &value, 0xFF);
    token = getToken(tokenizer);
    TEST_ASSERT_EQUAL(0, isTrue);
    TEST_ASSERT_EQUAL_STRING("abcdef", token->str);
    TEST_ASSERT_EQUAL(0xAA, value);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_verifyIsIntegerTokenThenConsume_given_is_integer_expect_token_is_consumed_and_value_is_extracted() {
  Tokenizer* tokenizer;
  Token *token;
  int value = 0xAA;
  Try{
    tokenizer = createTokenizer(" 0x789 muthu");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0xFFF);
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
    tokenizer = createTokenizer("  beef  or chicken");
    verifyIsIntegerTokenThenConsume(tokenizer, &value, 0xFF);
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

void test_isRegisterConsumeAndGetItsNumber_given_is_register_but_of_of_range_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Tokenizer* tokenizer;
  int regNum;
  Try{
    tokenizer = createTokenizer(" r5 bb");
    int isTrue = isRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
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
  
void test_checkExtraToken_given_not_null_token_expect_exception_ERR_EXTRA_PARAMETER_is_thrown() {
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

/*
void test_assembleDirectWithOperands_given_0xCD_with_A_and_DIRECT_A_LOGICAL_flag_expect_opcode_0x42CD() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xCD, A");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIRECT_A_LOGICAL);
    TEST_ASSERT_EQUAL(0x42CD, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0xCD_with_A_and_DIRECT_A_OTHER_flag_expect_opcode_0x42CD() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xCD, A");
    opcode = assembleDirectWithOperands(tokenizer, 0x40, DIRECT_A_OTHER);
    TEST_ASSERT_EQUAL(0x45CD, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x7A_with_R4_expect_opcode_0x1C7A() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x7A, R4");
    opcode = assembleDirectWithOperands(tokenizer, 0x10, DIRECT_REG);
    TEST_ASSERT_EQUAL(0x1C7A, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0xCC_with_indirectR1_expect_opcode_0x27CC() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xCC, @r1");
    opcode = assembleDirectWithOperands(tokenizer, 0x20, DIRECT_INDIRECT);
    TEST_ASSERT_EQUAL(0x27CC, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x9C_with_0xC9_expect_opcode_0x85C99C() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x9C, 0xC9");
    opcode = assembleDirectWithOperands(tokenizer, 0x80, DIRECT_DIRECT);
    TEST_ASSERT_EQUAL(0x85C99C, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x03_with_imm_0xBA_and_DIRECT_IMM_LOGICAL_flag_expect_opcode_0x6303BA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x03, #0xBA");
    opcode = assembleDirectWithOperands(tokenizer, 0x60, DIRECT_IMM_LOGICAL);
    TEST_ASSERT_EQUAL(0x6303BA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x03_with_imm_0xBA_and_DIRECT_IMM_OTHER_flag_expect_opcode_0x6503BA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x03, #0xBA");
    opcode = assembleDirectWithOperands(tokenizer, 0x60, DIRECT_IMM_OTHER);
    TEST_ASSERT_EQUAL(0x6503BA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, C");
    opcode = assembleAwithOperands(tokenizer, 0xE0, A_WITH_OPERANDS);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_A_with_R2_and_no_A_WITH_REGISTER_flag_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, r2");
    opcode = assembleAwithOperands(tokenizer, 0xE0, A_WITH_INDIRECT);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_invalid_indirect_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xBB, @hello");
    opcode = assembleDirectWithOperands(tokenizer, 0x20);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_operator_not_comma_expect_exception_ERR_MISSING_COMMA_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mov A$#23");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_MISSING_COMMA, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_operator_not_comma_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mov c, /0x55");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_xrl_c_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("xrl c, 0x23");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_anl_register_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("anl r5, #0xBB");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}
*/