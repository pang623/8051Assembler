#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

_8051Instructions instructionsTable[] = {
  {"add", modifyOpcode_A, 0x24},
  {"subb", modifyOpcode_A, 0x94},
  /*
  *
  *
  *
  *
  *
  */
  {NULL, NULL},
};

int assembleInstruction(Tokenizer *tokenizer) {
  Token* token;
  int opcode;
  int i = 0;

  token = getToken(tokenizer);
  if(token->type != TOKEN_IDENTIFIER_TYPE)
    throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);

  while(stricmp(((IdentifierToken *)token)->str, instructionsTable[i].instruction)) {
    if(instructionsTable[i++].instruction == NULL)
      throwException(ERR_INVALID_INSTRUCTION, token, "An invalid instruction '%s' is inputted", token->str);
  }

  funcPtr instructionToOpcode = instructionsTable[i].function;
  opcode = instructionToOpcode(token, tokenizer, instructionsTable[i].opcode);

  return opcode;
}

int modifyOpcode_A(Token *token, Tokenizer *tokenizer, uint16_t opcode) {
  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_IDENTIFIER_TYPE)                                                   //check is second operand an identifier
    throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);

  if(stricmp(((IdentifierToken *)token)->str, "A"))
    throwException(ERR_INVALID_OPERAND, token, "Expecting A as second operand, received %s instead", token->str);

  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_OPERATOR_TYPE)                                                     //check is third operand an operator
    throwException(ERR_MISSING_COMMA, token, "Expecting a comma, received %s instead", token->str);

  if(stricmp(((OperatorToken *)token)->str, ","))
    throwException(ERR_INVALID_OPERAND, token, "Expecting comma as third operand, received %s instead", token->str);
  else{
    token = getNewToken(tokenizer, token);
    if((token->type != TOKEN_IDENTIFIER_TYPE) && (token->type != TOKEN_OPERATOR_TYPE) && (token->type != TOKEN_INTEGER_TYPE))
      throwException(ERR_INVALID_OPERAND, token, "Last operand '%s' is invalid", token->str);

    switch(token->type) {
      case TOKEN_IDENTIFIER_TYPE :  if(((IdentifierToken *)token)->str[0] != 'R')
                                      throwException(ERR_INVALID_OPERAND, token, "Expecting a register, received %s instead", token->str);
                                    else
                                      opcode = getRegister(token, opcode += 0x04);
                                    break;
      case TOKEN_OPERATOR_TYPE   :  if(stricmp(((OperatorToken *)token)->str, "#") && stricmp(((OperatorToken *)token)->str, "@"))
                                      throwException(ERR_INVALID_OPERAND, token, "Expecting a '#' or '@', received %s instead", token->str);
                                    else if(!stricmp(((OperatorToken *)token)->str, "#")) {
                                      token = getNewToken(tokenizer, token);
                                      opcode = getImmediate(token, opcode *= 0x100);
                                    }else {
                                      token = getNewToken(tokenizer, token);
                                      opcode = getRegister(token, opcode += 0x02);
                                    }
                                    break;
      case TOKEN_INTEGER_TYPE    :  opcode = getDirect(token, opcode = (opcode + 1)*0x100);
                                    break;
      }
      
      checkExtraToken(token, tokenizer);
      return opcode;
  }
}

//possible code optimization(yet to be done)
uint8_t getRegister(Token *token, uint8_t opcode) {
  uint8_t opcodeMode = opcode;

  if((opcodeMode & 0x0F) == 8) {
    if(!(((IdentifierToken *)token)->str[1]))
      throwException(ERR_INVALID_REGISTER, token, "Expecting register of R0-R7, received %s instead", token->str);
    else if(((IdentifierToken *)token)->str[2])
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range", token->str);
    else if(((IdentifierToken *)token)->str[1] < '0' || ((IdentifierToken *)token)->str[1] > '7')
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range", token->str);
    else
      opcode += ((IdentifierToken *)token)->str[1] - 48;
  }else if((opcodeMode & 0x0F) == 6) {
    if(((IdentifierToken *)token)->str[0] != 'R')             //check token type is identifier first, if true then only check 'R'(yet to be done)
      throwException(ERR_INVALID_OPERAND, token, "Expecting a register for indirect addressing", token->str);
    else if(!(((IdentifierToken *)token)->str[1]))
      throwException(ERR_INVALID_REGISTER, token, "Invalid register indirect", token->str);
    else if(((IdentifierToken *)token)->str[2])
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range", token->str);
    else if(((IdentifierToken *)token)->str[1] < '0' || ((IdentifierToken *)token)->str[1] > '1')
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range", token->str);
    else
      opcode += ((IdentifierToken *)token)->str[1] - 48;
  }
  return opcode;
}

//yet to implement #xxh immediate format
uint16_t getImmediate(Token *token, uint16_t opcode) {
  if(token->type != TOKEN_INTEGER_TYPE)
    throwException(ERR_EXPECTING_INTEGER, token, "Expecting an integer, received '%s' instead", token->str);
  else if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_IMMEDIATE_OUT_OF_RANGE, token, "Immediate %x is out of range", ((IntegerToken *)token)->value);
  else
    opcode += ((IntegerToken *)token)->value;

  return opcode;
}

//yet to implement #xxh immediate format
uint16_t getDirect(Token *token, uint16_t opcode) {
  if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_DIRECT_OUT_OF_RANGE, token, "Direct address %x is out of range", ((IntegerToken *)token)->value);
  else
    opcode += ((IntegerToken *)token)->value;

  return opcode;
}

void checkExtraToken(Token *token, Tokenizer *tokenizer) {
  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_NULL_TYPE)
    throwException(ERR_EXTRA_PARAMETER, token, "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}

Token* getNewToken(Tokenizer *tokenizer, Token *token) {
  freeToken(token);
  return token = getToken(tokenizer);
}