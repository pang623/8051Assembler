#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

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

int assembleInstruction(Tokenizer *tokenizer);
int assembleArithAndLogicalOperation(Tokenizer *tokenizer, _8051Instructions *info);
int assembleMovOperation(Tokenizer *tokenizer, _8051Instructions *info);
int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info);
int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleRegWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleAwithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode, int flags);
int assembleCwithBit(Tokenizer *tokenizer, int opcode, int flags);
int verifyValidRegisterRangeAndReturnRegisterNumber(Token *token, int addrMode);
int extractRegNum(char *start, Token *token);
void checkExtraToken(Tokenizer *tokenizer);
int isOperatorToken(Token *token, char *Operator);
int isIdentifierToken(Token *token, char *identifier);
int isIntegerToken(Token *token);

#endif // ASSEMBLER8051_H
