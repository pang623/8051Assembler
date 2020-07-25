#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

/*
* to-do
* register in lowercase could not be read
* modify some function to be reused (similar patterns)
* way to implement mov instructions
*/

_8051Instructions instructionsTable[] = {
  {"nop" , NULL                 , 0x00},
  {"ret" , NULL                 , 0x22},
  {"reti", NULL                 , 0x32},
  {"add" , modifyOpcode_A       , 0x24},
  {"addc", modifyOpcode_A       , 0x34},
  {"subb", modifyOpcode_A       , 0x94},
  {"orl" , destAorDirect        , 0x42},
  {"anl" , destAorDirect        , 0x52},
  {"xrl" , destAorDirect        , 0x62},
  {"inc" , modifyOpcode_sOperand, 0x04},
  {"dec" , modifyOpcode_sOperand, 0x14},
  /*
  *
  *
  *
  *
  *
  */
  //  {"mov" ,???, ???},               yet to implement
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

  if(instructionsTable[i].function) {
    funcPtr instructionToOpcode = instructionsTable[i].function;
    token = getNewToken(tokenizer, token);
    opcode = instructionToOpcode(token, tokenizer, instructionsTable[i].opcode);
  }else {
    checkExtraToken(token, tokenizer);
    return instructionsTable[i].opcode;
  }

  return opcode;
}

int destAorDirect(Token *token, Tokenizer *tokenizer, int opcode) {
  if(token->type == TOKEN_INTEGER_TYPE)
    opcode = modifyOpcode_Direct(token, tokenizer, opcode*0x100);
  else if(token->type == TOKEN_IDENTIFIER_TYPE)
    opcode = modifyOpcode_A(token, tokenizer, opcode + 2);
  else
    throwException(ERR_INVALID_OPERAND, token, "Expecting an identifier or integer as the second operand, received %s instead", token->str);
  return opcode;
}

int modifyOpcode_sOperand(Token *token, Tokenizer *tokenizer, int opcode) {
  if((token->type != TOKEN_IDENTIFIER_TYPE) && (token->type != TOKEN_OPERATOR_TYPE) && (token->type != TOKEN_INTEGER_TYPE))
      throwException(ERR_INVALID_OPERAND, token, "Last operand '%s' is invalid", token->str);

    switch(token->type) {
      case TOKEN_IDENTIFIER_TYPE :  if(stricmp(((IdentifierToken *)token)->str, "A") && (((IdentifierToken *)token)->str[0]) != 'R')
                                      throwException(ERR_INVALID_OPERAND, token, "Expecting a register or 'A', received %s instead", token->str);
                                    else if(((IdentifierToken *)token)->str[0] == 'R')
                                      opcode = getRegister(token, opcode += 0x04);
                                    break;
      case TOKEN_OPERATOR_TYPE   :  if(stricmp(((OperatorToken *)token)->str, "@"))
                                      throwException(ERR_INVALID_OPERAND, token, "Expecting a '@', received %s instead", token->str);
                                    else {
                                      token = getNewToken(tokenizer, token);
                                      if(token->type != TOKEN_IDENTIFIER_TYPE)
                                        throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);
                                      opcode = getRegister(token, opcode += 0x02);
                                    }
                                    break;
      case TOKEN_INTEGER_TYPE    :  opcode = getDirect(token, opcode = (opcode + 1)*0x100);
                                    break;
      }

      checkExtraToken(token, tokenizer);
      return opcode;
}

int modifyOpcode_Direct(Token *token, Tokenizer *tokenizer, int opcode) {
  opcode = getDirect(token, opcode);
  
  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_OPERATOR_TYPE)
    throwException(ERR_MISSING_COMMA, token, "Expecting a comma, received %s instead", token->str);
  if(strcmp(((OperatorToken *)token)->str, ","))
    throwException(ERR_INVALID_OPERAND, token, "Expecting comma as third operand, received %s instead", token->str);
  
  token = getNewToken(tokenizer, token);
  if((token->type != TOKEN_IDENTIFIER_TYPE) && (token->type != TOKEN_OPERATOR_TYPE))
    throwException(ERR_INVALID_OPERAND, token, "Last operand '%s' is invalid", token->str);
  
  switch(token->type) {
    case  TOKEN_IDENTIFIER_TYPE : if(stricmp(((IdentifierToken *)token)->str, "A"))
                                    throwException(ERR_INVALID_OPERAND, token, "Expecting A as last operand, received %s instead", token->str);
                                  break;
    case  TOKEN_OPERATOR_TYPE   : if(strcmp(((OperatorToken *)token)->str, "#"))
                                    throwException(ERR_INVALID_OPERAND, token, "Expecting a '#', received %s instead", token->str);
                                  token = getNewToken(tokenizer, token);
                                  opcode = getImmediate(token, (opcode*0x100) + 0x010000);
                                  break;
  }
  checkExtraToken(token, tokenizer);
  return opcode;
}

