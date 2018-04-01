
#include "shell.h"
//################ error function ##############################


void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}




//################ RUNNING BUILT IN COMMANDS EG: HISTORY ##############################


void runBuiltinCommand(struct command *cmd, int bg) {
    switch (cmd->builtin) {
        case QUIT:
            exit(0); break;
        case HISTORY:
            getHistory(); break;
        case ALIAS:
            createAlias(cmd); break;
        default:
            error("Unknown builtin command");
    }
}

//################ RUNNING SYSTEM COMMANDS EG: ls ##############################

void runSystemCommand(struct command *cmd, int bg) { 
    // Should pipeHandler be called?
    if(cmd->hasPipe) {
        pipe_execute(cmd->argv);  
        return;
    }
        // Not there
    else {
      pid_t childPid;
      childPid = fork();
      if ( childPid < 0) error("fork error");
      else if (childPid == 0) { 
          if (execvp(cmd->argv[0], cmd->argv) < 0) {
              printf("%s: Command not found\n", cmd->argv[0]);
              exit(0);
            }
      }
      else { // parent. Shell continues here.
        updateHistoryPID(childPid);
         if (bg) 
            printf("Child in background [%d]\n",childPid);
         else {
            wait(&childPid);
         }
        }
    }
}


// ####################    SIGNAL HANDLERS ############################

void sigint_handler(int sig) {
 
    return;
}