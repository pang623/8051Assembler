#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Flags.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

_8051Instructions instructionsTable[] = {
  {"nop" , NULL                            , {0x00, 0, 0}},
  {"ret" , NULL                            , {0x22, 0, 0}},
  {"reti", NULL                            , {0x32, 0, 0}},
  {"div" , assembleSingleOperand           , {0x80, OPERAND_AB, 0}},
  {"mul" , assembleSingleOperand           , {0xA0, OPERAND_AB, 0}},
  {"swap", assembleSingleOperand           , {0xC0, OTHER_A   , 0}},
  {"da"  , assembleSingleOperand           , {0xD0, OTHER_A   , 0}},
  {"rl"  , assembleSingleOperand           , {0x20, ROTATE_A  , 0}},
  {"rlc" , assembleSingleOperand           , {0x30, ROTATE_A  , 0}},
  {"rr"  , assembleSingleOperand           , {0x00, ROTATE_A  , 0}},
  {"rrc" , assembleSingleOperand           , {0x10, ROTATE_A  , 0}},
  {"push", assembleSingleOperand           , {0xC0, STACK_DIRECT, 0}},
  {"pop" , assembleSingleOperand           , {0xD0, STACK_DIRECT, 0}},
  {"inc" , assembleSingleOperand           , {0x00, OTHER_A | OPERAND_REG | OTHER_DIRECT | OPERAND_INDIRECT, 0}},
  {"dec" , assembleSingleOperand           , {0x10, OTHER_A | OPERAND_REG | OTHER_DIRECT | OPERAND_INDIRECT, 0}},
  {"add" , assembleArithAndLogicalOperation, {0x20, A_WITH_OPERANDS, A_WITH_OPERANDS}},
  {"addc", assembleArithAndLogicalOperation, {0x30, A_WITH_OPERANDS, A_WITH_OPERANDS}},
  {"subb", assembleArithAndLogicalOperation, {0x90, A_WITH_OPERANDS, A_WITH_OPERANDS}},
  {"orl" , assembleArithAndLogicalOperation, {0x40, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_OPERANDS,
                                              A_WITH_OPERANDS | C_WITH_OPERANDS | DIRECT_A_LOGICAL | DIRECT_IMM_LOGICAL}},
  {"anl" , assembleArithAndLogicalOperation, {0x50, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_OPERANDS,
                                              A_WITH_OPERANDS | C_WITH_OPERANDS | DIRECT_A_LOGICAL | DIRECT_IMM_LOGICAL}},
  {"xrl" , assembleArithAndLogicalOperation, {0x60, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS,
                                              A_WITH_OPERANDS | DIRECT_A_LOGICAL | DIRECT_IMM_LOGICAL}},
  {"xch" , assembleArithAndLogicalOperation, {0xC0, A_WITH_OPERANDS, A_WITH_DIRECT | A_WITH_INDIRECT | A_WITH_REGISTER}},
  {"xchd", assembleArithAndLogicalOperation, {0xD0, A_WITH_OPERANDS, A_WITH_INDIRECT}},
  {"mov" , assembleMovOperation            , {0xE0, A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | C_WITH_OPERANDS | INDIRECT_WITH_OPERANDS | REGISTER_WITH_OPERANDS,
                                              A_WITH_OPERANDS | DIRECT_WITH_OPERANDS | DIRECT_A_OTHER | DIRECT_IMM_OTHER | C_WITH_BIT | INDIRECT_WITH_OPERANDS | REGISTER_WITH_OPERANDS}},
  {NULL  , NULL, {0, 0, 0}},
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
    checkExtraToken(tokenizer);
    return instructionPtr->data[0];
  }
  freeToken(token);
  return opcode;
}

