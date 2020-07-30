#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*
* to-do
* modify some function to be reused (similar patterns)
* way to implement mov instructions
*/

_8051Instructions instructionsTable[] = {
  {"nop" , NULL                 , 0x00, 0, 0, 0},
  {"ret" , NULL                 , 0x22, 0, 0, 0},
  {"reti", NULL                 , 0x32, 0, 0, 0},
  {"add" , modifyOpcode_A       , 0x24, 0x25, 0x26, 0x28},
  {"addc", modifyOpcode_A       , 0x34, 0x35, 0x36, 0x38},
  {"subb", modifyOpcode_A       , 0x94, 0x95, 0x96, 0x98},
 // {"orl" , destAorDirect        , 0x42},
 // {"anl" , destAorDirect        , 0x52},
 // {"xrl" , destAorDirect        , 0x62},
 // {"inc" , modifyOpcode_sOperand, 0x04},
 // {"dec" , modifyOpcode_sOperand, 0x14},
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
  _8051Instructions *opcPtr;

  token = getToken(tokenizer);
  if(token->type != TOKEN_IDENTIFIER_TYPE)
    throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);

  while(stricmp(token->str, instructionsTable[i].instruction)) {
    if(instructionsTable[i++].instruction == NULL)
      throwException(ERR_INVALID_INSTRUCTION, token, "An invalid instruction '%s' is inputted", token->str);
  }
  opcPtr = &instructionsTable[i];
  if(opcPtr->function) {
    funcPtr instructionToOpcode = opcPtr->function;
    token = getNewToken(tokenizer, token);
    opcode = instructionToOpcode(token, tokenizer, opcPtr);
  }else {
    checkExtraToken(token, tokenizer);
    return opcPtr->op1;
  }
  return opcode;
}
/*
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
      case TOKEN_OPERATOR_TYPE   :  if(strcmp(((OperatorToken *)token)->str, "@"))
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
*/
int isOperatorToken(Token *token, char *operators) {
  if(token->type != TOKEN_OPERATOR_TYPE)
    throwException(ERR_EXPECTING_OPERATOR, token, "Expecting an operator, received %s instead", token->str);

  if(strcmp(token->str, operators))
    return 0;
  else
    return 1;
}

int modifyOpcode_A(Token *token, Tokenizer *tokenizer, _8051Instructions *opcPtr) {
  int opcode;
  if(token->type != TOKEN_IDENTIFIER_TYPE)
    throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);

  if(stricmp(token->str, "A"))
    throwException(ERR_INVALID_OPERAND, token, "Expecting A as operand, received %s instead", token->str);

  token = getNewToken(tokenizer, token);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token, "Expecting a comma , received %s instead", token->str);

  token = getNewToken(tokenizer, token);
  if((token->type != TOKEN_IDENTIFIER_TYPE) && (token->type != TOKEN_OPERATOR_TYPE) && (token->type != TOKEN_INTEGER_TYPE))
    throwException(ERR_INVALID_OPERAND, token, "Last operand '%s' is invalid", token->str);

    switch(token->type) {
      case TOKEN_IDENTIFIER_TYPE :
        opcode = verifyValidRegisterRangeAndReturnRespectiveOpcode(token, opcPtr->op4);
        break;

      case TOKEN_OPERATOR_TYPE :
        if(isOperatorToken(token, "#")) {
          token = getNewToken(tokenizer, token);
          opcode = getImmediate(token, opcPtr->op1);
        }else if(isOperatorToken(token, "@")) {
          token = getNewToken(tokenizer, token);
          if(token->type != TOKEN_IDENTIFIER_TYPE)
            throwException(ERR_EXPECTING_IDENTIFIER, token, "Expecting an identifier, received %s instead", token->str);
          opcode = verifyValidRegisterRangeAndReturnRespectiveOpcode(token, opcPtr->op3);
        }else
          throwException(ERR_INVALID_OPERAND, token, "Expecting a # or @, received %s instead", token->str);
          break;

      case TOKEN_INTEGER_TYPE :
        opcode = getDirect(token, opcPtr->op2);
        break;
      }
  checkExtraToken(token, tokenizer);
  return opcode;
}

int extractNum(char *start, Token *token) {
  int num;
  char *end;
  num = strtol(start, &end, 10);
  if ( (start[0] == end[0]) || (end[0]) )
    throwException(ERR_INVALID_REGISTER, token, "An invalid register '%s' is inputted", token->str);
  else
    return num;
}

uint8_t verifyValidRegisterRangeAndReturnRespectiveOpcode(Token *token, uint8_t opcode) {
  uint8_t opcodeMode = opcode;
  int regNum;

  if(tolower(token->str[0]) != 'r')
    throwException(ERR_INVALID_OPERAND, token, "Expecting a register, received %s instead", token->str);

  regNum = extractNum(token->str+1, token);

  if((opcodeMode & 0x0F) == RegAddr) {
    if(regNum > 7)
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range, expecting register of R0-R7", token->str);
  }else if((opcodeMode & 0x0F) == IndAddr) {
    if(regNum > 1)
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range, expecting register of R0-R1", token->str);
  }
  return opcode += regNum;
}

//yet to implement #xxh immediate format
int getImmediate(Token *token, int opcode) {
  if(token->type != TOKEN_INTEGER_TYPE)
    throwException(ERR_EXPECTING_INTEGER, token, "Expecting an integer, received '%s' instead", token->str);
  else if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_IMMEDIATE_OUT_OF_RANGE, token, "Immediate %x is out of range", ((IntegerToken *)token)->value);
  else
    return opcode = (opcode << 8) + (((IntegerToken *)token)->value);
}

//yet to implement #xxh immediate format
int getDirect(Token *token, int opcode) {
  if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_DIRECT_OUT_OF_RANGE, token, "Direct address %x is out of range", ((IntegerToken *)token)->value);
  else
    return opcode = (opcode << 8) + (((IntegerToken *)token)->value);
}

void checkExtraToken(Token *token, Tokenizer *tokenizer) {
  token = getNewToken(tokenizer, token);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_NEWLINE_TYPE)
    throwException(ERR_EXTRA_PARAMETER, token, "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}

Token* getNewToken(Tokenizer *tokenizer, Token *token) {
  freeToken(token);
  return token = getToken(tokenizer);
}