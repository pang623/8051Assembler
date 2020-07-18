#ifndef Token_H
#define Token_H

#include <stdint.h>
#include "Common.h"
#include "CException.h"
#include "Exception.h"

typedef enum {
  UNKNOWN_SYMBOL,
  ADD_SYMBOL,
  SUB_SYMBOL,
  MUL_SYMBOL,
  DIV_SYMBOL,
  MOD_SYMBOL,
  MORE_SYMBOL,
  LESS_SYMBOL,
  BITWISE_AND_SYMBOL,
  BITWISE_OR_SYMBOL,
  BITWISE_NOT_SYMBOL,
  LOGICAL_AND_SYMBOL,
  LOGICAL_OR_SYMBOL,
  LOGICAL_NOT_SYMBOL,
  SHIFT_RIGHT_SYMBOL,
  SHIFT_LEFT_SYMBOL,
  MORE_EQUAL_SYMBOL,
  LESS_EQUAL_SYMBOL,
  OPEN_GROUPING_SYMBOL,
  CLOSED_GROUPING_SYMBOL,
  INTEGER_SYMBOL,
  FLOAT_SYMBOL,
  IDENTIFIER_SYMBOL,
} Symbol;

typedef enum {
  TOKEN_UNKNOWN_TYPE,
  TOKEN_NULL_TYPE,
  TOKEN_INVALID_TYPE,
  TOKEN_INTEGER_TYPE,
  TOKEN_OPERATOR_TYPE,
  TOKEN_FLOAT_TYPE,
  TOKEN_STRING_TYPE,
  TOKEN_IDENTIFIER_TYPE,
  TOKEN_NEWLINE_TYPE,
  TOKEN_END_OF_FILE_TYPE,
} TokenType;

typedef enum {
  NO_AFFIX,
  PREFIX,
  INFIX,
  POSTFIX,
} Affix;

typedef enum {
  NO_ASSOCIATIVITY,
  LEFT_TO_RIGHT,
  RIGHT_TO_LEFT,
} Associativity;

typedef struct {
  TokenType type:16;
  Symbol symbol:16;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
} Token;

typedef struct {
  TokenType type:16;
  Symbol symbol:16;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  int value;
} IntegerToken;

typedef struct {
  TokenType type:16;
  Symbol symbol:16;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  double value;
} FloatToken;

typedef struct {
  TokenType type:16;
  Symbol symbol:16;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
} IdentifierToken, StringToken, CharConstToken;

typedef struct {
  TokenType type:16;
  Symbol symbol:16;
  uint32_t startColumn;
  uint32_t length;
  char *originalStr;         ///
  char *str;
  Token *token[0];
} OperatorToken;

Token *createNullToken();
Token *createNewlineToken();
Token *createInvalidToken(char *originalString, int start, int len);
Token *cloneTokenAsInvalidToken(Token *token);
Token *createIntegerToken(int value, char *originalString);
Token *createFloatToken(double value, char *originalString);
Token *createIdentifierToken(char *str, char *originalString);
Token *createStringToken(char *str, char *originalString);
Token *createOperatorToken(char *str, char *originalString);
void freeToken(void *token);
void dumpTokenErrorMessage(CEXCEPTION_T ex, int lineNo);

#endif // Token_H

