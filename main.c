#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include "include/builtins.h"
#include "include/parser.h"

#define MAX_INPUT 100
#define MAX_ARGS 20
#define MAX_HISTORY 50

void handle_sigchld(int sig) {
    (void)sig;

    while (waitpid(-1, NULL, WNOHANG) > 0) {
        // Reap completed child processes
    }
}

int main() {
    signal(SIGINT, SIG_IGN);
    signal(SIGCHLD, handle_sigchld);

    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    char history[MAX_HISTORY][MAX_INPUT];
    int history_count = 0;

    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0) {
            break;
        }

        // Save command history
        if (strlen(input) == 0) {
            continue;
        }

        if (history_count < MAX_HISTORY) {
            strcpy(history[history_count], input);
            history_count++;
        }


        // Parse input

        int argc = parse_input(input, args, MAX_ARGS);

        // Check for background process &
        int background = 0;

        if (argc > 0 && strcmp(args[argc - 1], "&") == 0) {
            background = 1;
            args[argc - 1] = NULL;
        }

        // Check for pipe
        int pipe_position = -1;

        for (int i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "|") == 0) {
                pipe_position = i;
                break;
            }
        }

        // Check for invalid pipe usage
        if (pipe_position == 0 || args[pipe_position + 1] == NULL) {
            printf("Usage: command1 | command2\n");
            continue;
        }

        // Check for output redirection
        char *input_file = NULL;
        char *output_file = NULL;
        char *append_file = NULL;
        int redirection_error = 0;

        for (int i = 0; args[i] != NULL; i++) {

            // Input redirection <
            if (strcmp(args[i], "<") == 0) {

                if (args[i + 1] == NULL) {
                printf("Usage: command < filename\n");
                redirection_error = 1;
                break;
            }

            input_file = args[i + 1];
            args[i] = NULL;
        }

        // Append output redirection >>
        else if (strcmp(args[i], ">>") == 0) {

            if (args[i + 1] == NULL) {
                printf("Usage: command >> filename\n");
                redirection_error = 1;
                break;
            }

            append_file = args[i + 1];
            args[i] = NULL;
        }

        // Output redirection >
        else if (strcmp(args[i], ">") == 0) {

            if (args[i + 1] == NULL) {
                printf("Usage: command > filename\n");
                redirection_error = 1;
                break;
            }

            output_file = args[i + 1];
            args[i] = NULL;
        }
    }

        if (redirection_error) {
            continue;
        }
        // Multiple pipe handling
        if (pipe_position != -1) {

            char **commands[MAX_ARGS];
            int command_count = 0;

            // First command
            commands[0] = args;

            // Split commands at |
            for (int i = 0; args[i] != NULL; i++) {

                if (strcmp(args[i], "|") == 0) {

                    args[i] = NULL;

                    command_count++;
                    commands[command_count] = &args[i + 1];
                }
            }

            command_count++;

            int previous_read = -1;

            for (int i = 0; i < command_count; i++) {

                int current_pipe[2];

                // Create pipe if this is not the last command
                if (i < command_count - 1) {

                    if (pipe(current_pipe) == -1) {
                        perror("pipe");
                        break;
                    }
                }

                pid_t pid = fork();

                if (pid == 0) {

                    // Read from previous command
                    if (previous_read != -1) {
                        dup2(previous_read, STDIN_FILENO);
                        close(previous_read);
                    }

                    // Send output to next command
                    if (i < command_count - 1) {

                        close(current_pipe[0]);

                        dup2(current_pipe[1], STDOUT_FILENO);

                        close(current_pipe[1]);
                    }

                    execvp(commands[i][0], commands[i]);

                    perror("Command failed");
                    exit(1);
                }

                if (pid < 0) {
                    perror("fork failed");
                    break;
                }

                // Parent closes previous read end
                if (previous_read != -1) {
                    close(previous_read);
                }

                // Parent keeps only the read end
                // of the current pipe
                if (i < command_count - 1) {

                    close(current_pipe[1]);

                    previous_read = current_pipe[0];
                }
            }

            // Close final read end in parent
            if (previous_read != -1) {
                close(previous_read);
            }

            // Wait for all commands
            for (int i = 0; i < command_count; i++) {
                wait(NULL);
            }

            continue;
        }

            // Handle built-in pwd command
            if (strcmp(args[0], "pwd") == 0) {
                builtin_pwd();
                continue;
            }

            // Handle built-in history command
            if (strcmp(args[0], "history") == 0) {
                builtin_history(history, history_count);
                continue;
            }

            // Handle built-in cd command
            if (strcmp(args[0], "cd") == 0) {
                builtin_cd(args);
                continue;
            }

            // Handle built-in help command
            if (strcmp(args[0], "help") == 0) {
                builtin_help();
                continue;
            }

        // Handle built-in echo command
        if (strcmp(args[0], "echo") == 0) {

            if (append_file != NULL) {
                int fd = open(append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

                if (fd < 0) {
                    perror("open");
                    continue;
                }

                for (int i = 1; args[i] != NULL; i++) {
                    dprintf(fd, "%s", args[i]);

                    if (args[i + 1] != NULL) {
                        dprintf(fd, " ");
                    }
                }

                dprintf(fd, "\n");
                close(fd);
        }

        else if (output_file != NULL) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0) {
                perror("open");
                continue;
            }

            for (int i = 1; args[i] != NULL; i++) {
                dprintf(fd, "%s", args[i]);

                if (args[i + 1] != NULL) {
                    dprintf(fd, " ");
                }
            }

            dprintf(fd, "\n");
            close(fd);
        }

        else {
            builtin_echo(args);
        }

        continue;
    }


        pid_t pid = fork();

        if (pid == 0) {
            // Child process

            signal(SIGINT, SIG_DFL);

            // Handle input redirection <
            if (input_file != NULL) {
                int fd = open(input_file, O_RDONLY);

                if (fd < 0) {
                    perror("open");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }

            // Handle append redirection >>
            if (append_file != NULL) {
                int fd = open(append_file, O_WRONLY | O_CREAT | O_APPEND, 0644);

                if (fd < 0) {
                    perror("open");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            // Handle output redirection >
            if (output_file != NULL) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                if(fd < 0) {
                    perror("open");
                    exit(1);
                }

                dup2(fd, STDOUT_FILENO);
                close(fd);
            }

            execvp(args[0], args);

            perror("Command not found");
            exit(1);

        }
        else if (pid > 0) {
            // Parent process

            if (background) {
                printf("[Background process started: %d]\n", pid);
            }
            else {
                waitpid(pid, NULL, 0);
            }
        }
        else {
            perror("fork failed");
        }

    }

    return 0;
}
