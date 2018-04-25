#include "shell.h"

//################ PARSING ##############################


enum builtin_t parseBuiltin(struct command *cmd) {
    if (!strcmp(cmd->argv[0], "quit")) { // quit command
        return QUIT;
    } else if (!strcmp(cmd->argv[0], "history")) { // history command
        //updateHistory(getpid());
        return HISTORY;
    } else if (!strcmp(cmd->argv[0], "alias")) { // Alias command
        return ALIAS;
    }
    else if (!strcmp(cmd->argv[0], "cd")) { // Alias command
        return CD;
    }
    else if (!strcmp(cmd->argv[0], "sgown")) { // Alias command
        return SGOWN;
    }
    else if (!strcmp(cmd->argv[0], "news")) { // Alias command
        return NEWS;
    }
     else {
        //updateHistory(getpid());
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

    char *arguments[64];
    int argument_count = 0;
   // printf("\n argument count is %d", argument_count);
    // Nothing is entered? AMAZING
    if (cmdline == NULL) 
        error("command line is NULL\n");

    if (cmdline == NULL) 
        error("command line is NULL\n");

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);
//    printf("endline is %s", endline);

    // build argv list
    cmd->argc = 0;

    while (line < endline) {
        // skip delimiters
        line += strspn (line, delims);
        if (line >= endline) break;
     //   printf("line is %s\n", line);
        // Find token delimiter
        token = line + strcspn (line, delims);
        // terminate the token
        *token = '\0';
        //printf("line is %s \n", line);
        // Record token as the token argument
        cmd->argv[cmd->argc++] = line;



        // Check if pipe and add it. 
        if(strcmp(line,"|")==0) {
            cmd->hasPipe = true;
        }

        // Check if argv is full
        if (cmd->argc >= MAXARGS-1) break;

		if ( (strcmp(line,">") != 0) && (strcmp(line,"<") != 0) && (strcmp(line,"&") != 0)) {
            arguments[argument_count] = line;
            argument_count += 1;
        }

        line = token + 1;

      //  printf("Token is %s\n", token-1);

    }

    // for(int i=0;i<argument_count;i++) {
    //     printf("\nargument is %s", arguments[i]);
    // }
   // Testing
   // printf("argument is %s \n", cmd->argv[1]);
   // printf("Total no of commands is %d \n", cmd->argc);
    
   // argument list must end with a NULL pointer
    cmd->argv[cmd->argc] = NULL;
    

    if (cmd->argc == 0)  // ignore blank line
        return 1;
    
    cmd->builtin = parseBuiltin(cmd);

     // should job run in background?
    if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

//      printf("\n hasPipe? %d \n", cmd->hasPipe);


    // Handling redirection.

    // > indicates output redirection. 

    for(int i=0; i < cmd->argc; i++) {
        if(strcmp(cmd->argv[i], ">") == 0) {
            if(cmd->argv[i+1]==NULL) error("Need more arguments");
            IOhandler(arguments, NULL, cmd->argv[i+1]);
        }
    }

    return is_bg;
}



void IOhandler(char *args[], char* input, char* output) {
    __pid_t pid;
    int fd;
    pid = fork();
    if(pid==-1) error("Could not fork");
    if(pid==0) {
        fd = open(output, O_CREAT | O_TRUNC | O_WRONLY, 0600); 
        dup2(fd, STDOUT_FILENO);
        close(fd);
        if (execvp(args[0],args)== -1){
			error("error in execvp");
			kill(getpid(),SIGTERM);
		}
    }
    waitpid(pid,NULL,0);
}

