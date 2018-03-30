

void error(char *msg) {
  printf("Error: %s", msg);
  exit(0);
}

int parse(const char *cmdline, struct command *cmd) {
    static char array[MAXLINE];
    const char delims[10] = " \t\r\n";   // argument delimiters
    char *line = array;                   // ptr that traverses command line
    char *token;                          // ptr to the end of the current arg
    char *endline;                        // ptr to the end of the cmdline string
    int is_bg; // Run in the background?

    // Nothing is entered? AMAZING
    if (cmdline == NULL) 
        error("command line is NULL\n");

    if (cmdline == NULL) 
        error("command line is NULL\n");

    (void) strncpy(line, cmdline, MAXLINE);
    endline = line + strlen(line);

    // build argv list
    cmd->argc = 0;

    while (line < endline) {
        // skip delimiters
        line += strspn (line, delims);
        if (line >= endline) break;

        // Find token delimiter
        token = line + strcspn (line, delims);

        // terminate the token
        *token = '\0';

        // Record token as the token argument
        cmd->argv[cmd->argc++] = line;

        // Check if argv is full
        if (cmd->argc >= MAXARGS-1) break;

        line = token + 1;
    }

    // argument list must end with a NULL pointer
    cmd->argv[cmd->argc] = NULL;

    if (cmd->argc == 0)  // ignore blank line
        return 1;
    
    
    cmd->builtin = parseBuiltin(cmd);

     // should job run in background?
    if ((is_bg = (*cmd->argv[cmd->argc-1] == '&')) != 0)
        cmd->argv[--cmd->argc] = NULL;

    return is_bg;
}