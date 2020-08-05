#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

#define     A_WITH_OPERANDS          (1 << 0)
#define     DIRECT_WITH_OPERANDS     (1 << 1)
#define     C_WITH_BIT               (1 << 2)
#define     INDIRECT_WITH_OPERANDS   (1 << 3)
#define     REGISTER_WITH_OPERANDS   (1 << 4)

//funcPtr is a pointer to function taking in (Token *token, Tokenizer *tokenizer, uint16_t opcode) 
//returning integer
typedef struct _8051Instructions _8051Instructions;
typedef int (*funcPtr)(Tokenizer *tokenizer, _8051Instructions *instructionPtr);

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int data[2];
};

int assembleInstruction(Tokenizer *tokenizer);
int assembleArithAndLogicalOperation(Tokenizer *tokenizer, _8051Instructions *info);
int assembleMovOperation(Tokenizer *tokenizer, _8051Instructions *info);
int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode);
int assembleRegWithOperands(Tokenizer *tokenizer, int opcode);
int assembleDirectWithAandImmediateOnly(Tokenizer *tokenizer, int opcode);
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode);
int assembleAwithOperands(Tokenizer *tokenizer, int opcode);
int assembleCwithBit(Tokenizer *tokenizer, int opcode);
//int modifyOpcode_sOperand(Token *token, Tokenizer *tokenizer, _8051Instructions *opcPtr);
int verifyValidRegisterRangeAndReturnRegisterNumber(Token *token, int addrMode);
int extractRegNum(char *start, Token *token);
void checkExtraToken(Token *token, Tokenizer *tokenizer);
Token* getNewToken(Tokenizer *tokenizer, Token *token);
int isOperatorToken(Token *token, char *Operator);
int isIdentifierToken(Token *token, char *identifier);
int isIntegerToken(Token *token);

#endif // ASSEMBLER8051_H
