#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"

//addressing mode
#define     REGISTER_ADDRESSING     100
#define     INDIRECT_ADDRESSING     200

//funcPtr is a pointer to function taking in (Tokenizer *tokenizer, _8051Instructions *instructionPtr) 
//returning integer
typedef struct _8051Instructions _8051Instructions;
typedef int (*funcPtr)(Tokenizer *tokenizer, _8051Instructions *instructionPtr);

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int data[3];
};

int assembleAllInstruction(Tokenizer *tokenizer);
int assembleMOVCInstruction(Tokenizer *tokenizer, _8051Instructions *info);
int assembleInstructionWithOnlyAccAsFirstOperand(Tokenizer *tokenizer, _8051Instructions *info);
int assembleLogicalInstructionWithoutXRL(Tokenizer *tokenizer, _8051Instructions *info);
int assembleXRLinstruction(Tokenizer *tokenizer, _8051Instructions *info);
//int assembleMovOperation(Tokenizer *tokenizer, _8051Instructions *info);
//int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info);
//int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode, int flags);
//int assembleRegWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleAWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleCWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int extractNum(char *start, Token *token);
void checkExtraToken(Tokenizer *tokenizer);
int isOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator);
void verifyIsOperatorTokenThenConsume(Tokenizer *tokenizer, char *Operator);
int isIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier);
void verifyIsIdentifierTokenThenConsume(Tokenizer *tokenizer, char *identifier);
int isIntegerTokenThenConsume(Tokenizer *tokenizer, int *value);
void verifyIsIntegerTokenThenConsume(Tokenizer *tokenizer, int *value);
int isRegisterAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number);
void verifyIsRegisterAndGetItsNumber(Tokenizer *tokenizer, int addrMode, int *number);
void throwInvalidOperandException(Token *token);

#endif // ASSEMBLER8051_H
