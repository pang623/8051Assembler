#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

#define     RegAddr     8
#define     IndAddr     6

//funcPtr is a pointer to function taking in (Token *token, Tokenizer *tokenizer, uint16_t opcode) 
//returning integer
typedef struct _8051Instructions _8051Instructions;
typedef int (*funcPtr)(Token *token, Tokenizer *tokenizer, _8051Instructions *opcPtr);

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int op1;
  int op2;
  int op3;
  int op4;
  int flags;
};

int assembleInstruction(Tokenizer *tokenizer);
//int destAorDirect(Token *token, Tokenizer *tokenizer, int opcode);
int modifyOpcode_sOperand(Token *token, Tokenizer *tokenizer, _8051Instructions *opcPtr);
//int modifyOpcode_Direct(Token *token, Tokenizer *tokenizer, int opcode);
int modifyOpcode_A(Token *token, Tokenizer *tokenizer, _8051Instructions *opcPtr);
uint8_t verifyValidRegisterRangeAndReturnRespectiveOpcode(Token *token, uint8_t opcode);
int getImmediate(Token *token, int opcode);
int getDirect(Token *token, int opcode);
void checkExtraToken(Token *token, Tokenizer *tokenizer);
Token* getNewToken(Tokenizer *tokenizer, Token *token);
int isOperatorToken(Token *token, char *operators);
int extractRegNum(char *start, Token *token);

#endif // ASSEMBLER8051_H
