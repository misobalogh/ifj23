# Project Name: Implementace překladače imperativního jazyka IFJ23
# Authors: MICHAL BALOGH xbalog06, ADAM ČENĚK xcenek04, TADEAS ZOBAL xzobal02

# Compiler and flags
CC = gcc
CFLAGS = -g -Wextra -Wall -pedantic -std=c11 -Wno-missing-braces
TESTLIB = cunit

# Directories
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include
TEST_DIR = tests

# Target executable
TARGET = ifj23_compiler

# Source files from different modules
LEXICAL_SRC = $(wildcard $(SRC_DIR)/lexical/*.c)
SYNTACTIC_SRC = $(wildcard $(SRC_DIR)/syntactic/*.c)
SEMANTIC_SRC = $(wildcard $(SRC_DIR)/semantic/*.c)
CODEGEN_SRC = $(wildcard $(SRC_DIR)/codegen/*.c)
UTILS_SRC = $(wildcard $(SRC_DIR)/utils/*.c)
MAIN_SRC = $(SRC_DIR)/main.c

# All source files except main
SRC_FILES = $(LEXICAL_SRC) $(SYNTACTIC_SRC) $(SEMANTIC_SRC) $(CODEGEN_SRC) $(UTILS_SRC)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))
MAIN_OBJ = $(BUILD_DIR)/main.o

# Test files
TEST_FILES = $(wildcard $(TEST_DIR)/*.c)
UNIT_TESTS = $(patsubst $(TEST_DIR)/%.c, $(BUILD_DIR)/tests/%, $(TEST_FILES))

# Default target
all: $(TARGET)

# Help target
help:
	@echo ""
	@echo "IFJ23 Compiler Build System"
	@echo "==========================="
	@echo ""
	@echo "Usage:"
	@echo "  make help      - Print this help message"
	@echo "  make           - Build the main compiler"
	@echo "  make all       - Build the main compiler"
	@echo "  make test      - Run integration tests"
	@echo "  make unit      - Build and run unit tests"
	@echo "  make run       - Run the main program with test input"
	@echo "  make clean     - Remove all build artifacts"
	@echo "  make debug     - Build with additional debug flags"
	@echo ""

# Main target
$(TARGET): $(OBJ_FILES) $(MAIN_OBJ)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Debug build
debug: CFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)

# Object files compilation
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Unit tests compilation
$(BUILD_DIR)/tests/%: $(TEST_DIR)/%.c $(OBJ_FILES)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $^ -o $@ -l$(TESTLIB)

# Build all unit tests
unit-build: $(UNIT_TESTS)

# Run unit tests
unit: unit-build
	@echo "Running unit tests..."
	@for test in $(UNIT_TESTS); do \
		if [ -f $$test ]; then \
			echo "Running $$test..."; \
			./$$test; \
		fi; \
	done

# Integration tests
test: $(TARGET)
	@echo "Running integration tests..."
	@if [ -f test_integration.py ]; then \
		python3 test_integration.py; \
	else \
		echo "Integration test script not found"; \
	fi

# Run with sample input
run: $(TARGET)
	@if [ -f tests_code/factorial.swift ]; then \
		./$(TARGET) < tests_code/factorial.swift; \
	else \
		echo "No test input found. Run: ./$(TARGET) < your_input.swift"; \
	fi

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET) *.log *.o

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Phony targets
.PHONY: all help debug unit unit-build test run clean

# Dependencies (automatically generated)
-include $(OBJ_FILES:.o=.d)
-include $(MAIN_OBJ:.o=.d)

# Generate dependencies
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	@$(CC) -I$(INCLUDE_DIR) -MM $< | sed 's|^|$(BUILD_DIR)/|' > $@
