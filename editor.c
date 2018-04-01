#include "shell.h"
#include <termios.h>
#include <errno.h>


/*** defines ***/
#define CTRL_KEY(k) ((k) & 0x1f) // Mapping ctrl-Q to to quit. 


/*** data ***/

struct termios orig_termios;


/*** terminal ***/
void die(const char *s) {
  // Clear the screen on exit. 
  write(STDOUT_FILENO, "\x1b[2J", 4);
  write(STDOUT_FILENO, "\x1b[H", 3);
  perror(s);
  exit(1);
}


// When we exit the program, our current terminal should retain its original canonical state.

void disableRawMode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    die("tcsetattr");
}



void enableRawMode() {

  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
  // atexit() registers disableRawMode() function to be called automatically when program exits.
  atexit(disableRawMode);
  
  // Make a copy of orig_termios before starting to make changes.
  struct termios raw = orig_termios;
  
  // Terminal attributes are read into a terminos by tcsgetattr().
  tcgetattr(STDIN_FILENO, &raw);
  // ECHO is a bit flag. 
  // ICANON turns off canonical mode. 
  // ISIG turns off ctrl + c.
  // IXON turns off Ctrl-S and Q.
  //IEXTEN disables ctrl-v.
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);  // Turn off all output process using OPOST. like automatiacally formattting carriage return.
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

 // Adding a timeout for read.
 /*
 VMIN and VTIME come from <termios.h>. 
 VTIME value sets the maximum amount of time to wait before read() returns. 
 It is in tenths of a second, so we set it to 1/10 of a second, or 100 milliseconds. 
 If read() times out, it will return 0
 */
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  // TCSAFLUSH specifies when to apply a change. 
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");

}

/*** input ***/

// Wait for one keypress and return it. 
char editorReadKey() {
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
    if (nread == -1 && errno != EAGAIN) die("read");
  }
  return c;
}

// Waits for a keypress and handles it. 
void editorProcessKeypress() {
  char c = editorReadKey();
  switch (c) {
    case CTRL_KEY('q'):
      write(STDOUT_FILENO, "\x1b[2J", 4);
      write(STDOUT_FILENO, "\x1b[H", 3);
      exit(0);
      break;
  }
}

/*
Notes from the tutorial: 
We are using the J command (Erase In Display) to clear the screen. 
Escape sequence commands take arguments, which come before the command. 
In this case the argument is 2, which says to clear the entire screen. 
<esc>[1J would clear the screen up to where the cursor is, and <esc>[0J would clear 
the screen from the cursor up to the end of the screen. 
Also, 0 is the default argument for J, so just <esc>[J by itself would also clear 
the screen from the cursor to the end.


*/


/*** output ***/

void editorDrawRows() {
  int y;
  // ~ at beginning of each line. Doing it for arbitrary no of lines. 
  for (y = 0; y < 24; y++) {
    write(STDOUT_FILENO, "~\r\n", 3);
  }
}


void editorRefreshScreen() {
    // Escape sequences always start with an escape character (27) followed by a [ character. 
    // The first byte is \x1b, which is the escape character, or 27 in decimal
    // We are writing 4 bytes onto the terminal. 
    write(STDOUT_FILENO, "\x1b[2J", 4);
    // H command helps in repositioning to the top left corner. 
    write(STDOUT_FILENO, "\x1b[H", 3);

   // To draw a tilde at the beginning. 
   editorDrawRows();
   write(STDOUT_FILENO, "\x1b[H", 3);
}

int main() {

  enableRawMode();

  // Read 1 byte at a time from standard input into variable c.
  // iscntrl will check if a character is a control character. Eg: backspace.
   
  while (1) {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}