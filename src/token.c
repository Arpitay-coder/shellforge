#include <stdio.h>

#include "token.h"


const char *token_type_to_string(TokenType type)
{
    switch (type)
    {
        case TOKEN_WORD:
            return "WORD";

        case TOKEN_PIPE:
            return "PIPE";

        case TOKEN_REDIRECT_IN:
            return "REDIRECT_IN";

        case TOKEN_REDIRECT_OUT:
            return "REDIRECT_OUT";

        case TOKEN_BACKGROUND:
    return "BACKGROUND";

case TOKEN_END:
    return "END";
        default:
            return "UNKNOWN";
    }
}


void token_print(const TokenList *list)
{
    if (list == NULL)
        return;

    printf("\n========== TOKENS ==========\n");

    for (int i = 0; i < list->count; i++)
    {
        printf(
            "[%d] %-16s \"%s\"\n",
            i,
            token_type_to_string(list->tokens[i].type),
            list->tokens[i].value
        );
    }

    printf("============================\n\n");
}
