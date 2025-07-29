# IFJ23 Compiler

A compiler implementation for the IFJ23 programming language, developed as part of the Formal Languages and Compilers course at FIT VUT Brno.

## 📖 Overview

This project implements a complete compiler for the IFJ23 language (a simplified Swift-like language) that translates source code into IFJcode23 (a stack-based intermediate representation). The compiler performs lexical analysis, syntactic analysis, semantic analysis, and code generation.

## 🏗️ Architecture

The compiler follows a traditional multi-phase design:

1. **Lexical Analysis** - Tokenizes the input source code
2. **Syntactic Analysis** - Parses tokens using recursive descent and precedence parsing
3. **Semantic Analysis** - Type checking and semantic validation
4. **Code Generation** - Generates IFJcode23 intermediate code

## 📁 Project Structure

```
ifj23/
├── src/                     # Source code
│   ├── main.c               # Main program entry point
│   ├── lexical/             # Lexical analysis
│   │   ├── scanner.c        # Lexical analyzer implementation
│   │   └── token_types.c    # Token type definitions
│   ├── syntactic/           # Syntactic analysis
│   │   ├── synt_recursive_parser.c    # Recursive descent parser
│   │   ├── synt_precedence_parser.c   # Precedence parser for expressions
│   │   ├── synt_recur_rules.c         # Recursive parser rules
│   │   ├── synt_prec_rules.c          # Precedence parser rules
│   │   ├── synt_prec_stack.c          # Precedence parser stack
│   │   ├── expr.c                     # Expression handling
│   │   └── expr_stack.c               # Expression stack operations
│   ├── semantic/            # Semantic analysis
│   │   └── semantic_analysis.c        # Semantic analyzer
│   ├── codegen/             # Code generation
│   │   └── generator.c      # Code generator
│   └── utils/               # Utility modules
│       ├── dynamic_string.c # Dynamic string implementation
│       ├── substring.c      # Substring operations
│       ├── symtable.c       # Symbol table
│       ├── symtablestack.c  # Symbol table stack
│       ├── function_list.c  # Function management
│       ├── istack.c         # Integer stack
│       └── global_variables.c # Global variables
├── include/                 # Header files
│   ├── scanner.h            # Lexical analyzer interface
│   ├── token_types.h        # Token type definitions
│   ├── synt_analysis.h      # Syntactic analysis interface
│   ├── synt_*.h             # Various syntactic analysis headers
│   ├── semantic_analysis.h  # Semantic analysis interface
│   ├── generator.h          # Code generator interface
│   ├── error_codes.h        # Error code definitions
│   ├── macros.h             # Common macros
│   └── *.h                  # Other utility headers
├── tests/                   # Unit tests
│   ├── operator_precedence_tests.c
│   ├── synt_prec_stack_test.c
│   └── synt_precedence_parser_test.c
├── tests_code/              # Test Swift programs
│   ├── example1.swift       # Basic examples
│   ├── factorial.swift      # Factorial implementation
│   ├── builtin.swift        # Built-in functions test
│   └── *.swift              # Various test cases
├── tests_integration/       # Integration tests
├── docs/                    # Documentation
│   ├── dokumentace.pdf      # Project documentation
│   ├── LL_table.jpg         # LL parsing table
│   └── *.pdf                # Additional documentation
├── build/                   # Build artifacts (created during compilation)
├── Makefile                 # Build configuration
└── README.md               # This file
```

## 🛠️ Building the Project

### Prerequisites

- GCC compiler with C11 support
- Make utility
- CUnit library (for running tests)

### Build Commands

```bash
# Build the main compiler
make

# Build and run tests
make test

# Clean build artifacts
make clean

# Build in debug mode
make debug
```

### Manual Build

If you prefer to build manually:

```bash
gcc -std=c11 -Wall -Wextra -pedantic -g \
    src/main.c \
    src/lexical/*.c \
    src/syntactic/*.c \
    src/semantic/*.c \
    src/codegen/*.c \
    src/utils/*.c \
    -Iinclude \
    -o ifj23_compiler
```

## 🚀 Usage

```bash
# Compile a Swift program
./ifj23_compiler < input.swift > output.ifjcode23

# Example
./ifj23_compiler < tests_code/factorial.swift > factorial.ifjcode23
```

### Input/Output

- **Input**: IFJ23 source code (Swift-like syntax) via stdin
- **Output**: IFJcode23 intermediate code via stdout
- **Errors**: Error messages and codes via stderr

## 🧪 Testing

### Unit Tests

```bash
# Run all unit tests
make test

# Run specific test modules
./tests/operator_precedence_tests
./tests/synt_prec_stack_test
./tests/synt_precedence_parser_test
```

### Integration Tests

```bash
# Run integration tests
python3 test_integration.py

# Run specific test cases
./ifj23_compiler < tests_code/factorial.swift
./ifj23_compiler < tests_integration/test_0_builtins.swift
```

## 📋 Language Features

The IFJ23 compiler supports:

- **Data Types**: Int, Double, String, Bool, optional types
- **Variables**: Declaration, assignment, optional binding (`if let`)
- **Functions**: Definition, calls, parameters, return values
- **Control Structures**: `if-else`, `while` loops
- **Operators**: Arithmetic, comparison, logical, string concatenation
- **Built-in Functions**: `readString()`, `readInt()`, `readDouble()`, `write()`, `Int2Double()`, `Double2Int()`, `length()`, `substring()`
- **Comments**: Single-line (`//`) and multi-line (`/* */`)

## 🔧 Error Handling

The compiler returns specific exit codes for different error types:

- `0` - Success
- `1` - Lexical analysis error
- `2` - Syntax analysis error
- `3` - Semantic analysis error (undefined function/variable)
- `4` - Type compatibility error
- `5` - Function parameter error
- `6` - Other semantic errors
- `7` - Runtime error (division by zero, etc.)
- `99` - Internal error

## 👥 Team Members

- **Michal Balogh** (xbalog06) - Team Leader
- **Adam Čeněk** (xcenek04)
- **Tadeas Zobal** (xzobal02)

## 📚 Documentation

Detailed documentation can be found in the `docs/` directory:
- `dokumentace.pdf` - Complete project documentation (Czech)
- `LL_table.jpg` - LL parsing table visualization
- `lex_and_synt.pdf` - Lexical and syntactic analysis details

## 🔗 Additional Resources

- [IFJ23 Language Specification](docs/dokumentace.pdf)
- [IFJcode23 Documentation](https://www.fit.vutbr.cz/study/courses/IFJ/public/project/)
- [Swift Language Reference](https://docs.swift.org/swift-book/ReferenceManual/)



