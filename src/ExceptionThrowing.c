#include "ExceptionThrowing.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CException.h"
#include "Exception.h"

void throwAWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting register, integer, '#' or '@', received %s instead", token->str);
}

void throwRegWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting A, integer or '#', received %s instead", token->str);
}

void throwDirectWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting A, register, integer '@' or '#', received %s instead", token->str);
}

void throwInvalidMovOperand(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting A, C, DPTR, register, integer or '@', received %s instead", token->str);
}

void throwExpectingRegisterOrDPTRexception(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting register or 'DPTR', but received %s instead", token->str);
}

void throwExpectingAOrIndException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting 'A' or '@', but received %s instead", token->str);
}

void throwExpectingPCorDPTRexception(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting 'DPTR' or 'PC', but received %s instead", token->str);
}

void throwExpectingIntOrImmException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting integer or '#', but received %s instead", token->str);
}

void throwInvalidCJNEFirstOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token,
  "Expecting 'A', '@' or register, but received %s instead", token->str);
}