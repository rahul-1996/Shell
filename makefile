

CC=gcc
CFLAGS= -c 

all: main

main: shell.o helper_functions.o parse.o pipe.o
	  $(CC) shell.o helper_functions.o parse.o pipe.o

shell.o: shell.c
		$(CC) $(CFLAGS) shell.c

parse.o: parse.c
		$(CC) $(CFLAGS) parse.c

helper_functions.o: helper_functions.c
		$(CC) $(CFLAGS) helper_functions.c

pipe.o: pipe.c
		$(CC) $(CFLAGS) pipe.c

editor: editor.c
	$(CC) editor.c -o editor -Wall -Wextra -pedantic -std=c99

clean: 
	rm -rf *.o
