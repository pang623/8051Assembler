#ifndef EXCEPTIONTHROWING_H
#define EXCEPTIONTHROWING_H

#include "Tokenizer.h"

void throwAWithInvalidOperandException(Tokenizer *tokenizer);
void throwRegWithInvalidOperandException(Tokenizer *tokenizer);
void throwDirectWithInvalidOperandException(Tokenizer *tokenizer);
void throwInvalidMovOperand(Tokenizer *tokenizer);
void throwExpectingRegisterOrDPTRexception(Tokenizer *tokenizer);
void throwExpectingAOrIndException(Tokenizer *tokenizer);
void throwExpectingPCorDPTRexception(Tokenizer *tokenizer);

#endif // EXCEPTIONTHROWING_H
