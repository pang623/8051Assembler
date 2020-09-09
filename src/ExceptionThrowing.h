#ifndef EXCEPTIONTHROWING_H
#define EXCEPTIONTHROWING_H

#include "Error8051.h"
#include "Errors.h"
#include "Token.h"
#include "Tokenizer.h"
#include "CExceptionConfig.h"
#include "CException.h"
#include "Exception.h"

void throwMovAWithInvalidOperandException(Tokenizer *tokenizer);
void throwMovRegWithInvalidOperandException(Tokenizer *tokenizer);
void throwMovDirectWithInvalidOperandException(Tokenizer *tokenizer);
void throwInvalidMovFirstOperand(Tokenizer *tokenizer);
void throwExpectingRegisterOrDPTRexception(Tokenizer *tokenizer);
void throwExpectingAOrIndException(Tokenizer *tokenizer);
void throwExpectingPCorDPTRexception(Tokenizer *tokenizer);
void throwExpectingIntOrImmException(Tokenizer *tokenizer);
void throwInvalidCJNEFirstOperandException(Tokenizer *tokenizer);
void throwInvalidDJNZFirstOperandException(Tokenizer *tokenizer);

#endif // EXCEPTIONTHROWING_H
