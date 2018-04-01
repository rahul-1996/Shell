#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/wait.h>

#ifndef __SHELL_H__
#define __SHELL_H__

#define MAXLINE 1024

// Defining maximum number of arguments that can be passed. 
#define MAXARGS 64

struct command {
    int argc;
    char *argv[MAXARGS];
    enum builtin_t {
        SYSTEM, QUIT, HISTORY } builtin;
    bool hasPipe;
};
    

void eval(char *cmdline);
int parse(const char *cmdline, struct command *cmd);
void error(char *msg);
void runSystemCommand(struct command *cmd, int bg);
void runBuiltinCommand(struct command *cmd, int bg); 
void sigint_handler(int sig);
void sigstp_handler(int sig);
void pipe_execute(char *argv[]) ;
#endif