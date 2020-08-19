#include "Assembler8051.h"
#include "Exception.h"
#include "ExceptionThrowing.h"
#include "Error.h"
#include "Flags.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
/*
_8051Instructions instructionsTable[] = {
  {"nop" , NULL                                        , {0x00, 0}},
  {"ret" , NULL                                        , {0x22, 0}},
  {"reti", NULL                                        , {0x32, 0}},
  {"div" , assembleSingleOperand                       , {0x80, OPERAND_AB}},
  {"mul" , assembleSingleOperand                       , {0xA0, OPERAND_AB}},
  {"swap", assembleSingleOperand                       , {0xC0, OPERAND_A}},
  {"da"  , assembleSingleOperand                       , {0xD0, OPERAND_A}},
  {"rl"  , assembleSingleOperand                       , {0x20, OPERAND_A_ROT}},
  {"rlc" , assembleSingleOperand                       , {0x30, OPERAND_A_ROT}},
  {"rr"  , assembleSingleOperand                       , {0x00, OPERAND_A_ROT}},
  {"rrc" , assembleSingleOperand                       , {0x10, OPERAND_A_ROT}},
  {"push", assembleSingleOperand                       , {0xC0, OPERAND_DIR_STACK}},
  {"pop" , assembleSingleOperand                       , {0xD0, OPERAND_DIR_STACK}},
  {"inc" , assembleSingleOperand                       , {0x00, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}},
  {"dec" , assembleSingleOperand                       , {0x10, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}},
  {"add" , assembleInstructionWithOnlyAccAsFirstOperand, {0x20, A_DIR | A_IMM | A_IND | A_REG}},
  {"addc", assembleInstructionWithOnlyAccAsFirstOperand, {0x30, A_DIR | A_IMM | A_IND | A_REG}},
  {"subb", assembleInstructionWithOnlyAccAsFirstOperand, {0x90, A_DIR | A_IMM | A_IND | A_REG}},
  {"orl" , assembleLogicalInstructionWithoutXRL        , {0x40, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}},
  {"anl" , assembleLogicalInstructionWithoutXRL        , {0x50, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}},
  {"xrl" , assembleXRLinstruction                      , {0x60, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM}},
  {"xch" , assembleInstructionWithOnlyAccAsFirstOperand, {0xC0, A_DIR | A_IND | A_REG}},
  {"xchd", assembleInstructionWithOnlyAccAsFirstOperand, {0xD0, A_IND}},
 // {"cjne", assembleCJNEInstruction                     , {0, 0}},
  {"movc", assembleMOVCInstruction                     , {0, 0}},
  {"movx", assembleMOVXInstruction                     , {0, 0}},
  {"mov" , assembleMOVInstruction                      , {0, 0}},
  {NULL  , NULL, {0, 0}},
};

int assembleInstruction(Tokenizer *tokenizer, uint8_t **codePtrPtr) {
  Token* token;
  int opcode;
  int i = 0, len = 0;
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
    len = instructionToOpcode(tokenizer, instructionPtr, codePtrPtr);
  }else {                                            //when the instruction is with no operand (RET)
    uint8_t *codePtr = *codePtrPtr;
    checkExtraToken(tokenizer);
    len = writeCodeToCodeMemory(instructionPtr->data[0], codePtr);
    (*codePtrPtr) += len;
    return len;
  }
  freeToken(token);
  return len;
}

int assembleCJNEInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, direct = 0, relative = 0; immediate = 0;
  uint8_t *codePtr = *codePtrPtr;
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
      opcode = 0xB5 << 16 | direct << 8;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = 0xB4 << 16 | immediate << 8;
    else
      throwExpectingIntOrImmException(tokenizer);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255);
    opcode = (0xB8 + regNum) << 16 | immediate << 8;
  }else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255);
    opcode = (0xB6 + regNum) << 16 | immediate << 8;
  }else
    throwInvalidCJNEFirstOperandException(tokenizer);

  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  verifyIsIntegerTokenThenConsume(tokenizer, &relative, -128, 127);
  opcode |= relative;
  checkExtraToken(tokenizer);
  int len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

//only MOV uses this function
int assembleMOVInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, regNum = 0, direct = 0, directSrc = 0, immediate = 0, bitAddr = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum))
      opcode = 0xE8 | regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF))
      opcode = 0xE5 << 8 | direct;
    else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum))
      opcode = 0xE6 | regNum;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, 0xFF))
      opcode = 0x74 | immediate;
    else
      throwAWithInvalidOperandException(tokenizer);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = 0xF8 + regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF))
      opcode = ((0xA8 + regNum) << 8) | direct;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, 0xFF))
      opcode = ((0x78 + regNum) << 8) | immediate;
    else
      throwRegWithInvalidOperandException(tokenizer);
  }else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = 0xF6 + regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF))
      opcode = ((0xA6 + regNum) << 8) | direct;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, 0xFF))
      opcode = ((0x76 + regNum) << 8) | immediate;
    else
      throwRegWithInvalidOperandException(tokenizer);
  }else if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = (0xF5 << 8) | direct;
    else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum))
      opcode = ((0x88 + regNum) << 8) | direct;
    else if(isIntegerTokenThenConsume(tokenizer, &directSrc, 0xFF))
      opcode = (0x85 << 16) | (directSrc << 8) | direct;
    else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum))
      opcode = ((0x86 + regNum) << 8) | direct;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, 0xFF))
      opcode = (0x75 << 16) | (direct << 8) | immediate;
    else if(isIdentifierTokenThenConsume(tokenizer, "C"))
      opcode = (0x92 << 8) | direct;
    else
      throwDirectWithInvalidOperandException(tokenizer);
  }else if(isIdentifierTokenThenConsume(tokenizer, "C")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsIntegerTokenThenConsume(tokenizer, &bitAddr, 0xFF);
    opcode = (0xA2 << 8) | bitAddr;
  }else if(isIdentifierTokenThenConsume(tokenizer, "DPTR")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, 0xFFFF);
    opcode = (0x90 << 16) | immediate;
  }else
    throwInvalidMovOperand(tokenizer);

  checkExtraToken(tokenizer);
  int len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleMOVXInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, regNum = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsOperatorTokenThenConsume(tokenizer, "@");
    if(isRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum))
      opcode = 0xE2 + regNum;
    else if(isIdentifierTokenThenConsume(tokenizer, "DPTR"))
      opcode = 0xE0;
    else
      throwExpectingRegisterOrDPTRexception(tokenizer);
  }else if(isOperatorTokenThenConsume(tokenizer, "@")) {
    if(isRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum))
      opcode = 0xF2 + regNum;
    else if(isIdentifierTokenThenConsume(tokenizer, "DPTR"))
      opcode = 0xF0;
    else
      throwExpectingRegisterOrDPTRexception(tokenizer);
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsIdentifierTokenThenConsume(tokenizer, "A");
  }else
    throwExpectingAOrIndException(tokenizer);

  checkExtraToken(tokenizer);
  int len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleMOVCInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode;
  uint8_t *codePtr = *codePtrPtr;

  verifyIsIdentifierTokenThenConsume(tokenizer, "A");
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  verifyIsOperatorTokenThenConsume(tokenizer, "@");
  verifyIsIdentifierTokenThenConsume(tokenizer, "A");
  verifyIsOperatorTokenThenConsume(tokenizer, "+");
  if(isIdentifierTokenThenConsume(tokenizer, "DPTR"))
    opcode = 0x93;
  else if(isIdentifierTokenThenConsume(tokenizer, "PC"))
    opcode = 0x83;
  else
    throwExpectingPCorDPTRexception(tokenizer);

  checkExtraToken(tokenizer);
  int len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleInstructionWithOnlyAccAsFirstOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  uint8_t *codePtr = *codePtrPtr;
  
  int opcode = assembleAWithOperands(tokenizer, info->data[0], info->data[1]);
  int len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

//orl, anl
int assembleLogicalInstructionWithoutXRL(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  Token *token;
  int value, opcode, len;
  uint8_t *codePtr = *codePtrPtr;
  
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    pushBackToken(tokenizer, token);
    opcode = assembleAWithOperands(tokenizer, info->data[0], info->data[1]);
  }else if(isIdentifierTokenThenConsume(tokenizer, "C")) {
    pushBackToken(tokenizer, token);
    opcode = assembleCWithOperands(tokenizer, info->data[0] + 0x30, info->data[1]);
  }else if(isIntegerTokenThenConsume(tokenizer, &value, 0xFF)) {
    pushBackToken(tokenizer, token);
    opcode = assembleDirectWithOperands(tokenizer, info->data[0], info->data[1]);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting an 'A', 'C' or integer, received %s instead", token->str);
    
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleXRLinstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  Token *token;
  int value, opcode, len;
  uint8_t *codePtr = *codePtrPtr;
  
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    pushBackToken(tokenizer, token);
    opcode = assembleAWithOperands(tokenizer, info->data[0], info->data[1]);
  }else if(isIntegerTokenThenConsume(tokenizer, &value, 0xFF)) {
    pushBackToken(tokenizer, token);
    opcode = assembleDirectWithOperands(tokenizer, info->data[0], info->data[1]);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting an 'A' or integer, received %s instead", token->str);
    
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

//not yet implement operand C, BIT, REL
int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  Token *token;
  int opcode, len, regNum = 0, direct = 0;
  uint8_t *codePtr = *codePtrPtr;
  
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    if(info->data[1] & OPERAND_A_ROT)
      opcode = info->data[0] | 0x03;
    else if(info->data[1] & OPERAND_A)
      opcode = info->data[0] | 0x04;
    else
      throwInvalidOperandException(token);
  }else if(isIdentifierTokenThenConsume(tokenizer, "AB")) {
    if(info->data[1] & OPERAND_AB)
      opcode = info->data[0] | 0x04;
    else
      throwInvalidOperandException(token);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    if(info->data[1] & OPERAND_REG)
      opcode = (info->data[0] | 0x08) + regNum;
    else
      throwInvalidOperandException(token);
  }else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum)) {
    if(info->data[1] & OPERAND_IND)
      opcode = (info->data[0] | 0x06) + regNum;
    else
      throwInvalidOperandException(token);
  }else if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF)) {
    if(info->data[1] & OPERAND_DIR_STACK)
      opcode = (info->data[0] << 8) | direct;
    else if(info->data[1] & OPERAND_DIR)
      opcode = ((info->data[0] | 0x05) << 8) | direct;
    else
      throwInvalidOperandException(token);
  }else if(isIdentifierTokenThenConsume(tokenizer, "DPTR")) {
    if(info->data[1] & OPERAND_DPTR)
      opcode = 0xA3;
    else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "An invalid operand of %s has been inputted", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleAWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int regNum = 0, immediate = 0, direct = 0;
  verifyIsIdentifierTokenThenConsume(tokenizer, "A");
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIntegerTokenThenConsume(tokenizer, &direct, 0xFF)) {
    if(flags & A_DIR)
      opcode = ((opcode | 0x05) << 8) | direct;
    else
      throwInvalidOperandException(token);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    if(flags & A_REG)
      opcode = (opcode | 0x08) + regNum;
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "#")) {
    if(flags & A_IMM) {
      verifyIsIntegerTokenThenConsume(tokenizer, &immediate, 0xFF);
      opcode = ((opcode | 0x04) << 8) | immediate;
    }else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "@")) {
    if(flags & A_IND) {
      verifyIsRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
      opcode = (opcode | 0x06) + regNum;
    }else
        throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting register, integer, '#' or '@', received %s instead", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

//yet to implement direct, rel    bit, rel
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int direct = 0, immediate = 0;
  verifyIsIntegerTokenThenConsume(tokenizer, &direct, 0xFF);
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    if(flags & DIR_A)
      opcode = ((opcode | 0x02) << 8) | direct;
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "#")) {
    if(flags & DIR_IMM) {
      verifyIsIntegerTokenThenConsume(tokenizer, &immediate, 0xFF);
      opcode = ((opcode | 0x03) << 16) | direct << 8 | immediate;
    }else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting A or '#', received %s instead", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleCWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int bitAddr = 0;
  verifyIsIdentifierTokenThenConsume(tokenizer, "C");
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIntegerTokenThenConsume(tokenizer, &bitAddr, 0xFF)) {
    if(flags & C_BIT)
      opcode = ((opcode | 0x02) << 8) | bitAddr;
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "/")) {
    if(flags & C_BARBIT) {
      verifyIsIntegerTokenThenConsume(tokenizer, &bitAddr, 0xFF);
      opcode = ((opcode + 0x30) << 8) | bitAddr;
    }else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting integer or '/', but received %s instead", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}
*/
//consume token if is integer token and in valid range, else pushBack the token
int isIntegerTokenThenConsume(Tokenizer *tokenizer, uint8_t *value, int min, int max) {
  Token *token, *token1;
  int number;
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  
  if(isOperatorTokenThenConsume(tokenizer, "+")) {
    token1 = getToken(tokenizer);
    if(token1->type == TOKEN_INTEGER_TYPE) {
      if(((IntegerToken* )token1)->value > max || ((IntegerToken* )token1)->value < min)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token1,
        "Expecting integer of range %d to %d, received %d instead", min, max, ((IntegerToken *)token1)->value);
      else {
        *value = ((IntegerToken *)token1)->value;
        return 1;
      }
    }else {
      pushBackToken(tokenizer, token1);
      pushBackToken(tokenizer, token);
      return 0;
    }
  }else if(isOperatorTokenThenConsume(tokenizer, "-")) {
    token1 = getToken(tokenizer);
    if(token1->type == TOKEN_INTEGER_TYPE) {
      number = (~(((IntegerToken* )token1)->value)) + 1;
      if(number < min || number > max)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token1,
        "Expecting integer of range %d to %d, received %d instead", min, max, number);
      else {
        *value = number;
        return 1;
      }
    }else {
      pushBackToken(tokenizer, token1);
      pushBackToken(tokenizer, token);
      return 0;
    }
  }else {
    token = getToken(tokenizer);
    if(token->type == TOKEN_INTEGER_TYPE) {
      if(((IntegerToken* )token)->value > max || ((IntegerToken* )token)->value < min)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token,
        "Expecting integer of range %d to %d, received %d instead", min, max, ((IntegerToken *)token)->value);
      else {
        *value = ((IntegerToken *)token)->value;
        return 1;
      }
    }else {
      pushBackToken(tokenizer, token);
      return 0;
    }
  }
}
/*
//consume token if is integer token and in valid range, else throwException
void verifyIsIntegerTokenThenConsume(Tokenizer *tokenizer, int *value, int min, int max) {
  Token *token;
  int val = 0;
  if(!isIntegerTokenThenConsume(tokenizer, &val, integerRange)) {
    token = getToken(tokenizer);
    throwException(ERR_EXPECTING_INTEGER, token,
    "Expecting integer, but received %s instead", token->str);
  }else
    *value = val;
}

//consume token if is correct identifier token, else pushBack the token
int isIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier) {
  Token *token;
  token = getToken(tokenizer);
  if(token->type == TOKEN_IDENTIFIER_TYPE) {
    if(stricmp(token->str, identifier)) {
      pushBackToken(tokenizer, token);
      return 0;
    }else
      return 1;
  }else {
    pushBackToken(tokenizer, token);
    return 0;
  }
}

//consume token if is correct identifier token, else throwException
void verifyIsIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier) {
  Token *token;
  if(!isIdentifierTokenThenConsume(tokenizer, identifier)) {
    token = getToken(tokenizer);
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting '%s' as identifier, but received %s instead", identifier, token->str);
  }
}
*/
//consume token if is correct operator token, else pushBack the token
int isOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator) {
  Token *token;
  token = getToken(tokenizer);
  if(token->type == TOKEN_OPERATOR_TYPE) {
    if(strcmp(token->str, Operator)) {
      pushBackToken(tokenizer, token);
      return 0;
    }else
      return 1;
  }else {
    pushBackToken(tokenizer, token);
    return 0;
  }
}
/*
//consume token if is correct operator token, else throwException
void verifyIsOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator) {
  Token *token;
  if(!isOperatorTokenThenConsume(tokenizer, Operator)) {
    token = getToken(tokenizer);
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting '%s' as operator, but received %s instead", Operator, token->str);
  }
}

//consume token if is register and get reg num, else pushback token and return 0
//does not throw exception if its not register
//return 1 if register is valid and in range
int isRegisterConsumeAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number) {
  Token *token;
  token = getToken(tokenizer);

  if(token->type != TOKEN_IDENTIFIER_TYPE || (!(tolower(token->str[0]) == 'r'))) {
    pushBackToken(tokenizer, token);
    return 0;
  }

  *number = extractNum(token->str + 1, token);

  if(addrMode == REGISTER_ADDRESSING) {
    if(*number > 7)
      throwException(ERR_REG_OUT_OF_RANGE, token,
      "Register %s is out of range, expecting register of R0-R7", token->str);
  }else if(addrMode == INDIRECT_ADDRESSING) {
    if(*number > 1)
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token,
      "Register indirect %s is out of range, expecting register of R0-R1", token->str);
  }
  return 1;
}

//throw exception if token is not register, else consume token and get register number
void verifyIsRegisterConsumeAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number) {
  Token *token;
  int n = 0;
  if(!isRegisterConsumeAndGetItsNumber(tokenizer, addrMode, &n)) {
    token = getToken(tokenizer);
    throwException(ERR_EXPECTING_REGISTER, token,
    "Expecting a register, but received %s instead", token->str);
  }else
    *number = n;
}

//if @ token is detected, it is consumed and the token next to it must be register, else throwException
//if @ is not detected, it will NOT throwException and token is pushed back
int isIndRegisterThenGetItsNumberAndConsume(Tokenizer *tokenizer, int *number) {
  int regNum = 0;

  if(isOperatorTokenThenConsume(tokenizer, "@")) {
    verifyIsRegisterConsumeAndGetItsNumber(tokenizer, INDIRECT_ADDRESSING, &regNum);
    *number = regNum;
    return 1;
  }else
    return 0;
}

//if # token is detected, it is consumed and the token next to it must be integer, else throw exception
//if # is not detected, it will NOT throwException and token is pushed back
int isImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int integerRange) {
  int immediate = 0;

  if(isOperatorTokenThenConsume(tokenizer, "#")) {
    verifyIsIntegerTokenThenConsume(tokenizer, &immediate, integerRange);
    *value = immediate;
    return 1;
  }else
    return 0;
}

//throw exception if # is not detected
void verifyIsImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int integerRange) {
  int immediate = 0;
  Token *token;
  if(!isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, integerRange)) {
    token = getToken(tokenizer);
    throwException(ERR_EXPECTING_IMMEDIATE, token,
    "Expecting a '#', but received %s instead", token->str);
  }else
    *value = immediate;
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

void checkExtraToken(Tokenizer *tokenizer) {
  Token *token;
  token = getToken(tokenizer);
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_NEWLINE_TYPE)
    throwException(ERR_EXTRA_PARAMETER, token,
    "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}

int writeCodeToCodeMemory(int opcode, uint8_t *codePtr) {
  int bytes;
  if(opcode < 0xFF)
    bytes = 1;
  else if(opcode < 0xFFFF)
    bytes = 2;
  else if(opcode < 0xFFFFFF)
    bytes = 3;
  
  int pos = bytes;
  
  for(int i = 0; i < bytes; i++) {
    codePtr[i] = (opcode >> ((pos - 1) * 8)) & 0xFF;
    pos--;
  }
  return bytes;
}

void throwInvalidOperandException(Token *token) {
   throwException(ERR_INVALID_OPERAND, token,
   "Did not expect %s as an operand", token->str);
}*/