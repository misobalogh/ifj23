# Project Name: Implementace překladače imperativního jazyka IFJ23
# Authors: MICHAL BALOGH xbalog06, 

CFLAGS = -g -Wextra -Wall -pedantic -std=c11 -Wno-missing-braces -DDEBUG
CC = gcc
TESTLIB = cunit
LD = ld

SRC_DIR = .
OBJ_DIR = .
TARGET = main

SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXEC = $(basename $(SRC_FILES))
TEST_FILES = $(wildcard tests/*.c)
UNIT_TESTS = $(basename $(TEST_FILES))

all: main

help:
	@echo ""
	@echo "Usage:"
	@echo "  make help      - Print this help message"
	@echo "  make           - Print this help message"
	@echo "  make all       - Compile all files and run tests"
	@echo "  make test      - Runs integration test on the program"
	@echo "  make test (test name) Runs only specific test"
	@echo "  make run       - Run the main program"
	@echo "  make main      - Compile the main program"
	@echo "  make tests     - Compile all unit tests"
	@echo "  make runtests  - Run all unit tests"
	@echo "  make <target>  - Compile a specific target program"
	@echo "  make clean     - Remove all object files and binaries"	
	@echo ""



string_test: stest.c dynamic_string.c
	$(CC) $(CFLAGS) $^ -o $@

substr.o: substring.code
	$(LD) -r -b binary -o $@ $^

test: main
	rm -rf *.log
	python3 test_integration.py  $(word 2, $(MAKECMDGOALS))

test-code: main
	python3 ./test.py

rununit: $(UNIT_TESTS)
	@for test in $(UNIT_TESTS); do \
		./$$test; \
	done

run: main
	rm -rf *.log
	./main <tests_integration/test_current.swift

$(TARGET): $(OBJ_FILES) substr.o $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $(TARGET) substr.o $(OBJ_FILES) $(SRC_DIR)/main.c


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(UNIT_TESTS)

tests/%: tests/%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $@ -l$(TESTLIB)

clean:
	rm -rf $(TARGET) $(EXEC) $(UNIT_TESTS) *.log *.o ./obj

.PHONY: all clean
