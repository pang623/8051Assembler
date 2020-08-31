#include "Assembler8051.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

_8051Instructions instructionsTable[45] = {
  {"nop" , NULL                                        , {0x00, 0}},
  {"ret" , NULL                                        , {0x22, 0}},
  {"reti", NULL                                        , {0x32, 0}},
  {"jbc" , assembleBitWithRel                          , {0x10, 0}},
  {"jb"  , assembleBitWithRel                          , {0x20, 0}},
  {"jnb" , assembleBitWithRel                          , {0x30, 0}},
  {"div" , assembleSingleOperand                       , {0x80, OPERAND_AB}},
  {"mul" , assembleSingleOperand                       , {0xA0, OPERAND_AB}},
  {"swap", assembleSingleOperand                       , {0xC0, OPERAND_A}},
  {"da"  , assembleSingleOperand                       , {0xD0, OPERAND_A}},
  {"rl"  , assembleSingleOperand                       , {0x20, OPERAND_A_ROT}},
  {"rlc" , assembleSingleOperand                       , {0x30, OPERAND_A_ROT}},
  {"rr"  , assembleSingleOperand                       , {0x00, OPERAND_A_ROT}},
  {"rrc" , assembleSingleOperand                       , {0x10, OPERAND_A_ROT}},
  {"ljmp", assembleSingleOperand                       , {0x00, OPERAND_DIR16}},
  {"lcall",assembleSingleOperand                       , {0x10, OPERAND_DIR16}},
  {"ajmp", assembleSingleOperand                       , {0x01, OPERAND_DIR11}},
  {"acall",assembleSingleOperand                       , {0x11, OPERAND_DIR11}},
  {"jc " , assembleSingleOperand                       , {0x40, OPERAND_REL}},
  {"jnc" , assembleSingleOperand                       , {0x50, OPERAND_REL}},
  {"jz"  , assembleSingleOperand                       , {0x60, OPERAND_REL}},
  {"jnz" , assembleSingleOperand                       , {0x70, OPERAND_REL}},
  {"sjmp", assembleSingleOperand                       , {0x80, OPERAND_REL}},
  {"setb", assembleSingleOperand                       , {0x90, OPERAND_C | OPERAND_BIT}},
  {"clr" , assembleSingleOperand                       , {0xE0, OPERAND_C | OPERAND_BIT | OPERAND_A}},
  {"cpl" , assembleSingleOperand                       , {0xF0, OPERAND_C | OPERAND_BIT | OPERAND_A}},
  {"push", assembleSingleOperand                       , {0xC0, OPERAND_DIR_STACK}},
  {"pop" , assembleSingleOperand                       , {0xD0, OPERAND_DIR_STACK}},
  {"inc" , assembleSingleOperand                       , {0x00, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND | OPERAND_DPTR}},
  {"dec" , assembleSingleOperand                       , {0x10, OPERAND_A | OPERAND_REG | OPERAND_DIR | OPERAND_IND}},
  {"add" , assembleInstructionWithOnlyAccAsFirstOperand, {0x20, A_DIR | A_IMM | A_IND | A_REG}},
  {"addc", assembleInstructionWithOnlyAccAsFirstOperand, {0x30, A_DIR | A_IMM | A_IND | A_REG}},
  {"subb", assembleInstructionWithOnlyAccAsFirstOperand, {0x90, A_DIR | A_IMM | A_IND | A_REG}},
  {"orl" , assembleLogicalInstructionWithoutXRL        , {0x40, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}},
  {"anl" , assembleLogicalInstructionWithoutXRL        , {0x50, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM | C_BIT | C_BARBIT}},
  {"xrl" , assembleXRLinstruction                      , {0x60, A_DIR | A_IMM | A_IND | A_REG | DIR_A | DIR_IMM}},
  {"xch" , assembleInstructionWithOnlyAccAsFirstOperand, {0xC0, A_DIR | A_IND | A_REG}},
  {"xchd", assembleInstructionWithOnlyAccAsFirstOperand, {0xD0, A_IND}},
  {"jmp" , assembleJMPInstruction                      , {0, 0}},
  {"cjne", assembleCJNEInstruction                     , {0, 0}},
  {"djnz", assembleDJNZInstruction                     , {0, 0}},
  {"movc", assembleMOVCInstruction                     , {0, 0}},
  {"movx", assembleMOVXInstruction                     , {0, 0}},
  {"mov" , assembleMOVInstruction                      , {0, 0}},
  {NULL  , NULL                                        , {0, 0}},
};

