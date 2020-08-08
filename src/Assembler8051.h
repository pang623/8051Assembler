#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

//flags
#define     A_WITH_OPERANDS          (1 << 0)
#define     DIRECT_WITH_OPERANDS     (1 << 1)
#define     C_WITH_BIT               (1 << 2)
#define     INDIRECT_WITH_OPERANDS   (1 << 3)
#define     REGISTER_WITH_OPERANDS   (1 << 4)
#define     DIRECT_WITH_A_AND_IMM    (1 << 5)
#define     C_WITH_BARBIT            (1 << 6)

//single operand flags
#define     ROTATE_A                 (1 << 7)
#define     OTHER_A                  (1 << 8)
#define     OPERAND_AB               (1 << 9)
#define     OPERAND_REG              (1 << 10)
#define     OPERAND_INDIRECT         (1 << 11)
#define     STACK_DIRECT             (1 << 12)
#define     OTHER_DIRECT             (1 << 13)
#define     OPERAND_C                (1 << 14)

//addressing mode
#define     REGISTER_ADDRESSING     100
#define     INDIRECT_ADDRESSING     200

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
int assembleSingleOperand(Tokenizer *tokenizer, _8051Instructions *info);
int assembleIndirectWithOperands(Tokenizer *tokenizer, int opcode);
int assembleRegWithOperands(Tokenizer *tokenizer, int opcode);
int assembleDirectWithAandImmediateOnly(Tokenizer *tokenizer, int opcode);
int assembleDirectWithOperands(Tokenizer *tokenizer, int opcode);
int assembleAwithOperands(Tokenizer *tokenizer, int opcode);
int assembleCwithBit(Tokenizer *tokenizer, int opcode, int flags);
int verifyValidRegisterRangeAndReturnRegisterNumber(Token *token, int addrMode);
int extractRegNum(char *start, Token *token);
void checkExtraToken(Tokenizer *tokenizer);
int isOperatorToken(Token *token, char *Operator);
int isIdentifierToken(Token *token, char *identifier);
int isIntegerToken(Token *token);

#endif // ASSEMBLER8051_H