//if this function is to be reused, flags will be implemented later (movx? movc?)
int assembleMovOperation(Tokenizer *tokenizer, _8051Instructions *info) {
  Token *token1, *token2, *token3, *token4;
  int opcode, code;
  token1 = getToken(tokenizer);
  token2 = getToken(tokenizer);
  token3 = getToken(tokenizer);
  token4 = getToken(tokenizer);
  if(isOperatorToken(token3, "#") || isOperatorToken(token4, "#"))          //last operand mostly is token3
    code = info->data[0] - 0x70;                                            //except for indirect as '@' takes up a token
  else if(isIdentifierToken(token3, "A") || isIdentifierToken(token4, "A"))
    code = info->data[0] + 0x10;
  else
    code = info->data[0];
  
  if(isIdentifierToken(token1, "A")) {
    pushBackToken(tokenizer, token1);
    opcode = assembleAwithOperands(tokenizer, code, info->data[2]);
  }else if(tolower(token1->str[0]) == 'r') {
    pushBackToken(tokenizer, token1);
    if(isIntegerToken(token3))
      opcode = assembleRegWithOperands(tokenizer, code - 0x40, info->data[2]);
    else
      opcode = assembleRegWithOperands(tokenizer, code, info->data[2]);
  }else if(isIntegerToken(token1)) {
    pushBackToken(tokenizer, token1);
    if(isOperatorToken(token3, "#") || isIdentifierToken(token3, "A"))
      opcode = assembleDirectWithOperands(tokenizer, code, info->data[2]);
    else
      opcode = assembleDirectWithOperands(tokenizer, code - 0x60, info->data[2]);
  }else if(isOperatorToken(token1, "@")) {
    pushBackToken(tokenizer, token1);
    if(isIntegerToken(token4))
      opcode = assembleIndirectWithOperands(tokenizer, code - 0x40, info->data[2]);
    else
      opcode = assembleIndirectWithOperands(tokenizer, code, info->data[2]);
  }else if(isIdentifierToken(token1, "C")) {
    pushBackToken(tokenizer, token1);
    opcode = assembleCwithBit(tokenizer, code - 0x40, info->data[2]);
  }else
    throwException(ERR_INVALID_OPERAND, token1,
    "An invalid operand of %s has been inputted", token1->str);

  freeToken(token2);
  freeToken(token3);
  freeToken(token4);
  return opcode;
}

int assembleArithAndLogicalOperation(Tokenizer *tokenizer, _8051Instructions *info) {
  Token *token;
  int opcode;
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & A_WITH_OPERANDS)
      opcode = assembleAwithOperands(tokenizer, info->data[0], info->data[2]);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIdentifierToken(token, "C")) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & C_WITH_OPERANDS)
      opcode = assembleCwithBit(tokenizer, info->data[0] + 0x30, info->data[2]);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {
    pushBackToken(tokenizer, token);
    if(info->data[1] & DIRECT_WITH_OPERANDS)
      opcode = assembleDirectWithOperands(tokenizer, info->data[0], info->data[2]);
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  return opcode;
}

