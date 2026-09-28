#include <stdio.h>
#include <unistd.h>
#include "../include/builtins.h"

void builtin_pwd(void) {
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

void builtin_help(void) {
    printf("Mini Unix Shell Commands:\n");
    printf("  cd <directory>  - Change directory\n");
    printf("  pwd             - Show current directory\n");
    printf("  echo <text>     - Display text\n");
    printf("  help            - Show this help message\n");
    printf("  history         - Show command history\n");
    printf("  exit            - Exit the shell\n");
}

void builtin_history(char history[][100], int history_count) {
    for (int i = 0; i < history_count; i++) {
        printf("%d %s\n", i + 1, history[i]);
    }
}

void builtin_echo(char *args[]) {
    for (int i = 1; args[i] != NULL; i++) {
        printf("%s", args[i]);

        if (args[i + 1] != NULL) {
            printf(" ");
        }
    }

    printf("\n");
}

void builtin_cd(char *args[]) {
    if (args[1] == NULL) {
        fprintf(stderr, "cd: missing directory\n");
        return;
    }

    if (chdir(args[1]) != 0) {
        perror("cd");
    }
}