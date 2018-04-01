#include "shell.h"

// ################### HISTORY #################################

void getHistory() {
    printf("Command\t\t\tPID\n");
    for(int i=0;i<NO_OF_COMMANDS;i++) {
        printf("%s\t\t\t%d\n", History[i].command, History[i].pid);
    }
}

// ############### UPDATING HISTORY ###############################

void updateHistoryPID(int pid) {
    History[NO_OF_COMMANDS%HISTORY_SIZE].pid = pid;
}


void updateHistoryCommand(char *cmdline) {
    strcpy(History[NO_OF_COMMANDS%HISTORY_SIZE].command,cmdline);
}