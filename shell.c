#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXLINE 1024
char *prompt = "kdsh$ ";
// Defining maximum number of arguments that can be passed. 
#define MAXARGS 64

typedef struct command {
    int argc;
    char *argv[MAXARGS];
    enum builtin_t {
        NONE, QUIT, JOBS, BG, FG } builtin;
};
    

void eval(char *cmdline) {
  int bg;
  struct command cmd;

  printf("Evaluating '%s'\n", cmdline);
  // parse line into command struct
  bg = parse(cmdline, &cmd); 

  // parse error
  if (bg == -1) return;
}

int main(int argc, char *argv[]) {
    char cmdline[MAXLINE]; //command line for fgets.
    while(1) {
        printf("%s", prompt);
        // Check for error.
        if(fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
            printf("Error in taking input.");
        }
        // tests the end-of-file indicator for the stream
        // pointed  to by stream
        if(feof(stdin)) {
            printf("\n"); exit(0);
        }
        
        //Remove trailing newline. 
        cmdline[strlen(cmdline)-1] = '\0';

        // Evaluate command line. 
        eval(cmdline);
    }
}

    }
}