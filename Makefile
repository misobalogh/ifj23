# Project Name: Implementace překladače imperativního jazyka IFJ23
# Authors: MICHAL BALOGH xbalog06, 

CFLAGS = -g -Wextra -Wall -pedantic -std=c11
CC = gcc
TESTLIB = cunit

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)
EXEC = $(basename $(SRC_FILES))
TEST_FILES = $(wildcard tests/*.c)
UNIT_TESTS = $(basename $(TEST_FILES))


synt_precedence_parser:
	$(CC) $(CFLAGS) synt_precedence_parser.c -o synt_precedence_parser
	./synt_precedence_parser

help:
	@echo ""
	@echo "Usage:"
	@echo "  make help      - Print this help message"
	@echo "  make           - Print this help message"
	@echo "  make all       - Compile all files and run tests"
	@echo "  make run       - Run the main program"
	@echo "  make main      - Compile the main program"
	@echo "  make tests     - Compile all tests"
	@echo "  make runtests  - Run all tests"
	@echo "  make <target>  - Compile a specific target program"
	@echo "  make clean     - Remove all object files and binaries"	
	@echo ""


all: main tests

runtests: $(UNIT_TESTS)
	@for test in $(UNIT_TESTS); do \
		./$$test; \
	done

run: main
	./main

main: $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(UNIT_TESTS)

tests/%: tests/%.o $(filter-out main.o, $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $@ -l$(TESTLIB)

clean:
	rm -f $(OBJ_FILES) $(EXEC) $(UNIT_TESTS) tests/*.o

.PHONY: all clean main run synt_precedence_parser