int muteOnNoLabel = 0;
int lineNumber = 0;

uint8_t codeMemory[65536];
FILE *fileHandler;
DoubleLinkedList *listPtr;

void assembleInFileAndWriteToOutFile(char *inFile, char *outFile) {
  int totalBytes = assembleFile(inFile);
  saveCodeToBin(outFile, codeMemory, totalBytes);
}

int assembleFile(char *filename) {
  int totalBytes;
  listPtr = doubleLinkedListCreateList();
  for(int pass = 1; pass < 3; pass++) {
    lineNumber = 0;
    if((fileHandler = fopen(filename, "r")) == NULL) {
      printf("Error opening file!\n");
      exit(1);
    }
    muteOnNoLabel = !(pass - 1);
    totalBytes = assembleInstructions(getNextInstructionLine);
    fclose(fileHandler);
  }
  doubleLinkedListFreeList(listPtr, freeLabelInfo);
  return totalBytes;
}

int assembleInstructions(InstructionLineReader lineReader) {
  Tokenizer *tokenizer;
  char *line;
  uint8_t *codePtr = codeMemory;
  uint8_t **codePtrPtr = &codePtr;
  int totalLen = 0;

  for(int i = 0; i < 65536; i++)
    codeMemory[i] = 0;
  
  while((line = lineReader()) != NULL) {
    if(isspace(*line))
      continue;
    else {
      tokenizer = createTokenizer(line);
      totalLen += assembleInstruction(tokenizer, codePtrPtr);
    }
  }
  return totalLen;
}

char *getNextInstructionLine() {
  char buffer[1024];
  char *line;
  
  if(fgets(buffer, 1024, fileHandler) != NULL) {
    line = strdup(buffer);
    lineNumber += 1;
    return line;
  }else
    return NULL;
}

int assembleInstruction(Tokenizer *tokenizer, uint8_t **codePtrPtr) {
  Token* token;
  int opcode, len;
  int i = 0;
  _8051Instructions *instructionPtr;

  token = getToken(tokenizer);
  if(token->type != TOKEN_IDENTIFIER_TYPE)
    throwException(ERR_EXPECTING_IDENTIFIER, token,
    "Expecting an identifier, received %s instead", token->str);

  while(stricmp(token->str, instructionsTable[i].instruction)) {
    if(instructionsTable[i].instruction == NULL) {
      if(isOperatorTokenThenConsume(tokenizer, ":")) {
        if(muteOnNoLabel)
          recordLabel(token->str, (*codePtrPtr) - codeMemory, lineNumber);
        token = getToken(tokenizer);
        i = 0;
      }else
        throwException(ERR_INVALID_INSTRUCTION, token,
        "An invalid instruction '%s' is inputted", token->str);
    }else
      i++;
  }
  if(isOperatorTokenThenConsume(tokenizer, ":")) {
    throwException(ERR_ILLEGAL_LABEL, token,
    "Instruction mnemonic '%s' cannot be used as a label", token->str);
  }else {
    instructionPtr = &instructionsTable[i];
    if(instructionPtr->function) {
      funcPtr instructionToOpcode = instructionPtr->function;
      len = instructionToOpcode(tokenizer, instructionPtr, codePtrPtr);
    }else {                                            //handling instructions with no operand (eg: RET)
      uint8_t *codePtr = *codePtrPtr;
      checkExtraToken(tokenizer);
      len = writeCodeToCodeMemory(instructionPtr->data[0], codePtr);
      (*codePtrPtr) += len;
      return len;
    }
  }
  freeToken(token);
  return len;
}

