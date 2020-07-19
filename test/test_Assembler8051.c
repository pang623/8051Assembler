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

void test_assembleInstruction_given_add_A_with_r5_expect_opcode_0x2d() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, R5");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x2D, opcode);
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_add_immediate_xxh_to_A() {
	TEST_IGNORE_MESSAGE("immediate in #xxh format is unreadable");
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #44");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x2444, opcode);
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_immediate_in_hex_0x4f_expect_opcode_0x244f() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #0x4F");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x244F, opcode);
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_immediate_in_dec_56_expect_opcode_0x2438() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #56");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x2438, opcode);
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_direct_0x9f_expect_opcode_0x259f() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, 0x9F");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x259F, opcode);
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
	Try{
		tokenizer = createTokenizer("add A, @R0");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x26, opcode);
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_FAIL_MESSAGE("System Error: Don't expect any exception to be thrown!");
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_indirect_R1_expect_opcode_0x27() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, @R1");
		opcode = assembleInstruction(tokenizer);
		TEST_ASSERT_EQUAL(0x27, opcode);
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
		tokenizer = createTokenizer("add A, R9");
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
		tokenizer = createTokenizer("add A, R@3");
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
		TEST_ASSERT_EQUAL(ERR_MISSING_COMMA, e->errorCode);
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

void test_assembleInstruction_given_add_instruction_with_immediate_and_extra_param_expect_exception_ERR_EXTRA_PARAMETER_to_be_thrown() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #56 #7");
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

void test_assembleInstruction_given_add_A_with_immediate_out_of_range_in_dec_expect_exception_ERR_IMMEDIATE_OUT_OF_RANGE_to_be_thrown() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #9854346");
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

void test_assembleInstruction_given_add_A_with_invalid_direct_expect_exception_ERR_INVALID_INTEGER_to_be_thrown() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, 0xFJ@E");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_INVALID_INTEGER, e->errorCode);
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

void test_assembleInstruction_given_add_A_with_indirect_out_of_range_2_digits_expect_exception_ERR_INDIRECT_OUT_OF_RANGE_to_be_thrown() {
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

void test_assembleInstruction_given_add_A_with_invalid_indirect_integers_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, @11");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_given_add_A_with_invalid_indirect_symbols_expect_exception_ERR_INVALID_OPERAND_to_be_thrown() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, @#6#@");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
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