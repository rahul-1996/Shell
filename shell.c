#include "shell.h"

int NO_OF_COMMANDS = -1;
int NO_OF_ALIASES = 0;


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

 // Manage Environment variables. 

    // Taking care of history stuff.
    for(int i=0;i<HISTORY_SIZE;i++)
	{
		History[i].command=(char*)malloc(sizeof(char)*MAXLINE);
    }

    // Aliases.
    for(int i=0;i<ALIAS_SIZE;i++) {
        AliasTable[i].key = (char*)malloc(sizeof(char)*MAXLINE);
        AliasTable[i].value = (char*)malloc(sizeof(char)*MAXLINE);
    }

    NO_OF_COMMANDS = -1;
    NO_OF_ALIASES = 0;
}


int main(int argc, char *argv[]) {
    char *prompt = "kdsh$ ";
    char cmdline[MAXLINE]; //command line for fgets.
    initializeShell();
    while(1) {
        printf(C_BLUE);
        printf("%s", prompt);
        printf(C_GREEN);
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

        // Update the command entered for history. 
        updateHistoryCommand(cmdline);

        //Checking.
        // printf("\n NO OF ALIASES ARE %d", NO_OF_ALIASES);
        // if(NO_OF_ALIASES>0) {printf("ASASAS");getAliases();}

        // Check if it is an alias beforte evaluating. 
        for(int i=0;i<NO_OF_ALIASES;i++) {
            if(strcmp(AliasTable[i].key, cmdline)==0) {
                // printf("hi there");
                strcpy(cmdline,AliasTable[i].value);
                }
        }

       // printf("Finally %s\n", cmdline);
        // Evaluate command line. 
        eval(cmdline);
    }

    // Signal Handlers
   //Signal(SIGINT,  sigint_handler);   // ctrl-c
}

