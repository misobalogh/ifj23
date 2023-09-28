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

all: main tests

run:
	./main

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
