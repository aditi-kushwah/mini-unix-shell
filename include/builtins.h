#ifndef BUILTINS_H
#define BUILTINS_H

void builtin_pwd(void);
void builtin_help(void);
void builtin_history(char history[][100], int history_count);
void builtin_echo(char *args[]);
void builtin_cd(char *args[]);

#endif