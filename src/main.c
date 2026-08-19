#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "token.h"
#include "lexer.h"
#include "parser.h"
#include "expand.h"


int main(void)
{
    char *line;

    TokenList tokens;
    Pipeline pipeline;


    printf("\n");
    printf("========================================\n");
    printf("        SHELLFORGE - MILESTONE 2.2\n");
    printf("        Parser & Expand\n");
    printf("========================================\n");
    printf("\n");


    while (1)
    {
        /* Read command from user */

        line = readline("shellforge$ ");


        /* Ctrl+D / EOF */

        if (line == NULL)
        {
            printf("\nGoodbye!\n");
            break;
        }


        /* Ignore empty input */

        if (strlen(line) == 0)
        {
            free(line);
            continue;
        }


        /* Exit command */

        if (strcmp(line, "exit") == 0)
        {
            free(line);

            printf("Exiting...\n");

            break;
        }


        /* Add command to history */

        add_history(line);


        /* ====================================================
           LEXER
           ==================================================== */

        lexer_tokenize(
            line,
            &tokens
        );


        /* ====================================================
           DISPLAY TOKENS
           ==================================================== */

        token_print(&tokens);


        /* ====================================================
           PARSER
           ==================================================== */

        if (parse(
                &tokens,
                &pipeline
            ) != 0)
        {
            printf("Parsing failed.\n");

            free(line);

            continue;
        }


        /* ====================================================
           EXPAND VARIABLES
           ==================================================== */

        expand_variables(&pipeline);


        /* ====================================================
           DISPLAY PIPELINE
           ==================================================== */

        pipeline_print(&pipeline);


        /* Free input */

        free(line);
    }


    return 0;
}
