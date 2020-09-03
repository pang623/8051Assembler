#ifndef EXCEPTIONTHROWING_H
#define EXCEPTIONTHROWING_H

#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CException.h"
#include "Exception.h"

void throwAWithInvalidOperandException(Tokenizer *tokenizer);
void throwRegWithInvalidOperandException(Tokenizer *tokenizer);
void throwDirectWithInvalidOperandException(Tokenizer *tokenizer);
void throwInvalidMovFirstOperand(Tokenizer *tokenizer);
void throwExpectingRegisterOrDPTRexception(Tokenizer *tokenizer);
void throwExpectingAOrIndException(Tokenizer *tokenizer);
void throwExpectingPCorDPTRexception(Tokenizer *tokenizer);
void throwExpectingIntOrImmException(Tokenizer *tokenizer);
void throwInvalidCJNEFirstOperandException(Tokenizer *tokenizer);
void throwInvalidDJNZFirstOperandException(Tokenizer *tokenizer);

#endif // EXCEPTIONTHROWING_H
