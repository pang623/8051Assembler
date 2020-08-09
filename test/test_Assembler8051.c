#include "unity.h"
#include "Token.h"
#include "Tokenizer.h"
#include "Assembler8051.h"
#include <stdio.h>
#include "Error.h"
#include "Flags.h"
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

void test_assembleInstruction_given_DIV_AB_expect_opcode_0x84() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("dIv Ab");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x84, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_ret_expect_opcode_0x22() {
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
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@r1, #0x2D");
    opcode = assembleIndirectWithOperands(tokenizer, 0xB0, INDIRECT_IMM);
    TEST_ASSERT_EQUAL(0xB72D, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_IndirectWithA_expect_opcode_0xD6() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@R0, a");
    opcode = assembleIndirectWithOperands(tokenizer, 0xD0, INDIRECT_WITH_A);
    TEST_ASSERT_EQUAL(0xD6, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_IndirectWithDirect_expect_opcode_0x27A3() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("@r1, 0xA3");
    opcode = assembleIndirectWithOperands(tokenizer, 0x20, INDIRECT_DIRECT);
    TEST_ASSERT_EQUAL(0x27A3, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithDirect_expect_opcode_0xADDA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r5, 0xDA");
    opcode = assembleRegWithOperands(tokenizer, 0xA0, REG_WITH_DIRECT);
    TEST_ASSERT_EQUAL(0xADDA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithAcc_expect_opcode_0x1F() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r7, a");
    opcode = assembleRegWithOperands(tokenizer, 0x10, REG_WITH_A);
    TEST_ASSERT_EQUAL(0x1F, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_RegWithImmediate_expect_opcode_0xC8AA() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("r0, #0xAA");
    opcode = assembleRegWithOperands(tokenizer, 0xC0, REG_WITH_IMM);
    TEST_ASSERT_EQUAL(0xC8AA, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithIndirect_expect_opcode_0x46() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, @r0");
    opcode = assembleAwithOperands(tokenizer, 0x40, A_WITH_INDIRECT);
    TEST_ASSERT_EQUAL(0x46, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithRegister_expect_opcode_0x9E() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, r6");
    opcode = assembleAwithOperands(tokenizer, 0x90, A_WITH_REGISTER);
    TEST_ASSERT_EQUAL(0x9E, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithImmediate_expect_opcode_0x84C0() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, #0xC0");
    opcode = assembleAwithOperands(tokenizer, 0x80, A_WITH_IMMEDIATE);
    TEST_ASSERT_EQUAL(0x84C0, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_AWithDirect_expect_opcode_0x7538() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, 0x38");
    opcode = assembleAwithOperands(tokenizer, 0x70, A_WITH_DIRECT);
    TEST_ASSERT_EQUAL(0x7538, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

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

void test_assembleRegWithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("R6, AB");
    opcode = assembleRegWithOperands(tokenizer, 0x80, REGISTER_WITH_OPERANDS);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleRegWithOperands_given_R6_with_imm_and_no_REG_WITH_IMM_flag_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("R6, #0x23");
    opcode = assembleRegWithOperands(tokenizer, 0x80, REG_WITH_A);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
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
/*
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

void test_assembleIndirectWithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
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
*/
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