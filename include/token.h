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
    TOKEN_REDIRECT_APPEND,
    TOKEN_BACKGROUND,
    TOKEN_END
} TokenType;

typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_LENGTH];

} Token;


typedef struct
{
    Token tokens[MAX_TOKENS];
    int count;

} TokenList;


const char *token_type_to_string(TokenType type);

void token_print(const TokenList *list);

#endif
