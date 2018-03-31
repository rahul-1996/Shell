

CC=gcc
CFLAGS= -c 
DEPS = shell.h

all: main

main: shell.o helper_functions.o 
	  $(CC) -include $(DEPS) shell.o helper_functions.o

shell.o: 
		$(CC) $(CFLAGS) shell.c

helper_functions.o: helper_functions.c
		$(CC) $(CFLAGS) helper_functions.c

clean: 
	rm -rf *.o
