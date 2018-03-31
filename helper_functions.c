#include "shell.h"

//################ error function ##############################


void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}


//################ PARSING ##############################


enum builtin_t parseBuiltin(struct command *cmd) {
    if (!strcmp(cmd->argv[0], "quit")) { // quit command
        return QUIT;
    } else if (!strcmp(cmd->argv[0], "history")) { // jobs command
        return HISTORY;     
    } else {
        return SYSTEM;
    }
}



int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];
    const char delims[10] = " \t\r\n";   // argument delimiters
    char *line = array;                   // ptr that traverses command line
    char *token;                          // ptr to the end of the current arg
    char *endline;                        // ptr to the end of the cmdline string
    int is_bg; // Run in the background?

    // Nothing is entered? AMAZING
    if (cmdline == NULL) 
        error("command line is NULL\n");

    if (cmdline == NULL) 
        error("command line is NULL\n");

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);

    // build argv list
    cmd->argc = 0;

    while (line < endline) {
        // skip delimiters
        line += strspn (line, delims);
        if (line >= endline) break;

        // Find token delimiter
        token = line + strcspn (line, delims);

        // terminate the token
        *token = '\0';

        // Record token as the token argument
        cmd->argv[cmd->argc++] = line;

        // Check if argv is full
        if (cmd->argc >= MAXARGS-1) break;

        line = token + 1;
    }

    // argument list must end with a NULL pointer
    cmd->argv[cmd->argc] = NULL;

    if (cmd->argc == 0)  // ignore blank line
        return 1;
    
    cmd->builtin = parseBuiltin(cmd);

     // should job run in background?
    if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

    return is_bg;
}


//################ RUNNING BUILT IN COMMANDS EG: HISTORY ##############################


void runBuiltinCommand(struct command *cmd, int bg) {
    switch (cmd->builtin) {
        case QUIT:
            printf("TODO: quit\n"); break;
        case HISTORY:
            printf("TODO: History\n"); break;
        default:
            error("Unknown builtin command");
    }
}

//################ RUNNING SYSTEM COMMANDS EG: ls ##############################

void runSystemCommand(struct command *cmd, int bg) {
    pid_t childPid;
    if ((childPid = fork()) < 0) error("fork error");
    else if (childPid == 0) { 
        if (execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: Command not found\n", cmd->argv[0]);
            exit(0);
        }
    }
    else { // parent. Shell continues here.
       if (bg) 
          printf("Child in background [%d]\n",childPid);
       else
          wait(&childPid);
    }
}
