#include <ctype.h>
#include <string.h>

#include "lexer.h"


static void add_token(
    TokenList *list,
    TokenType type,
    const char *value
)
{
    if (list == NULL)
        return;

    if (list->count >= MAX_TOKENS)
        return;

    list->tokens[list->count].type = type;

    strncpy(
        list->tokens[list->count].value,
        value,
        MAX_TOKEN_LENGTH - 1
    );

    list->tokens[list->count]
        .value[MAX_TOKEN_LENGTH - 1] = '\0';

    list->count++;
}


int lexer_tokenize(const char *input, TokenList *list)
{
    int i = 0;

    if (input == NULL || list == NULL)
        return 0;

    /* Start with an empty token list */
    list->count = 0;


    while (input[i] != '\0' &&
           list->count < MAX_TOKENS)
    {
        /* ============================================
           Skip whitespace
           ============================================ */

        if (isspace((unsigned char)input[i]))
        {
            i++;
            continue;
        }


        /* ============================================
           Pipe |
           ============================================ */

        if (input[i] == '|')
        {
            add_token(
                list,
                TOKEN_PIPE,
                "|"
            );

            i++;

            continue;
        }


        /* ============================================
           Input redirection <
           ============================================ */

        if (input[i] == '<')
        {
            add_token(
                list,
                TOKEN_REDIRECT_IN,
                "<"
            );

            i++;

            continue;
        }


        /* ============================================
           Output redirection >
           or append >>
           ============================================ */

        if (input[i] == '>')
        {
            if (input[i + 1] == '>')
            {
                add_token(
                    list,
                    TOKEN_REDIRECT_APPEND,
                    ">>"
                );

                i += 2;
            }
            else
            {
                add_token(
                    list,
                    TOKEN_REDIRECT_OUT,
                    ">"
                );

                i++;
            }

            continue;
        }


        /* ============================================
           Background &
           ============================================ */

        if (input[i] == '&')
        {
            add_token(
                list,
                TOKEN_BACKGROUND,
                "&"
            );

            i++;

            continue;
        }


        /* ============================================
           Word
           ============================================ */

        {
            char word[MAX_TOKEN_LENGTH];

            int j = 0;


            while (
                input[i] != '\0' &&
                !isspace((unsigned char)input[i]) &&
                input[i] != '|' &&
                input[i] != '<' &&
                input[i] != '>' &&
                input[i] != '&'
            )
            {
                /* ====================================
                   Quoted text
                   ==================================== */

                if (
                    input[i] == '"' ||
                    input[i] == '\''
                )
                {
                    char quote = input[i];

                    i++;


                    while (
                        input[i] != '\0' &&
                        input[i] != quote &&
                        j < MAX_TOKEN_LENGTH - 1
                    )
                    {
                        word[j++] = input[i++];

                    }


                    /* Skip closing quote */

                    if (input[i] == quote)
                    {
                        i++;
                    }
                }

                else
                {
                    if (j < MAX_TOKEN_LENGTH - 1)
                    {
                        word[j++] = input[i];
                    }

                    i++;
                }
            }


            /* Terminate word */

            word[j] = '\0';


            /* Add word if it is not empty */

            if (j > 0)
            {
                add_token(
                    list,
                    TOKEN_WORD,
                    word
                );
            }
        }
    }


    /*
     * Add END token.
     *
     * The parser expects TOKEN_END after
     * the last real token.
     */

    if (list->count < MAX_TOKENS)
    {
        add_token(
            list,
            TOKEN_END,
            ""
        );
    }


    return list->count;
}
