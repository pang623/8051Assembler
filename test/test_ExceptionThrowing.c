#include "unity.h"
#include "ExceptionThrowing.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CException.h"
#include "Exception.h"

CEXCEPTION_T e;

void setUp(void)
{
}

void tearDown(void)
{
}

void test_throwMovAWithInvalidOperandException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer("PC");
  Try {
    throwMovAWithInvalidOperandException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwMovRegWithInvalidOperandException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" hello ");
  Try {
    throwMovRegWithInvalidOperandException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwMovDirectWithInvalidOperandException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" DPTR ");
  Try {
    throwMovDirectWithInvalidOperandException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwInvalidMovFirstOperand_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" #23 ");
  Try {
    throwInvalidMovFirstOperand(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwExpectingRegisterOrDPTRexception_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" A ");
  Try {
    throwExpectingRegisterOrDPTRexception(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwExpectingAOrIndException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" R4 ");
  Try {
    throwExpectingAOrIndException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwExpectingPCorDPTRexception_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" 0x56 ");
  Try {
    throwExpectingPCorDPTRexception(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwExpectingIntOrImmException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer("@r0 ");
  Try {
    throwExpectingIntOrImmException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwInvalidCJNEFirstOperandException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" SKIP ");
  Try {
    throwInvalidCJNEFirstOperandException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}

void test_throwInvalidDJNZFirstOperandException_just_to_test_exception_is_thrown_correctly() {
  Tokenizer *tokenizer;
  tokenizer = createTokenizer(" A ");
  Try {
    throwInvalidDJNZFirstOperandException(tokenizer);
    TEST_FAIL_MESSAGE("System Error: An exception is expected, but none received!");
  } Catch(e){
    dumpTokenErrorMessage(e, __LINE__);
    TEST_ASSERT_EQUAL(ERR_INVALID_OPERAND, e->errorCode);
  }
  freeTokenizer(tokenizer);
}