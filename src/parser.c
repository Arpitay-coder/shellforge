#include <stdio.h>
#include <string.h>

#include "parser.h"


/* ============================================================
   COMMAND INITIALIZATION
   ============================================================ */

static void command_init(Command *command)
{
    int i;

    command->argc = 0;

    command->input[0] = '\0';
    command->output[0] = '\0';

    command->append = 0;
    command->background = 0;

    for (i = 0; i < MAX_ARGS; i++)
    {
        command->argv[i][0] = '\0';
    }
}


/* ============================================================
   PIPELINE INITIALIZATION
   ============================================================ */

void pipeline_init(Pipeline *pipeline)
{
    int i;

    pipeline->command_count = 1;

    for (i = 0; i < MAX_COMMANDS; i++)
    {
        command_init(&pipeline->commands[i]);
    }
}


/* ============================================================
   PARSER
   ============================================================ */

int parse(TokenList *tokens, Pipeline *pipeline)
{
    int current = 0;
    int i;

    pipeline_init(pipeline);

    for (i = 0; i < tokens->count; i++)
    {
        Token *token = &tokens->tokens[i];


        /* ----------------------------------------------------
           END OF TOKENS
           ---------------------------------------------------- */

        if (token->type == TOKEN_END)
        {
            break;
        }


        /* ----------------------------------------------------
           PIPE |
           ---------------------------------------------------- */

        if (token->type == TOKEN_PIPE)
        {
            if (pipeline->commands[current].argc == 0)
            {
                printf("Parser Error: Empty command before pipe.\n");
                return 1;
            }

            if (current + 1 >= MAX_COMMANDS)
            {
                printf("Parser Error: Too many commands.\n");
                return 1;
            }

            current++;

            pipeline->command_count++;

            continue;
        }


        /* ----------------------------------------------------
           INPUT REDIRECTION <
           ---------------------------------------------------- */

        if (token->type == TOKEN_REDIRECT_IN)
        {
            if (i + 1 >= tokens->count ||
                tokens->tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Parser Error: Missing input file after '<'.\n");
                return 1;
            }

            strcpy(
                pipeline->commands[current].input,
                tokens->tokens[i + 1].value
            );

            i++;

            continue;
        }


        /* ----------------------------------------------------
           OUTPUT REDIRECTION >
           ---------------------------------------------------- */

        if (token->type == TOKEN_REDIRECT_OUT)
        {
            if (i + 1 >= tokens->count ||
                tokens->tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Parser Error: Missing output file after '>'.\n");
                return 1;
            }

            strcpy(
                pipeline->commands[current].output,
                tokens->tokens[i + 1].value
            );

            pipeline->commands[current].append = 0;

            i++;

            continue;
        }


        /* ----------------------------------------------------
           APPEND REDIRECTION >>
           ---------------------------------------------------- */

        if (token->type == TOKEN_REDIRECT_APPEND)
        {
            if (i + 1 >= tokens->count ||
                tokens->tokens[i + 1].type != TOKEN_WORD)
            {
                printf("Parser Error: Missing output file after '>>'.\n");
                return 1;
            }

            strcpy(
                pipeline->commands[current].output,
                tokens->tokens[i + 1].value
            );

            pipeline->commands[current].append = 1;

            i++;

           continue;
        }


        /* ----------------------------------------------------
           BACKGROUND &
           ---------------------------------------------------- */

        if (token->type == TOKEN_BACKGROUND)
        {
            pipeline->commands[current].background = 1;

            continue;
        }


        /* ----------------------------------------------------
           WORD
           ---------------------------------------------------- */

        if (token->type == TOKEN_WORD)
        {
            if (pipeline->commands[current].argc >= MAX_ARGS)
            {
                printf("Parser Error: Too many arguments.\n");
                return 1;
            }

            strcpy(
                pipeline->commands[current]
                    .argv[
                        pipeline->commands[current].argc
                    ],
                token->value
            );

            pipeline->commands[current].argc++;

            continue;
        }


        /* ----------------------------------------------------
           UNKNOWN TOKEN
           ---------------------------------------------------- */

        printf("Parser Error: Unknown token.\n");
        return 1;
    }


    /* --------------------------------------------------------
       CHECK LAST COMMAND
       -------------------------------------------------------- */

    if (pipeline->commands[current].argc == 0)
    {
        printf("Parser Error: Empty command.\n");
        return 1;
    }


    return 0;
}


/* ============================================================
   PRINT PIPELINE
   ============================================================ */

void pipeline_print(const Pipeline *pipeline)
{
    int i;
    int j;

    printf("\n");
    printf("========== PIPELINE ==========\n");

    for (i = 0; i < pipeline->command_count; i++)
    {
        const Command *command = &pipeline->commands[i];

        printf("\nCommand %d\n", i + 1);
        printf("------------------------------\n");

        printf("Arguments\n");

        for (j = 0; j < command->argc; j++)
        {
            printf(
                "argv[%d] = %s\n",
                j,
                command->argv[j]
            );
        }


        if (command->input[0] != '\0')
        {
            printf("Input : %s\n", command->input);
        }
        else
        {
            printf("Input : None\n");
        }


        if (command->output[0] != '\0')
        {
            printf("Output : %s\n", command->output);
        }
        else
        {
            printf("Output : None\n");
        }


        printf(
            "Append : %s\n",
            command->append ? "Yes" : "No"
        );


        printf(
            "Background : %s\n",
            command->background ? "Yes" : "No"
        );
    }

    printf("\n==============================\n");
}
