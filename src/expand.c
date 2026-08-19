#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expand.h"


/* ============================================================
   EXPAND ONE STRING
   ============================================================ */

static void expand_string(
    const char *input,
    char *output,
    size_t output_size
)
{
    size_t i = 0;
    size_t j = 0;

    while (input[i] != '\0' && j < output_size - 1)
    {
        /* ----------------------------------------------------
           NORMAL CHARACTER
           ---------------------------------------------------- */

        if (input[i] != '$')
        {
            output[j++] = input[i++];
            continue;
        }


        /* ----------------------------------------------------
           '$' AT END
           ---------------------------------------------------- */

        if (input[i + 1] == '\0')
        {
            output[j++] = '$';
            i++;
            continue;
        }


        /* ----------------------------------------------------
           SPECIAL VARIABLE: $?
           ---------------------------------------------------- */

        if (input[i + 1] == '?')
        {
            const char *value = "0";

            size_t k = 0;

            while (value[k] != '\0' &&
                   j < output_size - 1)
            {
                output[j++] = value[k++];
            }

            i += 2;

            continue;
        }


        /* ----------------------------------------------------
           ENVIRONMENT VARIABLE
           ---------------------------------------------------- */

        if ((input[i + 1] >= 'A' &&
             input[i + 1] <= 'Z') ||

            (input[i + 1] >= 'a' &&
             input[i + 1] <= 'z') ||

            input[i + 1] == '_')
        {
            char variable[128];

            size_t v = 0;

            i++;

            while (
                input[i] != '\0' &&
                (
                    (input[i] >= 'A' && input[i] <= 'Z') ||
                    (input[i] >= 'a' && input[i] <= 'z') ||
                    (input[i] >= '0' && input[i] <= '9') ||
                    input[i] == '_'
                )
            )
            {
                if (v < sizeof(variable) - 1)
                {
                    variable[v++] = input[i];
                }

                i++;
            }

            variable[v] = '\0';


            const char *value = getenv(variable);

            if (value == NULL)
            {
                value = "";
            }


            size_t k = 0;

            while (value[k] != '\0' &&
                   j < output_size - 1)
            {
                output[j++] = value[k++];
            }

            continue;
        }


        /* ----------------------------------------------------
           UNKNOWN '$'
           ---------------------------------------------------- */

        output[j++] = '$';

        i++;
    }

    output[j] = '\0';
}


/* ============================================================
   EXPAND ALL PIPELINE ARGUMENTS
   ============================================================ */

void expand_variables(Pipeline *pipeline)
{
    int i;
    int j;

    char buffer[1024];


    for (i = 0; i < pipeline->command_count; i++)
    {
        Command *command = &pipeline->commands[i];


        /* ----------------------------------------------------
           ARGUMENTS
           ---------------------------------------------------- */

        for (j = 0; j < command->argc; j++)
        {
            expand_string(
                command->argv[j],
                buffer,
                sizeof(buffer)
            );

            strcpy(
                command->argv[j],
                buffer
            );
        }


        /* ----------------------------------------------------
           INPUT FILE
           ---------------------------------------------------- */

        if (command->input[0] != '\0')
        {
            expand_string(
                command->input,
                buffer,
                sizeof(buffer)
            );

            strcpy(
                command->input,
                buffer
            );
        }


        /* ----------------------------------------------------
           OUTPUT FILE
           ---------------------------------------------------- */

        if (command->output[0] != '\0')
        {
            expand_string(
                command->output,
                buffer,
                sizeof(buffer)
            );

            strcpy(
                command->output,
                buffer
            );
        }
    }
}
