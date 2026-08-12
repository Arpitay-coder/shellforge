#ifndef TOKEN_H
#define TOKEN_H

#define MAX_TOKEN_LENGTH 256
#define MAX_TOKENS 128

typedef enum
{
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIRECT_IN,
    TOKEN_REDIRECT_OUT,
    TOKEN_REDIRECT_APPEND
} TokenType;

typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_LENGTH];
} Token;

const char *token_type_to_string(TokenType type);

#endif
