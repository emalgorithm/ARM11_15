CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o

<<<<<<< HEAD
=======
emulate: emulate.o reader.o pipeline.o printer.o branch.o sdt.o multiply.o
>>>>>>> FETCH_HEAD

clean:
	rm -f $(wildcard *.o)
	rm -f assemble
	rm -f emulate