void recordLabel(char *label, int index, int lineNo) {
  LabelInfo info = {label, index, lineNo};
  LabelInfo *infoPtr = createLabelInfo(&info);
  LabelInfo *nextInfoPtr;
  ListItem *itemPtr = doubleLinkedListCreateListItem(infoPtr);
  ListItem *nextItem = listPtr->head;
  if(nextItem == NULL)
    doubleLinkedListAddItemToHead(listPtr, itemPtr);
  else {
    while(nextItem) {
      nextInfoPtr = nextItem->data;
      if(stricmp(label, nextInfoPtr->name))
        nextItem = nextItem->next;
      else
        throwException(ERR_DUPLICATE_LABEL, NULL,
        "Label '%s' appeared before at line %d", label, nextInfoPtr->lineNo);
    }
    doubleLinkedListAddItemToHead(listPtr, itemPtr);
  }
}

int getIndexNumber(char *label) {
  LabelInfo *infoPtr;
  ListItem *nextItem = listPtr->head;
  if(nextItem == NULL)
    return -1;
  else {
    while(nextItem) {
      infoPtr = nextItem->data;
      if(stricmp(label, infoPtr->name))
        nextItem = nextItem->next;
      else
        return infoPtr->indexNo;
    }
    return -1;
  }
}

int computeRel(Tokenizer *tokenizer, int opcode, uint8_t *codePtr) {
  Token *token;
  int relative = 0, index = 0;
  if(!muteOnNoLabel) {
    token = getToken(tokenizer);
    pushBackToken(tokenizer, token);
    if(token->type == TOKEN_IDENTIFIER_TYPE) {
      token = getToken(tokenizer);
      index = getIndexNumber(token->str);
      if(index < 0)
        throwException(ERR_INVALID_LABEL, token,
        "Label '%s' is not found in this program", token->str);
      else {
        relative = index - (getCurrentAbsoluteAddr() + getInstructionBytes(opcode));
        if(relative < -128 || relative > 255)
          throwException(ERR_INTEGER_OUT_OF_RANGE, token,
          "Label '%s' is out of the branching range", token->str);
      }
    }else if(isIntegerTokenThenConsume(tokenizer, &relative, -128, 255))
      relative = relative;
    else
      throwException(ERR_INVALID_OPERAND, token,
      "Expecting a label or integer, but received '%s' instead", token->str);
    checkExtraToken(tokenizer);
  }
  return relative;
}

int assembleDJNZInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len, regNum = 0, relative = 0, direct = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum))
    opcode = (0xD8 + regNum) << 8;
  else if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
    opcode = 0xD5 << 16 | ((uint8_t) direct) << 8;
  else
    throwInvalidDJNZFirstOperandException(tokenizer);

  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  relative = computeRel(tokenizer, opcode, codePtr);
  opcode |= (uint8_t) relative;
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleCJNEInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len, regNum = 0, direct = 0, relative = 0, immediate = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
      opcode = 0xB5 << 16 | ((uint8_t) direct) << 8;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = 0xB4 << 16 | ((uint8_t) immediate) << 8;
    else
      throwExpectingIntOrImmException(tokenizer);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255);
    opcode = (0xB8 + regNum) << 16 | ((uint8_t) immediate) << 8;
  }else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255);
    opcode = (0xB6 + regNum) << 16 | ((uint8_t) immediate) << 8;
  }else
    throwInvalidCJNEFirstOperandException(tokenizer);

  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  relative = computeRel(tokenizer, opcode, codePtr);
  opcode |= (uint8_t) relative;
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleBitWithRel(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len, bitAddr = 0, relative = 0;
  uint8_t *codePtr = *codePtrPtr;

  verifyIsIntegerTokenThenConsume(tokenizer, &bitAddr, 0, 255);
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  opcode = (info->data[0] << 16) | ((uint8_t) bitAddr << 8);
  relative = computeRel(tokenizer, opcode, codePtr);
  opcode |= (uint8_t) relative;
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleJMPInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len;
  uint8_t *codePtr = *codePtrPtr;
  
  verifyIsOperatorTokenThenConsume(tokenizer, "@");
  verifyIsIdentifierTokenThenConsume(tokenizer, "A");
  verifyIsOperatorTokenThenConsume(tokenizer, "+");
  verifyIsIdentifierTokenThenConsume(tokenizer, "DPTR");
  opcode = 0x73;
  checkExtraToken(tokenizer);
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

//only MOV uses this function
int assembleMOVInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len, regNum = 0, direct = 0, directSrc = 0, immediate = 0, bitAddr = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum))
      opcode = 0xE8 + regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
      opcode = 0xE5 << 8 | ((uint8_t) direct);
    else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum))
      opcode = 0xE6 + regNum;
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = 0x74 << 8 | ((uint8_t) immediate);
    else
      throwAWithInvalidOperandException(tokenizer);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = 0xF8 + regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
      opcode = ((0xA8 + regNum) << 8) | ((uint8_t) direct);
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = ((0x78 + regNum) << 8) | ((uint8_t) immediate);
    else
      throwRegWithInvalidOperandException(tokenizer);
  }else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = 0xF6 + regNum;
    else if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255))
      opcode = ((0xA6 + regNum) << 8) | ((uint8_t) direct);
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = ((0x76 + regNum) << 8) | ((uint8_t) immediate);
    else
      throwRegWithInvalidOperandException(tokenizer);
  }else if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255)) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    if(isIdentifierTokenThenConsume(tokenizer, "A"))
      opcode = (0xF5 << 8) | ((uint8_t) direct);
    else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum))
      opcode = ((0x88 + regNum) << 8) | ((uint8_t) direct);
    else if(isIntegerTokenThenConsume(tokenizer, &directSrc, 0, 255))
      opcode = (0x85 << 16) | ((uint8_t) directSrc << 8) | ((uint8_t) direct);
    else if(isIndRegisterThenGetItsNumberAndConsume(tokenizer, &regNum))
      opcode = ((0x86 + regNum) << 8) | ((uint8_t) direct);
    else if(isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -128, 255))
      opcode = (0x75 << 16) | ((uint8_t) direct << 8) | ((uint8_t) immediate);
    else if(isIdentifierTokenThenConsume(tokenizer, "C"))
      opcode = (0x92 << 8) | ((uint8_t) direct);
    else
      throwDirectWithInvalidOperandException(tokenizer);
  }else if(isIdentifierTokenThenConsume(tokenizer, "C")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsIntegerTokenThenConsume(tokenizer, &bitAddr, 0, 255);
    opcode = (0xA2 << 8) | ((uint8_t) bitAddr);
  }else if(isIdentifierTokenThenConsume(tokenizer, "DPTR")) {
    verifyIsOperatorTokenThenConsume(tokenizer, ",");
    verifyIsImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, -32768, 65535);      //16 bits immediate
    opcode = (0x90 << 16) | ((uint16_t) immediate);
  }else
    throwInvalidMovOperand(tokenizer);

  checkExtraToken(tokenizer);
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleMOVXInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len, regNum = 0;
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
  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleMOVCInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  int opcode, len;
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
  len = writeCodeToCodeMemory(opcode, codePtr);
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
  }else if(isIntegerTokenThenConsume(tokenizer, &value, 0, 255)) {
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
  }else if(isIntegerTokenThenConsume(tokenizer, &value, 0, 255)) {
    pushBackToken(tokenizer, token);
    opcode = assembleDirectWithOperands(tokenizer, info->data[0], info->data[1]);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "Expecting an 'A' or integer, received %s instead", token->str);

  len = writeCodeToCodeMemory(opcode, codePtr);
  (*codePtrPtr) += len;
  return len;
}

