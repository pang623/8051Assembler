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
  Token* token;
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

void test_assembleInstruction_given_ret_expect_opcode_0x22() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("RET");
    opcode = assembleInstruction(tokenizer);
    TEST_ASSERT_EQUAL(0x22, opcode);
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
  }
  freeTokenizer(tokenizer);
}

void test_assembleIndirectWithOperands_given_directWithImmediate_expect_opcode_0x772D() {
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

void test_assembleAwithOperands_given_AWithIndirect_expect_opcode_0x46() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("a, @r0");
    //from instruction orl
    opcode = assembleAwithOperands(tokenizer, 0x40);
    TEST_ASSERT_EQUAL(0x46, opcode);
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
    tokenizer = createTokenizer("r3, @R0");
    opcode = assembleRegWithOperands(tokenizer, 0x80);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_assembleAwithOperands_given_invalid_last_operand_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
  Token* token;
  Tokenizer* tokenizer;
  int opcode;
  Try{
    tokenizer = createTokenizer("A, B");
    opcode = assembleAwithOperands(tokenizer, 0xE0);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, 1);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
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