
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

