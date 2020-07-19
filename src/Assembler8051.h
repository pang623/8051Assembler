#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"
#include <stdint.h>

int assembleInstruction(Tokenizer* tokenizer);
int addA(Token *token, Tokenizer* tokenizer);
uint8_t getRegister(Token* token, uint8_t opcode);
uint16_t getImmediate(Token *token, Tokenizer *tokenizer, uint16_t opcode);
void checkExtraToken(Token *token, Tokenizer *tokenizer);

#endif // ASSEMBLER8051_H
