#include "unity.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include <stdio.h>
#include "Error.h"
#include "CException.h"
#include "Exception.h"


void setUp(void)
{
}

void tearDown(void)
{
}

CEXCEPTION_T e;

void test_assembleInstruction_given_add_A_with_r7_expect_opcode_0x2f() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, R7");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x2F, opcode);
    } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
    }
    freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_direct_0xba_expect_opcode_0x25ba() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, 0xBA");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x25BA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_R0_expect_opcode_0x26() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{  tokenizer = createTokenizer("add A, @R0");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x26, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_subb_A_with_immediate_in_hex_0xA8_expect_opcode_0x94A8() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("subb A, #0xA8");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x94A8, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_direct_with_immediate_expect_opcode_0x755abb() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("MoV 0x5A, #0xbb");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x755abb, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_c_with_bit_expect_opcode_0xA27B() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mOv c, 0x7b");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0xA27B, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_indirect_with_direct_expect_opcode_0x7713() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mOv @r1, a");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0xf7, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_reg_with_imm_expect_opcode_0x78dd() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mOv r0, #0xdd");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x78dd, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_a_with_indirect_expect_opcode_0xe7() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("mOv a, @r1");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0xe7, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ret_expect_opcode_0x22() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("rET");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x22, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_IndirectWithImmediate_expect_opcode_0xB72D() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@r1, #0x2D");
    opcode = assembleIndirectWithOperands(tokenizer, 0xB0);
    TEST_ASSERT_EQUAL(0xB72D, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_IndirectWithA_expect_opcode_0xD6() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@R0, a");
    opcode = assembleIndirectWithOperands(tokenizer, 0xD0);
    TEST_ASSERT_EQUAL(0xD6, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_IndirectWithDirect_expect_opcode_0x27A3() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@r1, 0xA3");
    opcode = assembleIndirectWithOperands(tokenizer, 0x20);
    TEST_ASSERT_EQUAL(0x27A3, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithDirect_expect_opcode_0xADDA() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r5, 0xDA");
    opcode = assembleRegWithOperands(tokenizer, 0xA0);
    TEST_ASSERT_EQUAL(0xADDA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithAcc_expect_opcode_0x1F() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r7, a");
    opcode = assembleRegWithOperands(tokenizer, 0x10);
    TEST_ASSERT_EQUAL(0x1F, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithImmediate_expect_opcode_0xC8AA() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r0, #0xAA");
    opcode = assembleRegWithOperands(tokenizer, 0xC0);
    TEST_ASSERT_EQUAL(0xC8AA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithIndirect_expect_opcode_0x46() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, @r0");
    opcode = assembleAwithOperands(tokenizer, 0x40);
    TEST_ASSERT_EQUAL(0x46, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithRegister_expect_opcode_0x9E() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, r6");
    opcode = assembleAwithOperands(tokenizer, 0x90);
    TEST_ASSERT_EQUAL(0x9E, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithImmediate_expect_opcode_0x84C0() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, #0xC0");
    opcode = assembleAwithOperands(tokenizer, 0x80);
    TEST_ASSERT_EQUAL(0x84C0, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithDirect_expect_opcode_0x7538() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, 0x38");
    opcode = assembleAwithOperands(tokenizer, 0x70);
    TEST_ASSERT_EQUAL(0x7538, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithAandImmediateOnly_given_0xED_with_A_expect_opcode_0x42ED() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xED, a");
    opcode = assembleDirectWithAandImmediateOnly(tokenizer, 0x40);
    TEST_ASSERT_EQUAL(0x42ED, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithAandImmediateOnly_given_0xED_with_Imm_0xFF_expect_opcode_0x83EDFF() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xED, #0xFF");
    opcode = assembleDirectWithAandImmediateOnly(tokenizer, 0x80);
    TEST_ASSERT_EQUAL(0x83EDFF, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0xCD_with_A_expect_opcode_0x45CD() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xCD, A");
    opcode = assembleDirectWithOperands(tokenizer, 0x40);
    TEST_ASSERT_EQUAL(0x45CD, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x7A_with_R4_expect_opcode_0x1C7A() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x7A, R4");
    opcode = assembleDirectWithOperands(tokenizer, 0x10);
    TEST_ASSERT_EQUAL(0x1C7A, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0xCC_with_indirectR1_expect_opcode_0x27CC() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0xCC, @r1");
    opcode = assembleDirectWithOperands(tokenizer, 0x20);
    TEST_ASSERT_EQUAL(0x27CC, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x03_with_imm_0xBA_expect_opcode_0x6503BA() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x03, #0xBA");
    opcode = assembleDirectWithOperands(tokenizer, 0x60);
    TEST_ASSERT_EQUAL(0x6503BA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_0x9C_with_0xC9_expect_opcode_0x85C99C() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("0x9C, 0xC9");
    opcode = assembleDirectWithOperands(tokenizer, 0x80);
    TEST_ASSERT_EQUAL(0x85C99C, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("R6, AB");
    opcode = assembleRegWithOperands(tokenizer, 0x80);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_invalid_immediate_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, #R0");
    opcode = assembleAwithOperands(tokenizer, 0xE0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleDirectWithOperands_given_invalid_indirect_expect_exception_ERR_EXPECTING_REGISTER_to_be_thrown() {
  Token* token;
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

void test_assembleIndirectWithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@r0, @R1");
    opcode = assembleIndirectWithOperands(tokenizer, 0x20);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_mov_operator_not_comma_expect_exception_ERR_MISSING_COMMA_to_be_thrown() {
  Token* token;
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
  Token* token;
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
  Token* token;
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
  Token* token;
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

void test_token() {
  Tokenizer *tokenizer;
  Token *token1, *token2, *token3, *token;
  tokenizer = createTokenizer("3, A");
  token1 = getToken(tokenizer);
  token2 = getToken(tokenizer);
  token3 = getToken(tokenizer);
  printf("%s", token1->str);
  printf("%s", token2->str);
  printf("%s", token3->str);
  pushBackToken(tokenizer, token1);
  token = getToken(tokenizer);
  printf("%s", token->str);
 // freeToken(token);
  token = getToken(tokenizer);
  printf("%s", token->str);
 // freeToken(token);
  token = getToken(tokenizer);
  printf("%s", token->str);
  freeToken(token);
  freeToken(token2);
  freeToken(token3);
  freeTokenizer(tokenizer);
}
/*
void test_assembleInstruction_given_inc_indirectR1_expect_opcode_0x07() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("iNC @R1");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x07, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_dec_direct_expect_opcode_0x15AD() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("dEc 0xAD");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x15AD, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_reg_out_of_range_expect_exception_ERR_REG_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add a, R9 ");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_REG_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_reg_out_of_range_three_digits_expect_exception_ERR_REG_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, R202");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_REG_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_register_expect_exception_ERR_INVALID_REGISTER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, R22ss");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_REGISTER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_comma_missing_expect_exception_ERR_MISSING_COMMA_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A R2");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_OPERATOR, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_wrong_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A& R2");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_A_is_not_the_second_token_expect_exception_ERR_EXPECTING_IDENTIFIER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add 19, R3");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_A_is_not_the_second_token_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add b3, R3");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, ");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_instruction_expect_exception_ERR_EXPECTING_IDENTIFIER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("1#$6 A, R7");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_instruction_with_extra_param_expect_exception_ERR_EXTRA_PARAMETER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, R7 R9");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_immediate_out_of_range_in_hex_expect_exception_ERR_IMMEDIATE_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, #0xFFFF");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_IMMEDIATE_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_integer_expect_exception_ERR_INVALID_INTEGER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, #0xfdg12");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_direct_out_of_range_expect_exception_ERR_DIRECT_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, 0xFFA");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_DIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_out_of_range_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, @R2");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_out_of_range_2_digits_expect_exception_ERR_REG_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, @R30");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INDIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_indirect_integers_expect_exception_ERR_EXPECTING_IDENTIFIER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, @11");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_indirect_symbols_expect_exception_ERR_EXPECTING_IDENTIFIER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, @#6#@");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_indirect_string_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, @hello");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("add A, ABC123");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_instruction_divide_A_expect_exception_ERR_INVALID_INSTRUCTION_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("divide A, #23");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_INSTRUCTION, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_orl_A_with_R3_expect_opcode_0x4B() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("orl A, R3");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x4B, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_orl_A_with_indirect_R0_expect_opcode_0x46() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("orl 0x23, #0x5F");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x43235F, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_too_large_direct_expect_exception_ERR_DIRECT_OUT_OF_RANGE_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("anl 0x2F3D, #21");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_DIRECT_OUT_OF_RANGE, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_operand_A_expect_exception_ERR_EXPECTING_INTEGER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("xRl 0x2F, #A");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXPECTING_INTEGER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_second_operand_$_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("ANL $0xFF, #A");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_extra_parameter_expect_exception_ERR_EXTRA_PARAMETER_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("nop A");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_invalid_operand_imm23_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("dec B");
    opcode = assembleInstruction(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}*/