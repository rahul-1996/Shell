#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 1024
char *prompt = "kdsh$ ";
// Defining maximum number of arguments that can be passed. 
#define MAXARGS 64

struct command {
    int argc;
    char *argv[MAXARGS];
    enum builtin_t {
        SYSTEM, QUIT, HISTORY } builtin;
};
    

void eval(char *cmdline);
int parse(const char *cmdline, struct command *cmd);
void error(char *msg);
void runSystemCommand(struct command *cmd, int bg);
void runBuiltinCommand(struct command *cmd, int bg); 