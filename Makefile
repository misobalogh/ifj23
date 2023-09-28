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


help:
	@echo ""
	@echo "Usage:"
	@echo "make help     - print this help message"
	@echo "make	      - print this help message"
	@echo "make all      - compile all files and run tests"
	@echo "make tests    - compile all tests"
	@echo "make runtests - run all tests"
	@echo "make <target> - compile targeted program"
	@echo "make clean    - remove all object files and binaries"	
	@echo ""

all: main tests

runtests: $(UNIT_TESTS)
	@for test in $(UNIT_TESTS); do \
		./$$test; \
	done
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(UNIT_TESTS)

tests/%: tests/%.o $(filter-out main.o, $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $@ -l$(TESTLIB)
	./$@

clean:
	rm -f $(OBJ_FILES) $(EXEC) $(UNIT_TESTS) tests/*.o

.PHONY: all tests clean