int modifyOpcode_A(Token *token, Tokenizer *tokenizer, int opcode) {
  if(token->type != TOKEN_IDENTIFIER_TYPE)                                                   //check is second operand an identifier
    throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);

  if(stricmp(((IdentifierToken *)token)->str, "A"))
    throwException(ERR_INVALID_OPERAND, token, "Expecting A as operand, received %s instead", token->str);

  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_OPERATOR_TYPE)                                                     //check is third operand an operator
    throwException(ERR_MISSING_COMMA, token, "Expecting a comma, received %s instead", token->str);

  if(strcmp(((OperatorToken *)token)->str, ","))
    throwException(ERR_INVALID_OPERAND, token, "Expecting comma as third operand, received %s instead", token->str);
  else{
    token = getNewToken(tokenizer, token);
    if((token->type != TOKEN_IDENTIFIER_TYPE) && (token->type != TOKEN_OPERATOR_TYPE) && (token->type != TOKEN_INTEGER_TYPE))
      throwException(ERR_INVALID_OPERAND, token, "Last operand '%s' is invalid", token->str);

    switch(token->type) {
      case TOKEN_IDENTIFIER_TYPE :  opcode = getRegister(token, opcode += 0x04);
                                    break;
      case TOKEN_OPERATOR_TYPE   :  if(strcmp(((OperatorToken *)token)->str, "#") && strcmp(((OperatorToken *)token)->str, "@"))
                                      throwException(ERR_INVALID_OPERAND, token, "Expecting a '#' or '@', received %s instead", token->str);
                                    else if(!stricmp(((OperatorToken *)token)->str, "#")) {
                                      token = getNewToken(tokenizer, token);
                                      opcode = getImmediate(token, opcode *= 0x100);
                                    }else {
                                      token = getNewToken(tokenizer, token);
                                      if(token->type != TOKEN_IDENTIFIER_TYPE)
                                        throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);
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

uint8_t getRegister(Token *token, uint8_t opcode) {
  uint8_t opcodeMode = opcode;

  if(((IdentifierToken *)token)->str[0] != 'R')
    throwException(ERR_INVALID_OPERAND, token, "Expecting a register, received %s instead", token->str);
  if((!(((IdentifierToken *)token)->str[1])) || (((IdentifierToken *)token)->str[1] > '@'))
    throwException(ERR_INVALID_REGISTER, token, "An invalid register '%s' is inputted", token->str);
  if(((IdentifierToken *)token)->str[2] >= '0' && ((IdentifierToken *)token)->str[2] <= '9')
    throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range, register cannot be of more than one digit range", token->str);
  if(((IdentifierToken *)token)->str[2] >= '@')
    throwException(ERR_INVALID_REGISTER, token, "An invalid register '%s' is inputted", token->str);

  if((opcodeMode & 0x0F) == 8) {
    if(((IdentifierToken *)token)->str[1] < ':' && ((IdentifierToken *)token)->str[1] > '7')
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range, expecting register of R0-R7", token->str);
  }else if((opcodeMode & 0x0F) == 6) {
    if(((IdentifierToken *)token)->str[1] < ':' && ((IdentifierToken *)token)->str[1] > '1')
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range, expecting register of R0-R1", token->str);
  }

  return opcode += ((IdentifierToken *)token)->str[1] - 48;
}

//yet to implement #xxh immediate format
int getImmediate(Token *token, int opcode) {
  if(token->type != TOKEN_INTEGER_TYPE)
    throwException(ERR_EXPECTING_INTEGER, token, "Expecting an integer, received '%s' instead", token->str);
  else if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_IMMEDIATE_OUT_OF_RANGE, token, "Immediate %x is out of range", ((IntegerToken *)token)->value);
  else
    return opcode += ((IntegerToken *)token)->value;
}

//yet to implement #xxh immediate format
int getDirect(Token *token, int opcode) {
  if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_DIRECT_OUT_OF_RANGE, token, "Direct address %x is out of range", ((IntegerToken *)token)->value);
  else
    return opcode += ((IntegerToken *)token)->value;
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