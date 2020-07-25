#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

//funcPtr is a pointer to function taking in (Token *token, Tokenizer *tokenizer, uint16_t opcode) 
//returning integer
typedef int (*funcPtr)(Token *token, Tokenizer *tokenizer, int opcode);
typedef struct _8051Instructions _8051Instructions;

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  int opcode;
};

int assembleInstruction(Tokenizer *tokenizer);
int destAorDirect(Token *token, Tokenizer *tokenizer, int opcode);
int modifyOpcode_sOperand(Token *token, Tokenizer *tokenizer, int opcode);
int modifyOpcode_Direct(Token *token, Tokenizer *tokenizer, int opcode);
int modifyOpcode_A(Token *token, Tokenizer *tokenizer, int opcode);
uint8_t getRegister(Token *token, uint8_t opcode);
int getImmediate(Token *token, int opcode);
int getDirect(Token *token, int opcode);
void checkExtraToken(Token *token, Tokenizer *tokenizer);
Token* getNewToken(Tokenizer *tokenizer, Token *token);

#endif // ASSEMBLER8051_H
