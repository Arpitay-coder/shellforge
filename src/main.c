#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "lexer.h"
#include "token.h"

int main(void)
{
    printf("=====================================\n");
    printf("Shellforge\n");
    printf("A Unix Style Shell written in C\n");
    printf("=====================================\n");

    char *line;

    while (1)
    {
        line = readline("shellforge$ ");

        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }

        add_history(line);

        if (strcmp(line, "exit") == 0)
        {
            free(line);
            printf("Exiting...\n");
            break;
        }

        Token tokens[MAX_TOKENS];

        int token_count =
            lexer_tokenize(line, tokens, MAX_TOKENS);

        printf("\nTokens:\n");

        for (int i = 0; i < token_count; i++)
        {
            printf("  [%d] %-16s : %s\n",
                   i,
                   token_type_to_string(tokens[i].type),
                   tokens[i].value);
        }

        printf("\n");

        free(line);
    }

    return 0;
}
