#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>

#ifndef __SHELL_H__
#define __SHELL_H__

#define MAXLINE 1024

// Defining maximum number of arguments that can be passed. 
#define MAXARGS 64

#define HISTORY_SIZE 25
#define ALIAS_SIZE 100
struct command {
    int argc;
    char *argv[MAXARGS];
    enum builtin_t {
        SYSTEM, QUIT, HISTORY, ALIAS } builtin;
    bool hasPipe;
};

extern int NO_OF_COMMANDS;
extern int NO_OF_ALIASES;
struct history
{
	char* command;
	pid_t pid;
};

struct history History[HISTORY_SIZE];


// Alias structure. 

typedef struct Alias {
    char *key;
    char *value;
}Alias;

Alias AliasTable[ALIAS_SIZE]; 


#define C_YELLOW  "\x1B[33m" 
#define C_BLUE    "\e[1;34m"
#define C_GREEN   "\e[1;32m"
#define C_RESET   "\033[0m"

void eval(char *cmdline);
int parse(const char *cmdline, struct command *cmd);
void error(char *msg);
void runSystemCommand(struct command *cmd, int bg);
void runBuiltinCommand(struct command *cmd, int bg); 
void sigint_handler(int sig);
void sigstp_handler(int sig);
void pipe_execute(char *argv[]) ;
void IOhandler(char *args[], char* input, char* output) ;
void getHistory();
void updateHistoryPID(int pid);
void updateHistoryCommand(char *cmdline);
void createAlias(struct command *cmd);
void checkAlias(char (*cmdline)[1024]);
void getAliases();

#endif