//not yet implement operand C, DPTR, BIT, REL
int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info) {
  Token *token;
  int opcode, regNum;
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {
    if(info->data[1] & ROTATE_A)
      opcode = info->data[0] | 0x03;
    else if(info->data[1] & OTHER_A)
      opcode = info->data[0] | 0x04;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIdentifierToken(token, "AB")) {
    if(info->data[1] & OPERAND_AB)
      opcode = info->data[0] | 0x04;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(tolower(token->str[0]) == 'r') {
    if(info->data[1] & OPERAND_REG) {
      regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
      opcode = (info->data[0] | 0x08) + regNum;
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "@")) {
    if(info->data[1] & OPERAND_INDIRECT) {
      token = getToken(tokenizer);
      if(tolower(token->str[0]) == 'r') {
        regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
        opcode = (info->data[0] | 0x06) + regNum;
      }else
        throwException(ERR_EXPECTING_REGISTER, token,
        "Expecting a register, received %s instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {
    if(info->data[1] & STACK_DIRECT)
      opcode = (info->data[0] << 8) | ((IntegerToken *)token)->value;
    else if(info->data[1] & OTHER_DIRECT)
      opcode = ((info->data[0] | 0x05) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);
  
  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
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
    throwException(ERR_MISSING_COMMA, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {                                 //Acc
    if(flags & INDIRECT_WITH_A || flags & INDIRECT_WITH_OPERANDS)
      opcode = (opcode | 0x06) + regNum;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {                                    //direct
    if(flags & INDIRECT_DIRECT || flags & INDIRECT_WITH_OPERANDS)
      opcode = (((opcode | 0x06) + regNum) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "#")) {                              //Immediate
    if(flags & INDIRECT_IMM || flags & INDIRECT_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = (((opcode | 0x06) + regNum) << 8) | ((IntegerToken *)token)->value;
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);
  
  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleRegWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  token = getToken(tokenizer);
  int regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_MISSING_COMMA, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {                                 //A
    if(flags & REG_WITH_A || flags & REGISTER_WITH_OPERANDS)
      opcode = (opcode | 0x08) + regNum;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "#")) {                             //Immediate
    if(flags & REG_WITH_IMM || flags & REGISTER_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = (((opcode | 0x08) + regNum) << 8) | ((IntegerToken *)token)->value;
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {                                   //Direct
    if(flags & REG_WITH_DIRECT || flags & REGISTER_WITH_OPERANDS)
      opcode = (((opcode | 0x08) + regNum) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleAwithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int regNum;
  token = getToken(tokenizer);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_MISSING_COMMA, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIntegerToken(token))                                           //direct
    if(flags & A_WITH_DIRECT || flags & A_WITH_OPERANDS)
      opcode = ((opcode | 0x05) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  else if(tolower(token->str[0]) == 'r') {                            //register
    if(flags & A_WITH_REGISTER || flags & A_WITH_OPERANDS) {
      regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
      opcode = (opcode | 0x08) + regNum;
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "#")) {                             //immediate
    if(flags & A_WITH_IMMEDIATE || flags & A_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = ((opcode | 0x04) << 8) | ((IntegerToken *)token)->value;
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "@")) {                             //indirect
    if(flags & A_WITH_INDIRECT || flags & A_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(tolower(token->str[0]) == 'r') {
        regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
        opcode = (opcode | 0x06) + regNum;
      }else
        throwException(ERR_EXPECTING_REGISTER, token,
        "Expecting a register, received %s instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int regNum;
  token = getToken(tokenizer);
  int direct = ((IntegerToken *)token)->value;
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_MISSING_COMMA, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIdentifierToken(token, "A")) {                                 //A
    if(flags & DIRECT_A_LOGICAL)
      opcode = ((opcode | 0x02) << 8) | direct;
    else if(flags & DIRECT_A_OTHER)
      opcode = ((opcode | 0x05) << 8) | direct;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(tolower(token->str[0]) == 'r') {                            //Reg
    if(flags & DIRECT_REG || flags & DIRECT_WITH_OPERANDS) {
      regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, REGISTER_ADDRESSING);
      opcode = (((opcode | 0x08) + regNum) << 8) | direct;
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isIntegerToken(token)) {                                    //Direct
    if(flags & DIRECT_DIRECT || flags & DIRECT_WITH_OPERANDS)
      opcode = ((opcode | 0x05) << 16) | (((IntegerToken *)token)->value) << 8 |
      direct;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "#")) {                              //Immediate
    if(flags & DIRECT_IMM_LOGICAL) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = ((opcode | 0x03) << 16) | direct << 8 |
        (((IntegerToken *)token)->value);
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else if(flags & DIRECT_IMM_OTHER) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = ((opcode | 0x05) << 16) | direct << 8 |
        (((IntegerToken *)token)->value);
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "@")) {                             //Indirect
    if(flags & DIRECT_INDIRECT || flags & DIRECT_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(tolower(token->str[0]) == 'r') {
        regNum = verifyValidRegisterRangeAndReturnRegisterNumber(token, INDIRECT_ADDRESSING);
        opcode = (((opcode | 0x06) + regNum) << 8) | direct;
      }else
        throwException(ERR_EXPECTING_REGISTER, token,
        "Expecting a register, received %s instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

//flags are checked inside the function because MOV uses this function but dont
//have the operand combination of C, /bit
int assembleCwithBit(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  token = getToken(tokenizer);
  freeToken(token);
  token = getToken(tokenizer);
  if(!isOperatorToken(token, ","))
    throwException(ERR_MISSING_COMMA, token,
    "Expecting a comma , received %s instead", token->str);
  token = getToken(tokenizer);
  if(isIntegerToken(token)) {                                         //bit
    if(flags & C_WITH_BIT || flags & C_WITH_OPERANDS)
      opcode = ((opcode | 0x02) << 8) | ((IntegerToken *)token)->value;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else if(isOperatorToken(token, "/")) {                              //barBit
    if(flags & C_WITH_BARBIT || flags & C_WITH_OPERANDS) {
      token = getToken(tokenizer);
      if(isIntegerToken(token))
        opcode = ((opcode + 0x30) << 8) | ((IntegerToken *)token)->value;
      else
        throwException(ERR_EXPECTING_INTEGER, token,
        "Expecting an integer, received '%s' instead", token->str);
    }else
      throwException(ERR_INVALID_OPERAND, token,
      "Did not expect %s as an operand", token->str);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
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

void checkExtraToken(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_NEWLINE_TYPE)
    throwException(ERR_EXTRA_PARAMETER, token,
    "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}