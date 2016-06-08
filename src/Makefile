# Variables used by implicit rules
# See https://www.gnu.org/software/make/manual/make.html#Implicit-Variables
CC      = gcc
CFLAGS  = -Wall -g -D_POSIX_SOURCE -D_BSD_SOURCE -std=c99 -Werror -pedantic

# Object files grouped by directory
EMULATOR_OBJ := $(patsubst %.c, %.o, $(shell find emulator/ -type f -name '*.c'))
ASSEMBLER_OBJ := $(patsubst %.c, %.o, $(shell find assembler/ -type f -name '*.c'))
DISASSEMBLER_OBJ := $(patsubst %.c, %.o, $(shell find disassembler/ -type f -name '*.c'))
UTIL_OBJ := $(patsubst %.c, %.o, $(shell find util/ -type f -name '*.c'))
TESTS := $(patsubst %.spec.c, %.spec, $(shell find ../test/ -type f -name '*.spec.c'))

.SUFFIXES: .c .o

.PHONY: all clean

all: disassemble assemble emulate test

# Compile disassembler executable
disassemble: disassemble.o $(DISASSEMBLER_OBJ) $(EMULATOR_OBJ) $(UTIL_OBJ)

# Compile assembler executable
assemble: assemble.o $(ASSEMBLER_OBJ) $(UTIL_OBJ)

# Compile emulator executable
emulate: emulate.o $(EMULATOR_OBJ) $(UTIL_OBJ)

# Runs all tests
# where a test is a *.spec executable in the tree of test/
test: $(TESTS)
	for unit in $(shell find ../test/ -type f -name '*.spec'); do  $$unit; done

# Pattern rule for test compilation to handle the fancy suffix
# See https://www.gnu.org/software/make/manual/make.html#Pattern-Intro
# and https://www.gnu.org/software/make/manual/make.html#Automatic-Variables
%.spec %.spec.o: %.spec.c $(EMULATOR_OBJ) $(ASSEMBLER_OBJ) $(DISASSEMBLER_OBJ) $(UTIL_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Remove all object files and executables
clean:
	rm -f $(shell find ../ -type f -name '*.o')    # remove all object files
	rm -f $(TESTS)                                 # remove test executables
	rm -f assemble                                 # remove assemble executable
	rm -f emulate                                  # remove emulate executable
	rm -f disassemble                              # remove disassemble executable
