#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"

#define     getCurrentAbsoluteAddr()    codePtr - codeMemory

//addressing mode
#define     REGISTER_ADDRESSING         100
#define     INDIRECT_ADDRESSING         200

//funcPtr is a pointer to function taking in (Tokenizer *tokenizer, _8051Instructions *instructionPtr) 
//returning integer
typedef struct _8051Instructions _8051Instructions;
typedef int (*funcPtr)(Tokenizer *tokenizer, _8051Instructions *instructionPtr, uint8_t **codePtrPtr);

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int data[3];
};

int assembleInstruction(Tokenizer *tokenizer, uint8_t **codePtrPtr);
int assembleMOVInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleMOVCInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleMOVXInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
int assembleCJNEInstruction(Tokenizer *tokenizer, _8051Instructions *info, uint8_t **codePtrPtr);
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

#endif // ASSEMBLER8051_H
