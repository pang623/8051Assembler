#include "Assembler8051.h"
#include "Exception.h"
#include "Error.h"
#include "Token.h"
#include "Tokenizer.h"
#include <stdio.h>

int assembleInstruction (Tokenizer* tokenizer) {
	Token* token;
	int opcode;

	token = getToken(tokenizer);
	if (token->type != TOKEN_IDENTIFIER_TYPE)
		throwException(ERR_EXPECTING_IDENTIFIER, token, "The first operand is not an identifier");
	else{
		if(((IdentifierToken *)token)->str[1] == 'd') {			//check first operand
			freeToken(token);
      token = getToken(tokenizer);
      opcode = addA(token, tokenizer);
    }//other instructions   else if() {
//    }
  }
  return opcode;
}

int addA(Token *token, Tokenizer* tokenizer) {
  if(((IdentifierToken *)token)->str[0] != 'A' || ((IdentifierToken *)token)->str[1] != NULL)
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
        if(((IdentifierToken *)token)->str[1] == NULL)
          throwException(ERR_INVALID_REGISTER, token, "Invalid Register", token->str);
        else if(((IdentifierToken *)token)->str[2] != NULL)
          throwException(ERR_INVALID_REGISTER, token, "Register %s is invalid", token->str);
        else {
        uint8_t opcode;
        int reg = getARegister(token->str[1]);
        switch(reg) {
					case 0: opcode = 0x28;
									break;
					case 1: opcode = 0x29;
									break;
					case 2: opcode = 0x2A;
									break;
					case 3: opcode = 0x2B;
									break;
					case 4: opcode = 0x2C;
									break;
					case 5: opcode = 0x2D;
									break;
					case 6: opcode = 0x2E;
									break;
					case 7: opcode = 0x2F;
									break;
					default: throwException(ERR_INVALID_REGISTER, token, "Register %s is invalid", token->str);
				}
        freeToken(token);
				token = getToken(tokenizer);
				if(token->str != NULL)
					throwException(ERR_EXTRA_PARAMETER, token, "Does not expect an extra parameter");
				else {
          freeToken(token);
					return opcode;
        }
        }
      }else if(((CharConstToken *)token)->str[0] == '#') {
          freeToken(token);
					token = getToken(tokenizer);
					if(token->type != TOKEN_INTEGER_TYPE)
						throwException(ERR_INVALID_INTEGER, token, "Immediate %s is invalid", token->str);
          else{
            uint16_t opcode;
						get_A_Immediate(((IntegerToken *)token)->value, token);
						opcode = 0x2400 + ((IntegerToken *)token)->value;
						freeToken(token);
						token = getToken(tokenizer);
						if(token->str != NULL)
							throwException(ERR_EXTRA_PARAMETER, token, "Does not expect an extra parameter");
						else {
              freeToken(token);
							return opcode;
            }
          }
      }
    }
  }
}

int getARegister(int regs) {
    switch(regs) {
      case '0': regs = 0;
            break;
      case '1': regs = 1;
            break;
      case '2': regs = 2;
            break;
      case '3': regs = 3;
            break;
      case '4': regs = 4;
            break;
      case '5': regs = 5;
            break;
      case '6': regs = 6;
            break;
      case '7': regs = 7;
            break;
      default: regs = 100;
    }
	return regs;
}

void get_A_Immediate(int immediate, Token* token) {
	if(immediate < 0x00 || immediate > 0xFF)
		throwException(ERR_IMMEDIATE_OUT_OF_RANGE, token, "Immediate %x is out of range", immediate);
}
