#include <string.h>
#include "../include/parser.h"

int parse_input(char *input, char *args[], int max_args) {
    int argc = 0;

    char *operators = "|<>&";
    char parsed_input[max_args * 10];

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

    // Split into arguments while respecting double quotes
    int in_quotes = 0;
    char *start = parsed_input;

    for (int i = 0; parsed_input[i] != '\0'; i++) {

        if (parsed_input[i] == '"') {
            in_quotes = !in_quotes;

            // Remove the quote
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
