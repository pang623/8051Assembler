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

void test_assembleInstruction_R7() {
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

void test_assembleInstruction_R5() {
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

void test_assembleInstruction_invalid_reg_out_of_range_one_digit() {
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

void test_assembleInstruction_invalid_reg_out_of_range_three_digit() {
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

void test_assembleInstruction_invalid_reg() {
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

void test_assembleInstruction_missing_comma() {
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

void test_assembleInstruction_second_operand_is_not_Accumulator() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add b3, R3");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_EXPECTING_IDENTIFIER, e->errorCode);
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_first_operand_is_not_an_identifier() {
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

void test_assembleInstruction_extra_parameter() {
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

void test_assembleInstruction_add_immediateHEX_to_A() {
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

void test_assembleInstruction_add_immediateDEC_to_A() {
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

void test_assembleInstruction_add_immediateDEC_with_extra_parameter() {
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

void test_assembleInstruction_add_immediateHEX_to_A_out_of_range() {
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

void test_assembleInstruction_add_immediateDEC_to_A_out_of_range() {
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

void test_assembleInstruction_invalid_immediateHEX() {
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

void test_assembleInstruction_invalid_immediateDEC() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, #rtg");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_INVALID_INTEGER, e->errorCode);
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_add_directAddr_0x9F_to_A() {
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

void test_assembleInstruction_add_directAddr_0xBA_to_A() {
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

void test_assembleInstruction_add_directAddr_0xFFA_to_A_out_of_range() {
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

void test_assembleInstruction_add_directAddr_0xFJGE_to_A_invalid_direct() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, 0xFJGE");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_INVALID_INTEGER, e->errorCode);
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_add_directAddr_0x36_to_A_extra_param() {
	Token* token;
	Tokenizer* tokenizer;
	int opcode;
	Try{
		tokenizer = createTokenizer("add A, 0x36 #34");
		opcode = assembleInstruction(tokenizer);
		TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
	} Catch(e){
		dumpTokenErrorMessage(e, 1);
		TEST_ASSERT_EQUAL(ERR_EXTRA_PARAMETER, e->errorCode);
	}
	freeTokenizer(tokenizer);
}

void test_assembleInstruction_add_Indirect_R0_to_A() {
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

void test_assembleInstruction_add_Indirect_R1_to_A() {
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

void test_assembleInstruction_add_invalidIndirect_R2_to_A_out_of_range() {
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

void test_assembleInstruction_add_invalidIndirect_R30_to_A_out_of_range() {
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

void test_assembleInstruction_add_invalidIndirect_integer_to_A_invalid_operand() {
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

void test_assembleInstruction_add_invalidIndirect_symbol_to_A_invalid_operand() {
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

void test_assembleInstruction_add_invalidIndirect_string_to_A_invalid_operand() {
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