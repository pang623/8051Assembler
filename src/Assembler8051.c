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

_8051Instructions instructionsTable[] = {
  {"nop" , NULL                 , {0x00, 0}},
  {"ret" , NULL                 , {0x22, 0}},
  {"reti", NULL                 , {0x32, 0}},
  {"add" , assembleArithAndLogicalOperation, {0x20, A_WITH_OPERANDS}},
  {"addc", assembleArithAndLogicalOperation, {0x30, A_WITH_OPERANDS}},
  {"subb", assembleArithAndLogicalOperation, {0x90, A_WITH_OPERANDS}},
  {"orl" , assembleArithAndLogicalOperation, {0x40, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_BIT}},
  {"anl" , assembleArithAndLogicalOperation, {0x50, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_BIT}},
  {"mov" , assembleMovOperation,  {0xE0, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_BIT | INDIRECT_WITH_OPERANDS | REGISTER_WITH_OPERANDS}},
 // {"inc" , modifyOpcode_sOperand, 0x04, 0x05, 0x06, 0x08},
 // {"dec" , modifyOpcode_sOperand, 0x14, 0x15, 0x16, 0x18},
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
  _8051Instructions *instructionPtr;

  token = getToken(tokenizer);
  if(token->type != TOKEN_IDENTIFIER_TYPE)
    throwException(ERR_EXPECTING_IDENTIFIER, token,
    "Expecting an identifier, received %s instead", token->str);

  while(stricmp(token->str, instructionsTable[i].instruction)) {
    if(instructionsTable[i++].instruction == NULL)
      throwException(ERR_INVALID_INSTRUCTION, token,
      "An invalid instruction '%s' is inputted", token->str);
  }
  instructionPtr = &instructionsTable[i];
  if(instructionPtr->function) {
    funcPtr instructionToOpcode = instructionPtr->function;
    opcode = instructionToOpcode(tokenizer, instructionPtr);
  }else {
    checkExtraToken(token, tokenizer);
    return instructionPtr->data[0];
  }
  freeToken(token);
  return opcode;
}

//yet to be done
int assembleMovOperation(Tokenizer *tokenizer, _8051Instructions *info) {
  Token *token1, *token2, *token3;
  int opcode;
  token1 = getToken(tokenizer);
  token2 = getToken(tokenizer);
  token3 = getToken(tokenizer);
  if(isOperatorToken(token3, "#"))
    info->data[0] -= 0x70;
  if(isIdentifierToken(token3, "A"))
    info->data[0] += 0x10;
   
  if(isIdentifierToken(token1, "A")) {
    pushBackToken(tokenizer, token1);
    opcode = assembleAwithOperands(tokenizer, info->data[0]);
  }else if(tolower(token1->str[0]) == 'r') {
    if(isIntegerToken(token3)) {
      pushBackToken(tokenizer, token1);
      opcode = assembleRegWithOperands(tokenizer, info->data[0] - 0x40);
    }else {
      pushBackToken(tokenizer, token1);
      opcode = assembleRegWithOperands(tokenizer, info->data[0]);
    }
  }else if(isIntegerToken(token1)) {
    if(isOperatorToken(token3, "#") || isIdentifierToken(token3, "A")){
      pushBackToken(tokenizer, token1);
      opcode = assembleDirectWithOperands(tokenizer, info->data[0]);
    }else {
      pushBackToken(tokenizer, token1);
      opcode = assembleDirectWithOperands(tokenizer, info->data[0] - 0x60);
    }
  }else if(isOperatorToken(token1, "@")) {
    if(isIntegerToken(token3)) {
      pushBackToken(tokenizer, token1);
      opcode = assembleIndirectWithOperands(tokenizer, info->data[0] - 0x40);
    }else {
      pushBackToken(tokenizer, token1);
      opcode = assembleIndirectWithOperands(tokenizer, info->data[0]);
    }
  }else
    throwException(ERR_INVALID_OPERAND, token1,
    "An invalid operand of %s has been inputted", token1->str);

  freeToken(token2);
  freeToken(token3);
  return opcode;
}

