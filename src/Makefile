# Variables used by implicit rules
# See https://www.gnu.org/software/make/manual/make.html#Implicit-Variables
CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

# Object files grouped by directory
EMULATOR_OBJ := $(patsubst %.c, %.o, $(shell find emulator/ -type f -name '*.c'))
ASSEMBLER_OBJ := $(patsubst %.c, %.o, $(shell find assembler/ -type f -name '*.c'))
TESTS := $(patsubst %.spec.c, %.spec, $(shell find ../test/ -type f -name '*.spec.c'))

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate test

# Compile assembler executable
assemble: assemble.o $(ASSEMBLER_OBJ)

# Compile emulator executable
emulate: emulate.o $(EMULATOR_OBJ)

# Runs all tests
test: $(TESTS)
	for unit in $(shell find ../test/ -type f -name '*.spec'); do  $$unit; done

# Pattern rule for test compilation to handle the fancy suffix
# See https://www.gnu.org/software/make/manual/make.html#Pattern-Intro
# and https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
%.spec %.spec.o: %.spec.c $(EMULATOR_OBJ) $(ASSEMBLER_OBJ)
	$(CC) $(CCFLAGS) -o $@ $^

# Remove all object files and executables
clean:
	rm -f *.o
	rm -f $(TESTS)
	rm -f assemble
	rm -f emulate
