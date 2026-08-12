#include <ctype.h>
#include <string.h>
#include "lexer.h"

static void add_token(Token tokens[], int *count,
                      TokenType type, const char *value)
{
    if (*count >= MAX_TOKENS)
        return;

    tokens[*count].type = type;

    strncpy(tokens[*count].value, value, MAX_TOKEN_LENGTH - 1);
    tokens[*count].value[MAX_TOKEN_LENGTH - 1] = '\0';

    (*count)++;
}

int lexer_tokenize(const char *input, Token tokens[], int max_tokens)
{
    int count = 0;
    int i = 0;

    while (input[i] != '\0' && count < max_tokens)
    {
        /* Skip whitespace */
        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }

        /* Pipe */
        if (input[i] == '|')
        {
            add_token(tokens, &count, TOKEN_PIPE, "|");
            i++;
            continue;
        }

        /* Input redirection */
        if (input[i] == '<')
        {
            add_token(tokens, &count, TOKEN_REDIRECT_IN, "<");
            i++;
            continue;
        }

        /* Output redirection */
        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                add_token(tokens, &count,
                          TOKEN_REDIRECT_APPEND, ">>");
                i += 2;
            }
            else
            {
                add_token(tokens, &count,
                          TOKEN_REDIRECT_OUT, ">");
                i++;
            }

            continue;
        }

        /* Word */
        {
            char word[MAX_TOKEN_LENGTH];
            int j = 0;

            while (input[i] != '\0' &&
                   !isspace((unsigned char)input[i]) &&
                   input[i] != '|' &&
                   input[i] != '<' &&
                   input[i] != '>')
            {
                if ((input[i] == '"' || input[i] == '\''))
                {
                    char quote = input[i];
                    i++;

                    while (input[i] != '\0' &&
                           input[i] != quote &&
                           j < MAX_TOKEN_LENGTH - 1)
                    {
                        word[j++] = input[i++];
                    }

                    if (input[i] == quote)
                        i++;
                }
                else
                {
                    if (j < MAX_TOKEN_LENGTH - 1)
                        word[j++] = input[i];

                    i++;
                }
            }

            word[j] = '\0';

            if (j > 0)
                add_token(tokens, &count, TOKEN_WORD, word);
        }
    }

    return count;
}
