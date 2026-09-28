#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "../include/parser.h"

#define PARSED_INPUT_SIZE 1000

int parse_input(char *input, char *args[], int max_args) {

    int argc = 0;

    char *operators = "|<>&";
    char parsed_input[PARSED_INPUT_SIZE];

    parsed_input[0] = '\0';

    // Add spaces around operators
    // so commands like ls|grep and echo hi>file work.
    for (int i = 0; input[i] != '\0'; i++) {

        int len = strlen(parsed_input);

        // Handle >>
        if (input[i] == '>' && input[i + 1] == '>') {

            parsed_input[len] = ' ';
            parsed_input[len + 1] = '>';
            parsed_input[len + 2] = '>';
            parsed_input[len + 3] = ' ';
            parsed_input[len + 4] = '\0';

            i++;
        }

        // Handle single-character operators
        else if (strchr(operators, input[i]) != NULL) {

            parsed_input[len] = ' ';
            parsed_input[len + 1] = input[i];
            parsed_input[len + 2] = ' ';
            parsed_input[len + 3] = '\0';
        }

        // Normal character
        else {
            parsed_input[len] = input[i];
            parsed_input[len + 1] = '\0';
        }
    }

    // Expand environment variables
    char expanded_input[PARSED_INPUT_SIZE];
    int j = 0;

    for (int i = 0; parsed_input[i] != '\0' &&
                    j < PARSED_INPUT_SIZE - 1; i++) {

        if (parsed_input[i] == '$') {

            char variable[100];
            int k = 0;

            i++;

            while (parsed_input[i] != '\0' &&
                   (isalnum((unsigned char)parsed_input[i]) ||
                    parsed_input[i] == '_')) {

                if (k < 99) {
                    variable[k++] = parsed_input[i];
                }

                i++;
            }

            variable[k] = '\0';

            // Move back because the for-loop increments i again
            i--;

            if (k > 0) {

                char *value = getenv(variable);

                if (value != NULL) {

                    int value_len = strlen(value);

                    for (int x = 0;
                         x < value_len &&
                         j < PARSED_INPUT_SIZE - 1;
                         x++) {

                        expanded_input[j++] = value[x];
                    }
                }
            }
            else {
                expanded_input[j++] = '$';
            }
        }
        else {
            expanded_input[j++] = parsed_input[i];
        }
    }

    expanded_input[j] = '\0';

    strcpy(parsed_input, expanded_input);


    // Split input into arguments while respecting double quotes
    int in_quotes = 0;
    char *start = parsed_input;

    for (int i = 0; parsed_input[i] != '\0'; i++) {

        if (parsed_input[i] == '"') {

            in_quotes = !in_quotes;

            memmove(
                &parsed_input[i],
                &parsed_input[i + 1],
                strlen(&parsed_input[i])
            );

            i--;
        }

        else if (parsed_input[i] == ' ' && !in_quotes) {

            parsed_input[i] = '\0';

            if (start[0] != '\0' && argc < max_args - 1) {
                args[argc++] = start;
            }

            start = &parsed_input[i + 1];
        }
    }

    if (start[0] != '\0' && argc < max_args - 1) {
        args[argc++] = start;
    }

    args[argc] = NULL;

    return argc;
}
