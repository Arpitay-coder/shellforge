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

        case TOKEN_REDIRECT_APPEND:
            return "REDIRECT_APPEND";

        default:
            return "UNKNOWN";
    }
}
