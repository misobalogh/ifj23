# Project Name: Implementace překladače imperativního jazyka IFJ23
# Authors: MICHAL BALOGH xbalog06, 

CFLAGS = -g -Wextra -Wall -pedantic -std=c11
CC = gcc
TESTLIB = cunit

SRC_DIR = .
OBJ_DIR = obj
TARGET = main

SRC_FILES = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
EXEC = $(basename $(SRC_FILES))
TEST_FILES = $(wildcard tests/*.c)
UNIT_TESTS = $(basename $(TEST_FILES))


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
	rm -rf *.log
	./main

$(TARGET): $(OBJ_FILES) $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ_FILES) $(SRC_DIR)/main.c


$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

tests: $(UNIT_TESTS)

tests/%: tests/%.o $(filter-out $(OBJ_DIR)/main.o, $(OBJ_FILES))
	$(CC) $(CFLAGS) $^ -o $@ -l$(TESTLIB)

clean:
	rm -rf $(OBJ_DIR) $(TARGET) $(EXEC) $(UNIT_TESTS) *.log

.PHONY: all clean
