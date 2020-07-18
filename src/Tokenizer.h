#ifndef Tokenizer_H
#define Tokenizer_H

#include <stdint.h>
#include "Common.h"
#include "Token.h"

#define TOKENIZER_MAX_PUSHBACK		        5
#define TOKENIZER_DOLLAR_SIGN_HEX         (1 << 0)
#define TOKENIZER_DISABLE_STRING_SUPPORT  (1 << 1)

typedef struct Tokenizer Tokenizer;
struct Tokenizer {
  char *str;
  int index;
  uint32_t config;
  uint32_t prevStartColumn;
  uint32_t prevLength;
 // Token *previousToken;
  int lineNum;
  int tokenIndex;
  Token *token[TOKENIZER_MAX_PUSHBACK];
};

#define getTokenizerConfiguration(tokenizer)    ((tokenizer)->config)

void reinitTokenizer(Tokenizer *tokenizer, char *str);
Tokenizer *createTokenizer(char *stringToTokenize);
Tokenizer *cloneTokenizer(Tokenizer *tokenizer);
void freeTokenizer(Tokenizer *tokenizer);
void configureTokenizer(Tokenizer *tokenizer, uint32_t configuration);
//INLINE Token *advanceToken(Tokenizer *tokenizer);
Token *getToken(Tokenizer *tokenizer);
Token *getNextToken(Tokenizer *tokenizer);
void pushBackToken(Tokenizer *tokenizer, Token *token);
//INLINE Token *peepToken(Tokenizer *tokenizer);

#endif // Tokenizer_H
