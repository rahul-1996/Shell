#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
char *prompt = "kdsh$ ";
// Defining maximum number of arguments that can be passed. 
#define MAXARGS 64

struct command {
    int argc;
    char *argv[MAXARGS];
    enum builtin_t {
        NONE, QUIT, HISTORY } builtin;
};
    
enum builtin_t parseBuiltin(struct command *cmd) {
    if (!strcmp(cmd->argv[0], "quit")) { // quit command
        return QUIT;
    } else if (!strcmp(cmd->argv[0], "history")) { // jobs command
        return HISTORY;     
    } else {
        return NONE;
    }
}

void eval(char *cmdline);
int parse(const char *cmdline, struct command *cmd);
void error(char *msg);