#include "shell.h"

void eval(char *cmdline) {
  int bg;
  struct command cmd;
  printf("Evaluating '%s'\n", cmdline);
  // parse line into command struct
  bg = parse(cmdline, &cmd); 
  // ERROR
  if (bg == -1) return;
  if (cmd.argv[0] == NULL) return;
  if (cmd.builtin == SYSTEM) 
    runSystemCommand(&cmd, bg);
  else 
    runBuiltinCommand(&cmd, bg);
}

int main(int argc, char *argv[]) {
    char *prompt = "kdsh$ ";
    char cmdline[MAXLINE]; //command line for fgets.
    while(1) {
        printf("%s", prompt);
        // Error?
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
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

