#include "shell.h"

int NO_OF_COMMANDS = 0;



void eval(char *cmdline) {
  int bg;
  struct command cmd;
  cmd.hasPipe = false;
  printf("Evaluating '%s'\n", cmdline);
  // parse line into command struct
  bg = parse(cmdline, &cmd); 
  //printf("\n hasPipe? %d \n", cmd.hasPipe);
  // ERROR
  if (bg == -1) return;
  if (cmd.argv[0] == NULL) return;
  if (cmd.builtin == SYSTEM) 
    runSystemCommand(&cmd, bg);
  else 
    runBuiltinCommand(&cmd, bg);
}

void initializeShell() {

	for(int i=0;i<HISTORY_SIZE;i++)
	{
		History[i].command=(char*)malloc(sizeof(char)*MAXLINE);
    }
    NO_OF_COMMANDS = -1;
}

int main(int argc, char *argv[]) {
    char *prompt = "kdsh$ ";
    char cmdline[MAXLINE]; //command line for fgets.
    initializeShell();
    while(1) {
        printf("%s", prompt);
        // Error?
        if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
            printf("Error in taking input.");
        // pointed  to by stream
        }
        // tests the end-of-file indicator for the stream
        if(feof(stdin)) {
            printf("\n"); exit(0);
        }
        
        //Remove trailing newline. 
        cmdline[strlen(cmdline)-1] = '\0';

        // Updating Number of Commands. 
        NO_OF_COMMANDS += 1;
        updateHistoryCommand(cmdline);
        // Evaluate command line. 
        eval(cmdline);
    }

    // Signal Handlers
   //Signal(SIGINT,  sigint_handler);   // ctrl-c
}

