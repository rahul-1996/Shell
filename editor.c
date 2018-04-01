#include "shell.h"
#include <termios.h>

/*
Echo causes everything we type to be printed on the terminal. 
Note: We are using the ICANON flag to turn off canonical mode. 
We will read input byte-by-byte instead of line-by-line. 
*/

// Original terminal values are stored in the global variable orig_termios.
struct termios orig_termios;
// When we exit the program, our current terminal should retain its original canonical state.
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}


void enableRawMode() {

  tcgetattr(STDIN_FILENO, &orig_termios);
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
  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_oflag &= ~(OPOST);  // Turn off all output process using OPOST. like automatiacally formattting carriage return.
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // TCSAFLUSH specifies when to apply a change. 
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

}

int main() {

  enableRawMode();

  char c;
  // Read 1 byte at a time from standard input into variable c.
  // iscntrl will check if a character is a control character. Eg: backspace.
  while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
    if (iscntrl(c)) {
      printf("%d\n", c);
    } else {
      printf("%d ('%c')\n", c, c);
    }
  }
  return 0;
}