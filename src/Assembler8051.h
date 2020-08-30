#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include "DoubleLinkedList.h"

#define     getCurrentAbsoluteAddr()    codePtr - codeMemory

//addressing mode
#define     REGISTER_ADDRESSING         100
#define     INDIRECT_ADDRESSING         200

typedef struct _8051Instructions _8051Instructions;
typedef struct LabelInfo LabelInfo;
typedef int (*funcPtr)(Tokenizer *tokenizer, _8051Instructions *instructionPtr, uint8_t **codePtrPtr);
typedef char *(*InstructionLineReader)();

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int data[3];
};

struct LabelInfo {
  char *name;
  int indexNo;
  int lineNo;
};

/*
functions yet to be tested
  void assembleInFileAndWriteToOutFile(char *inFile, char *outFile);
  int assembleFile(char *filename);
  int assembleInstructions(InstructionLineReader lineReader);
  char *getNextInstructionLine();
  
  int assembleDJNZInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
  int assembleCJNEInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
  int assembleJMPInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
  int assembleBitWithRel(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
  
  (tested partially, haven't test instructions with rel (LABEL) because not yet implement)
  int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
*/

void assembleInFileAndWriteToOutFile(char *inFile, char *outFile);
int assembleFile(char *filename);
int assembleInstructions(InstructionLineReader lineReader);
char *getNextInstructionLine();
int assembleInstruction(Tokenizer *tokenizer, uint8_t **codePtrPtr);
int assembleMOVInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleMOVCInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleMOVXInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleDJNZInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleCJNEInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleJMPInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleBitWithRel(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleInstructionWithOnlyAccAsFirstOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleLogicalInstructionWithoutXRL(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleXRLinstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleAWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleCWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int isOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator);
void verifyIsOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator);
int isIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier);
void verifyIsIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier);
int isIntegerTokenThenConsume(Tokenizer *tokenizer, int *value, int min, int max);
void verifyIsIntegerTokenThenConsume(Tokenizer *tokenizer, int *value, int min, int max);
int isRegisterConsumeAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number);
void verifyIsRegisterConsumeAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number);
int isIndRegisterThenGetItsNumberAndConsume(Tokenizer *tokenizer, int *number);
int isImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int min, int max);
void verifyIsImmediateThenGetsItsValueAndConsume(Tokenizer *tokenizer, int *value, int min, int max);
int extractNum(char *start, Token *token);
void checkExtraToken(Tokenizer *tokenizer);
void throwInvalidOperandException(Token *token);
int writeCodeToCodeMemory(int opcode, uint8_t *codePtr);
void recordLabel(char *label, int index, int lineNo);
int getIndexNumber(char *label);
int getInstructionBytes(int opcode);
LabelInfo *createLabelInfo(char *label, int index, int lineNo);
void freeLabelInfo(void *info);

#endif // ASSEMBLER8051_H