int assembleArithAndLogicalOperation(Tokenizer *tokenizer, _8051Instructions *info) {
  Token *token;
  int opcode;
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & A_WITH_OPERANDS)
      opcode = assembleAwithOperands(tokenizer, info->data[0]);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIdentifierToken(token, "C")) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & C_WITH_BIT)
      opcode = assembleCwithBit(tokenizer, info->data[0] + 0x30);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & DIRECT_WITH_OPERANDS)
      opcode = assembleDirectWithAandImmediateOnly(tokenizer, info->data[0]);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  return opcode;
}

int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode) {
  Token *token;
  int regNum;
  token = getToken(tokenizer);
  freeToken(token);
  token = getToken(tokenizer);
  if(tolower(token->str[0]) == 'r')
    regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
  else
    throwException(ERR_EXPECTING_REGISTER, token,
    "Expecting a register, received %s instead", token->str);

  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getNewToken(tokenizer, token);
  if(isIdentifierToken(token, "A"))                                   //Acc
    opcode = (opcode | 0x06) + regNum;
  else if(isIntegerToken(token))                                      //direct
    opcode = (((opcode | 0x06) + regNum) << 8) | ((IntegerToken *)token)->value;
  else if(isOperatorToken(token, "#")) {                              //Immediate
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = (((opcode | 0x06) + regNum) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  checkExtraToken(token, tokenizer);
  return opcode;
}

int assembleRegWithOperands(Tokenizer *tokenizer, int opcode) {
  Token *token;
  token = getToken(tokenizer);
  int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getNewToken(tokenizer, token);
  if(isIdentifierToken(token, "A"))                                   //A
    opcode = (opcode | 0x08) + regNum;
  else if(isOperatorToken(token, "#")) {                              //Immediate
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = (((opcode | 0x08) + regNum) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }else if(isIntegerToken(token))                                     //Direct
    opcode = (((opcode | 0x08) + regNum) << 8) | ((IntegerToken *)token)->value;
  else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  checkExtraToken(token, tokenizer);
  return opcode;
}

int assembleAwithOperands(Tokenizer *tokenizer, int opcode) {
  Token *token;
  token = getToken(tokenizer);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getNewToken(tokenizer, token);
  if(isIntegerToken(token))                                           //direct
    opcode = ((opcode | 0x05) << 8) | ((IntegerToken *)token)->value;
  else if(tolower(token->str[0]) == 'r') {                            //register
    int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
    opcode = (opcode | 0x08) + regNum;
  }else if(isOperatorToken(token, "#")) {                             //immediate
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = ((opcode | 0x04) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }else if(isOperatorToken(token, "@")) {                             //indirect
    token = getNewToken(tokenizer, token);
    if(tolower(token->str[0]) == 'r') {
      int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
      opcode = (opcode | 0x06) + regNum;
    }else
      throwException(ERR_EXPECTING_REGISTER, token,
      "Expecting a register, received %s instead", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  checkExtraToken(token, tokenizer);
  return opcode;
}

//for orl, anl, ... (logical instructions)
int assembleDirectWithAandImmediateOnly(Tokenizer *tokenizer, int opcode) {
  Token *token;
  token = getToken(tokenizer);
  int direct = ((IntegerToken *)token)->value;
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getNewToken(tokenizer, token);
  if(isIdentifierToken(token, "A"))                            //A
    opcode = ((opcode | 0x02) << 8) | direct;
  else if(isOperatorToken(token, "#")) {                       //immediate
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = ((opcode | 0x03) << 16) | (direct << 8) |
      (((IntegerToken *)token)->value);
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  checkExtraToken(token, tokenizer);
  return opcode;
}

//for mov instruction
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode) {
  Token *token;
  token = getToken(tokenizer);
  int direct = ((IntegerToken *)token)->value;
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getNewToken(tokenizer, token);
  if(isIdentifierToken(token, "A"))                            //A
    opcode = ((opcode | 0x05) << 8) | direct;
  else if(tolower(token->str[0]) == 'r') {                     //Reg
    int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
    opcode = (((opcode | 0x08) + regNum) << 8) | direct;
  }else if(isIntegerToken(token))                              //Direct
    opcode = ((opcode | 0x05) << 16) | (((IntegerToken *)token)->value) << 8 |
    direct;
  else if(isOperatorToken(token, "#")) {                       //Immediate
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = ((opcode | 0x05) << 16) | direct << 8 |
      (((IntegerToken *)token)->value);
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }else if(isOperatorToken(token, "@")) {                      //Indirect
    token = getNewToken(tokenizer, token);
    if(tolower(token->str[0]) == 'r') {
      int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
      opcode = (((opcode | 0x06) + regNum) << 8) | direct;
    }else
      throwException(ERR_EXPECTING_REGISTER, token,
      "Expecting a register, received %s instead", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  checkExtraToken(token, tokenizer);
  return opcode;
}

int assembleCwithBit(Tokenizer *tokenizer, int opcode) {
  Token *token;
  token = getToken(tokenizer);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIntegerToken(token))                                    //bit
    opcode = ((opcode | 0x02) << 8) | ((IntegerToken *)token)->value;
  else if(isOperatorToken(token, "/")) {                       //barBit
    token = getNewToken(tokenizer, token);
    if(isIntegerToken(token))
      opcode = ((opcode + 0x30) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting an integer, received '%s' instead", token->str);
  }
  checkExtraToken(token, tokenizer);
  return opcode;
}

int isIdentifierToken(Token *token, char *identifier) {
  if(token->type == TOKEN_IDENTIFIER_TYPE)
    if(stricmp(token->str, identifier))
      return 0;
    else
      return 1;
  else
    return 0;
}

int isIntegerToken(Token* token) {
  if(token->type == TOKEN_INTEGER_TYPE)
    if(((IntegerToken* )token)->value > 0xFF)
    throwException(ERR_INTEGER_OUT_OF_RANGE, token,
    "Expecting integer of range 0x00-0xFF, received %x instead", ((IntegerToken *)token)->value);
    else
      return 1;
  else
    return 0;
}

int isOperatorToken(Token *token, char *Operator) {
  if(token->type == TOKEN_OPERATOR_TYPE)
    if(strcmp(token->str, Operator))
      return 0;
    else
      return 1;
  else
    return 0;
}

int extractNum(char *start, Token *token) {
  int num;
  char *end;
  num = strtol(start, &end, 10);
  if ( (start[0] == end[0]) || (end[0]) )
    throwException(ERR_INVALID_REGISTER, token,
    "An invalid register '%s' is inputted", token->str);
  else
    return num;
}

int verifyValidRegisterRangeAndReturnRegisterNumber(Token *token, int addrMode) {
  int regNum;

  regNum = extractNum(token->str+1, token);

  if(addrMode == REGISTER_ADDRESSING) {
    if(regNum > 7)
      throwException(ERR_REG_OUT_OF_RANGE, token,
      "Register %s is out of range, expecting register of R0-R7", token->str);
  }else if(addrMode == INDIRECT_ADDRESSING) {
    if(regNum > 1)
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token,
      "Register indirect %s is out of range, expecting register of R0-R1", token->str);
  }
  return regNum;
}

void checkExtraToken(Token *token, Tokenizer *tokenizer) {
  freeToken(token);
  token = getToken(tokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_NEWLINE_TYPE)
    throwException(ERR_EXTRA_PARAMETER, token,
    "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}

Token* getNewToken(Tokenizer *tokenizer, Token *token) {
  freeToken(token);
  return token = getToken(tokenizer);
}