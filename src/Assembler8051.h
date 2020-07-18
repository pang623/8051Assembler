#ifndef ASSEMBLER8051_H
#define ASSEMBLER8051_H

#include "Tokenizer.h"

int assembleInstruction(Tokenizer* tokenizer);
int addA(Token *token, Tokenizer* tokenizer);
int getARegister(int regs);
void get_A_Immediate(int immediate, Token* token);

#endif // ASSEMBLER8051_H
