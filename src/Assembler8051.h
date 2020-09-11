#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Exception.h"
#include "ExceptionThrowing.h"
#include "Errors.h"
#include "Error8051.h"
#include "Flags.h"
#include "Token.h"
#include "Tokenizer.h"
#include "DoubleLinkedList.h"
#include "LabelInfo.h"
#include "AssembleFile.h"
#include "MemAlloc.h"
#include "SaveCodeToBin.h"

#define     getCurrentAbsoluteAddr()        (codePtr - codeMemory)
#define     isIntegerToken(token)           (token->type == TOKEN_INTEGER_TYPE)
#define     isIdentifierToken(token)        (token->type == TOKEN_IDENTIFIER_TYPE)

//addressing mode
#define     REGISTER_ADDRESSING             100
#define     INDIRECT_ADDRESSING             200

typedef struct _8051Instructions _8051Instructions;
typedef int (*funcPtr)(Tokenizer *tokenizer, _8051Instructions *instructionPtr, uint8_t **codePtrPtr);
typedef char *(*InstructionLineReader)();

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int data[3];
};

int assembleInFileAndWriteToOutFile(char *inFile, char *outFile);
int assembleInstructions(InstructionLineReader lineReader);
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
int assembleSingleOperandWithLabel(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
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
int extractNum(char *start);
void checkExtraToken(Tokenizer *tokenizer);
void throwUnsupportedOperandException(Token *token);
int writeCodeToCodeMemory(int opcode, uint8_t *codePtr);
int getInstructionBytes(int opcode);
void recordLabel(char *label, int index, int lineNo);
int getLabelIndex(char *label);
int getAbsoluteAddress(Tokenizer *tokenizer, int min, int max);
int getRelativeAddress(Tokenizer *tokenizer, int baseAddr, int min, int max);

#endif // ASSEMBLER8051_H