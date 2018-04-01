#include "shell.h"




void createAlias(struct command *cmd) {
    //while(cmd->argv[i]!=NULL) {
        // printf("\n%s", cmd->argv[0]);
        // printf("\n%s", cmd->argv[1]);
        // printf("Command is %d\n", cmd->argc);
    // for(int i=0;i<cmd->argc;i++) {
    //     printf("\n%s", cmd->argv[i]);   
    // }
    char str[1024] = "";
    if(cmd->argc < 3) error("Need more arguments for alias");
    NO_OF_ALIASES += 1;
    strcpy(AliasTable[NO_OF_ALIASES-1].key,cmd->argv[1]);
    for(int i=2;i<cmd->argc;i++){
        strcat(str,cmd->argv[i]);
        strcat(str, " ");
    }
    strcpy(AliasTable[NO_OF_ALIASES-1].value,str);
    // printf("   %s    \n", AliasTable[NO_OF_ALIASES-1].key);
    // printf("   %s    \n", AliasTable[NO_OF_ALIASES-1].value);
    return;
}

void checkAlias(char (*cmdline)[1024]) {
    // printf("Checking\n");
    // printf("cmdline is %s\n", cmdline);
    // printf("alias is %s\n", AliasTable[0].key);
    // for(int i=0;i<NO_OF_ALIASES;i++) {
    //     if(strcmp(AliasTable[i].key, cmdline)==0) {
    //         printf("hi there");
    //         strcpy(cmdline,AliasTable[i].value);
    //         return;
    //     }
    // }
    return;
}

void getAliases() {
    // printf("REACHED HERE\n");
    // printf("MAGIC IS %s\n", AliasTable[0].key);
    for(int i=0;i<NO_OF_ALIASES;i++) {
        printf("%s\t%s\n", AliasTable[i].key, AliasTable[i].value);
    }
}



