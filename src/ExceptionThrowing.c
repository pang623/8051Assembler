#include "ExceptionThrowing.h"

void throwMovAWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting register, integer, '#' or '@', received %s instead", token->str);
}

void throwMovRegWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting A, integer or '#', received %s instead", token->str);
}

void throwMovDirectWithInvalidOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting A, register, integer '@' or '#', received %s instead", token->str);
}

void throwInvalidMovFirstOperand(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting A, C, DPTR, register, integer or '@', received %s instead", token->str);
}

void throwExpectingRegisterOrDPTRexception(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting register or 'DPTR', but received %s instead", token->str);
}

void throwExpectingAOrIndException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting 'A' or '@', but received %s instead", token->str);
}

void throwExpectingPCorDPTRexception(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting 'DPTR' or 'PC', but received %s instead", token->str);
}

void throwExpectingIntOrImmException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting integer or '#', but received %s instead", token->str);
}

void throwInvalidCJNEFirstOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting 'A', '@' or register, but received %s instead", token->str);
}

void throwInvalidDJNZFirstOperandException(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  throwException(ERR_INVALID_OPERAND, token, 0,
  "Expecting integer or register, but received %s instead", token->str);
}