
#include "shell.h"
//################ error function ##############################


void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}




//################ RUNNING BUILT IN COMMANDS EG: HISTORY ##############################


void runBuiltinCommand(struct command *cmd, int bg) {
    char *gdir;
    char *dir;
    char *to;
    char buf[MAXARGS];
    switch (cmd->builtin) {
        case QUIT:
            exit(0); break;
        case HISTORY:
            getHistory(); break;
        case ALIAS:
            createAlias(cmd); break;
        case CD:
            gdir = getcwd(buf, sizeof(buf));
            dir = strcat(gdir, "/");
            to = strcat(dir, cmd->argv[1]);
            chdir(to);
            break;  
        case SGOWN:
            if(cmd->argc<3) {
                printf("too few arguments\n");
                break;
            }
            sgown(cmd->argv[1], cmd->argv[2]); break;
        case NEWS:
            checkNews(cmd);
            break;
        default:
            error("Unknown builtin command");
    }
}


//################# NEWS ######################################

void checkNews(struct command *cmd) {
    pid_t childPid;
    childPid = fork();
    if ( childPid < 0) error("fork error");
    else if (childPid == 0) { 
        if (execlp("python3", "python3", "load.py", cmd->argv[1] , (char*) NULL) < 0) {
            printf("%s: Failed\n", cmd->argv[0]);
            exit(0);
        }
    }
    else { // parent. Shell continues here.
        updateHistoryPID(childPid);
        wait(&childPid);
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





// ########################  Sgown ########################################################

int getOccurenceCount(char * path,char * word) {
 	FILE *fptr;
    fptr = fopen(path, "r");
 	 /* Exit if file not opened successfully */
    if (fptr == NULL) {
         return -1;
    }
 	int BUFFER_SIZE=2000;
  	char str[BUFFER_SIZE];
    char *pos;
    int index, count, lineNo; 
    count = 0;
    lineNo = 1;
     // Read line from file till end of file.
     while ((fgets(str, BUFFER_SIZE, fptr)) != NULL) {
         index = 0;
         // Find next occurrence of word in str
         while ((pos = strstr(str + index, word)) != NULL) {
             index = (pos - str) + 1;
             count++;
             printf("Found %s at Line No %d\n", word, lineNo++);
         }
     }
     return count;
 }
 

 void sgown(char *name, char * searchstring) {
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(name))) {
        return;
 	}
    while ((entry = readdir(dir)) != NULL) {
 	    char path[1024];
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            sgown(path,searchstring);
        }
        
 		else {
 			char fullpath[1024];
 			strcpy(fullpath,name);
 			strcat(fullpath,"/");
            strcat(fullpath,entry->d_name);
            printf("Current in file %s\n", fullpath);
 			int count=getOccurenceCount(fullpath,searchstring);
 			printf("file: %s count: %d\n\n\n",fullpath,count);
 		  	}
   	}
     closedir(dir);
 }