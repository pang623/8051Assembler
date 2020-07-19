#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>
#include <stdint.h>

int assembleInstruction (Tokenizer *tokenizer) {
	Token* token;
	int opcode;

	token = getToken(tokenizer);
	if (token->type != TOKEN_IDENTIFIER_TYPE)
		throwException(ERR_EXPECTING_IDENTIFIER, token, "The first operand is not an identifier");
	else{
		if(((IdentifierToken *)token)->str[1] == 'd') {			//check first operand (need to check the word 'add', but uses 'd' atm)
			freeToken(token);
      token = getToken(tokenizer);
      opcode = addA(token, tokenizer);
    }//other instructions   else if() {
//    }
  }
  return opcode;
}

int addA(Token *token, Tokenizer *tokenizer) {
  if(((IdentifierToken *)token)->str[0] != 'A' || ((IdentifierToken *)token)->str[1])
    throwException(ERR_EXPECTING_IDENTIFIER, token, "The second operand is not accumulator");			//check second operand
  else{
    freeToken(token);
		token = getToken(tokenizer);
    if(((OperatorToken *)token)->str[0] != ',')
      throwException(ERR_MISSING_COMMA, token, "The third operand is not a comma");				//check comma
    else{
      freeToken(token);
			token = getToken(tokenizer);
			if(((IdentifierToken *)token)->str[0] == 'R') {
        uint8_t opcode = 0x28;
        opcode = getRegister(token, opcode);
        checkExtraToken(token, tokenizer);
				return opcode;
      }else if(((CharConstToken *)token)->str[0] == '#') {
        uint16_t opcode = 0x2400;
        opcode = getImmediate(token, tokenizer, opcode);
        checkExtraToken(token, tokenizer);
        return opcode;
      }else if(token->type == TOKEN_INTEGER_TYPE) {
        uint16_t opcode = 0x2500;
        opcode = getDirect(token, opcode);
        checkExtraToken(token, tokenizer);
        return opcode;
      }else if(((CharConstToken *)token)->str[0] == '@') {
        freeToken(token);
        token = getToken(tokenizer);
        uint8_t opcode = 0x26;
        opcode = getRegister(token, opcode);
        checkExtraToken(token, tokenizer);
        return opcode;
      }else {
        throwException(ERR_INVALID_OPERAND, token, "Invalid operand %s", token->str);
        freeToken(token);
      }
    }
  }
}

uint8_t getRegister(Token *token, uint8_t opcode) {
  uint8_t opcodeMode = opcode;

  if((opcodeMode &= 0x0F) == 8) {
    if(!(((IdentifierToken *)token)->str[1]))
      throwException(ERR_INVALID_REGISTER, token, "Invalid Register", token->str);
    else if(((IdentifierToken *)token)->str[2])
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range", token->str);
    else if(((IdentifierToken *)token)->str[1] < '0' || ((IdentifierToken *)token)->str[1] > '7')
      throwException(ERR_REG_OUT_OF_RANGE, token, "Register %s is out of range", token->str);
    else
      opcode += ((IdentifierToken *)token)->str[1] - 48;
  }else if((opcodeMode &= 0x0F) == 6) {
    if(((IdentifierToken *)token)->str[0] != 'R')
      throwException(ERR_INVALID_OPERAND, token, "Expecting a register for indirect addressing", token->str);
    else if(!(((IdentifierToken *)token)->str[1]))
      throwException(ERR_INVALID_REGISTER, token, "Invalid register indirect", token->str);
    else if(((IdentifierToken *)token)->str[2])
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range", token->str);
    else if(((IdentifierToken *)token)->str[1] < '0' || ((IdentifierToken *)token)->str[1] > '1')
      throwException(ERR_INDIRECT_OUT_OF_RANGE, token, "Register indirect %s is out of range", token->str);
    else
      opcode += ((IdentifierToken *)token)->str[1] - 48;
  }
  return opcode;
}

//yet to implement #xxh immediate format
uint16_t getImmediate(Token *token, Tokenizer *tokenizer, uint16_t opcode) {
  freeToken(token);
  token = getToken(tokenizer);
  if(token->type != TOKEN_INTEGER_TYPE)
    throwException(ERR_INVALID_INTEGER, token, "Immediate %s is invalid", token->str);
  else if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_IMMEDIATE_OUT_OF_RANGE, token, "Immediate %x is out of range", ((IntegerToken *)token)->value);
  else
    opcode += ((IntegerToken *)token)->value;

  return opcode;
}

//yet to implement #xxh immediate format
uint16_t getDirect(Token *token, uint16_t opcode) {
  if(((IntegerToken *)token)->value < 0x00 || ((IntegerToken *)token)->value > 0xFF)
    throwException(ERR_DIRECT_OUT_OF_RANGE, token, "Direct address %x is out of range", ((IntegerToken *)token)->value);
  else
    opcode += ((IntegerToken *)token)->value;

  return opcode;
}

void checkExtraToken(Token *token, Tokenizer *tokenizer) {
  freeToken(token);
	token = getToken(tokenizer);
	if(token->str)
		throwException(ERR_EXTRA_PARAMETER, token, "Does not expect an extra parameter");
	else
    freeToken(token);
}
