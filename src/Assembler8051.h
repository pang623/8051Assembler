#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

//funcPtr is a pointer to function taking in (Token *token, Tokenizer *tokenizer, uint16_t opcode) 
//returning integer
typedef int (*funcPtr)(Token *token, Tokenizer *tokenizer, uint16_t opcode);
typedef struct _8051Instructions _8051Instructions;

struct _8051Instructions {
  char *instruction;
  funcPtr function;
  uint16_t opcode;
};

int assembleInstruction(Tokenizer *tokenizer);
int modifyOpcode_A(Token *token, Tokenizer *tokenizer, uint16_t opcode);
uint8_t getRegister(Token *token, uint8_t opcode);
uint16_t getImmediate(Token *token, uint16_t opcode);
uint16_t getDirect(Token *token, uint16_t opcode);
void checkExtraToken(Token *token, Tokenizer *tokenizer);
Token* getNewToken(Tokenizer *tokenizer, Token *token);

#endif // ASSEMBLER8051_H