int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr) {
  Token *token;
  int opcode, len, min = 0, max = 255, regNum = 0, value = 0;
  uint8_t *codePtr = *codePtrPtr;

  if(info->data[1] & OPERAND_REL)
    min = -128;
  else if(info->data[1] & OPERAND_DIR16)
    max = 65535;
  else if(info->data[1] & OPERAND_DIR11)
    max = 2047;

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
  }else if(isIntegerTokenThenConsume(tokenizer, &value, min, max)) {
    if(info->data[1] & OPERAND_DIR_STACK || info->data[1] & OPERAND_REL)
      opcode = (info->data[0] << 8) | ((uint8_t) value);
    else if(info->data[1] & OPERAND_DIR)
      opcode = ((info->data[0] | 0x05) << 8) | ((uint8_t) value);
    else if(info->data[1] & OPERAND_BIT)
      opcode = ((((info->data[0] ^ 0x40) | (((info->data[0] ^ 0x10) & 0x10) << 2))
               & (((info->data[0] & 0x10) << 1) | 0xDF)) | 0x02) << 8 | ((uint8_t) value);
    else if(info->data[1] & OPERAND_DIR16)
      opcode = ((info->data[0] | 0x02) << 16) | ((uint16_t) value);
    else if(info->data[1] & OPERAND_DIR11)
      opcode = (((value & 0xF00) >> 3 | info->data[0]) << 8) | (value & 0xFF);
    else
      throwInvalidOperandException(token);
  }else if(isIdentifierTokenThenConsume(tokenizer, "DPTR")) {
    if(info->data[1] & OPERAND_DPTR)
      opcode = 0xA3;
    else
      throwInvalidOperandException(token);
  }else if(isIdentifierTokenThenConsume(tokenizer, "C")) {
    if(info->data[1] & OPERAND_C)
      opcode = (((info->data[0] ^ 0x40) | (((info->data[0] ^ 0x10) & 0x10) << 2))
               & (((info->data[0] & 0x10) << 1) | 0xDF)) | 0x03;
    else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "The operand '%s' entered is invalid", token->str);

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
  if(isIntegerTokenThenConsume(tokenizer, &direct, 0, 255)) {
    if(flags & A_DIR)
      opcode = ((opcode | 0x05) << 8) | ((uint8_t) direct);
    else
      throwInvalidOperandException(token);
  }else if(isRegisterConsumeAndGetItsNumber(tokenizer, REGISTER_ADDRESSING, &regNum)) {
    if(flags & A_REG)
      opcode = (opcode | 0x08) + regNum;
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "#")) {
    if(flags & A_IMM) {
      verifyIsIntegerTokenThenConsume(tokenizer, &immediate, -128, 255);
      opcode = ((opcode | 0x04) << 8) | ((uint8_t) immediate);
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
    "The operand '%s' inputted is not a valid operand", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags) {
  Token *token;
  int direct = 0, immediate = 0;
  verifyIsIntegerTokenThenConsume(tokenizer, &direct, 0, 255);
  verifyIsOperatorTokenThenConsume(tokenizer, ",");
  token = getToken(tokenizer);
  pushBackToken(tokenizer, token);
  if(isIdentifierTokenThenConsume(tokenizer, "A")) {
    if(flags & DIR_A)
      opcode = ((opcode | 0x02) << 8) | ((uint8_t) direct);
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "#")) {
    if(flags & DIR_IMM) {
      verifyIsIntegerTokenThenConsume(tokenizer, &immediate, -128, 255);
      opcode = ((opcode | 0x03) << 16) | ((uint8_t) direct) << 8 | ((uint8_t) immediate);
    }else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "The operand '%s' inputted is not a valid operand", token->str);

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
  if(isIntegerTokenThenConsume(tokenizer, &bitAddr, 0, 255)) {
    if(flags & C_BIT)
      opcode = ((opcode | 0x02) << 8) | ((uint8_t) bitAddr);
    else
      throwInvalidOperandException(token);
  }else if(isOperatorTokenThenConsume(tokenizer, "/")) {
    if(flags & C_BARBIT) {
      verifyIsIntegerTokenThenConsume(tokenizer, &bitAddr, 0, 255);
      opcode = ((opcode + 0x30) << 8) | ((uint8_t) bitAddr);
    }else
      throwInvalidOperandException(token);
  }else
    throwException(ERR_INVALID_OPERAND, token,
    "The operand '%s' inputted is not a valid operand", token->str);

  freeToken(token);
  checkExtraToken(tokenizer);
  return opcode;
}

