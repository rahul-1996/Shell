#include "shell.h"

void pipe_execute(char *args[]) {
    // printf("In pipe_execute\n");
    // File descriptors
	int fd[2]; // pos. 0 output, pos. 1 input of the pipe
	int kd[2];
	int num_cmds = 0;
    char *command[256];
    
	pid_t pid;
	
	int err = -1;
	int end = 0;
	
	int i = 0;
	int j = 0;
	int k = 0;
	int l = 0;
	
	// First we calculate the number of commands (they are separated
	// by '|')
	while (args[l] != NULL){
		if (strcmp(args[l],"|") == 0){
			num_cmds++;
		}
		l++;
	}   
	num_cmds++;
	while (args[j] != NULL && end != 1) {
		k = 0;
		while (strcmp(args[j],"|") != 0) {
			command[k] = args[j];
			j++;	
			if (args[j] == NULL){
				// 'end' variable used to keep the program from entering
				// again in the loop when no more arguments are found
				end = 1;
				k++;
				break;
			}
			k++;
		}
		// Last position of the command will be NULL to indicate that
		// it is its end when we pass it to the exec function
		command[k] = NULL;
		j++;		    
		if (i % 2 != 0){
			pipe(fd); // for odd i
		}else{
			pipe(kd); // for even i
		}
		
		pid=fork();
		
		if(pid==-1) {					
			printf("Child process could not be created\n");
			return;
        }
        
		if(pid==0) {
			// If we are in the first command
			if (i == 0) {
				dup2(kd[1], STDOUT_FILENO);
			}
			// If we are in the last command, depending on whether it
			// is placed in an odd or even position, we will replace
			// the standard input for one pipe or another. 
			else if (i == num_cmds - 1){
				if (num_cmds % 2 != 0){ // for odd number of commands
					dup2(fd[0],STDIN_FILENO);
				}else{ // for even number of commands
					dup2(kd[0],STDIN_FILENO);
				}
			// If we are in a command that is in the middle, we will
			// have to use two pipes, one for input and another for
			// output. 
            }
            else { // for odd i
				if (i % 2 != 0){
					dup2(kd[0],STDIN_FILENO); 
					dup2(fd[1],STDOUT_FILENO);
				}else{ // for even i
					dup2(fd[0],STDIN_FILENO); 
					dup2(kd[1],STDOUT_FILENO);					
				} 
			}
			
			if (execvp(command[0],command)==err){
				kill(getpid(),SIGTERM);
			}		
        }
        // Updating history PID
        updateHistoryPID(pid);			
        // CLOSING DESCRIPTORS ON PARENT
        
		if (i == 0){
			close(kd[1]);
		}
		else if (i == num_cmds - 1){
			if (num_cmds % 2 != 0){					
				close(fd[0]);
			}else{					
				close(kd[0]);
			}
        }
        else{
			if (i % 2 != 0){					
				close(kd[0]);
				close(fd[1]);
			}else{					
				close(fd[0]);
				close(kd[1]);
			}
		}
		waitpid(pid,NULL,0);
		i++;	
	}
}