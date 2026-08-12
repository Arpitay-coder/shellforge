#ifndef LEXER_H
#define LEXER_H

#include "token.h"

int lexer_tokenize(const char *input, Token tokens[], int max_tokens);

#endif