//consume token if is integer token and in valid range
//if token is not +, -, or integer, pushback the token
//throw exception if token next to '+' and '-' is not integer
//throw exception if integer token is out of range
int isIntegerTokenThenConsume(Tokenizer *tokenizer, int *val, int min, int max) {
  Token *token;
  int number;

  if(isOperatorTokenThenConsume(tokenizer, "+")) {
    token = getToken(tokenizer);
    if(token->type == TOKEN_INTEGER_TYPE) {
      if(((IntegerToken* )token)->value < min || ((IntegerToken* )token)->value > max)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token,
        "Expecting integer of range %d to %d, received %d instead", min, max, ((IntegerToken *)token)->value);
      else {
        *val = ((IntegerToken *)token)->value;
        return 1;
      }
    }else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting integer, but received %s instead", token->str);
  }else if(isOperatorTokenThenConsume(tokenizer, "-")) {
    token = getToken(tokenizer);
    if(token->type == TOKEN_INTEGER_TYPE) {
      number = (~(((IntegerToken* )token)->value)) + 1;
      if(number < min || number > max)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token,
        "Expecting integer of range %d to %d, received %d instead", min, max, number);
      else {
        *val = number;
        return 1;
      }
    }else
      throwException(ERR_EXPECTING_INTEGER, token,
      "Expecting integer, but received %s instead", token->str);
  }else {
    token = getToken(tokenizer);
    if(token->type == TOKEN_INTEGER_TYPE) {
      if(((IntegerToken* )token)->value < min || ((IntegerToken* )token)->value > max)
        throwException(ERR_INTEGER_OUT_OF_RANGE, token,
        "Expecting integer of range %d to %d, received %d instead", min, max, ((IntegerToken *)token)->value);
      else {
        *val = ((IntegerToken *)token)->value;
        return 1;
      }
    }else {
      pushBackToken(tokenizer, token);
      return 0;
    }
  }
}

//consume token if is integer token and in valid range, else throwException
void verifyIsIntegerTokenThenConsume(Tokenizer *tokenizer, int *value, int min, int max) {
  Token *token;
  int val = 0;
  if(!isIntegerTokenThenConsume(tokenizer, &val, min, max)) {
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
int isImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int min, int max) {
  int immediate = 0;

  if(isOperatorTokenThenConsume(tokenizer, "#")) {
    verifyIsIntegerTokenThenConsume(tokenizer, &immediate, min, max);
    *value = immediate;
    return 1;
  }else
    return 0;
}

//throw exception if # is not detected, which means it is not immediate
void verifyIsImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int min, int max) {
  int immediate = 0;
  Token *token;
  if(!isImmediateThenGetsItsValueAndConsume(tokenizer, &immediate, min, max)) {
    token = getToken(tokenizer);
    throwException(ERR_EXPECTING_IMMEDIATE, token,
    "Expecting immediate, but received %s instead", token->str);
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
  if(token->type != TOKEN_NULL_TYPE && token->type != TOKEN_NEWLINE_TYPE && (strcmp(token->str, ";")))
    throwException(ERR_EXTRA_PARAMETER, token,
    "Does not expect an extra parameter '%s'", token->str);
  else
    freeToken(token);
}

int writeCodeToCodeMemory(int opcode, uint8_t *codePtr) {
  int bytes;
  bytes = getInstructionBytes(opcode);

  int pos = bytes;

  for(int i = 0; i < bytes; i++) {
    codePtr[i] = (opcode >> ((pos - 1) * 8)) & 0xFF;
    pos--;
  }
  return bytes;
}

//this exception is thrown when a set of valid operands is entered,
//but the flags for this operand is not set for that certain instruction
//eg : xchd has only A, @Ri set of operands, if A, Rn or others is inputted,
//this exception will be thrown
void throwInvalidOperandException(Token *token) {
   throwException(ERR_INVALID_OPERAND, token,
   "Did not expect %s as an operand here", token->str);
}

int getInstructionBytes(int opcode) {
  if(opcode <= 0xFF)
    return 1;
  else if(opcode <= 0xFFFF)
    return 2;
  else if(opcode <= 0xFFFFFF)
    return 3;
}