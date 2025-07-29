# IFJ23 Project Structure

This document explains the reorganized structure of the IFJ23 compiler project.

## Directory Structure

```
ifj23/
├── src/                          # All source code
│   ├── main.c                   # Main entry point
│   ├── lexical/                 # Lexical analysis module
│   │   ├── scanner.c           # Lexical analyzer (tokenizer)
│   │   └── token_types.c       # Token type definitions and utilities
│   ├── syntactic/              # Syntactic analysis module
│   │   ├── synt_recursive_parser.c     # Main recursive descent parser
│   │   ├── synt_precedence_parser.c    # Operator precedence parser
│   │   ├── synt_recur_rules.c          # Grammar rules for recursive parser
│   │   ├── synt_prec_rules.c           # Grammar rules for precedence parser
│   │   ├── synt_prec_stack.c           # Stack operations for precedence parser
│   │   ├── expr.c                      # Expression evaluation
│   │   └── expr_stack.c                # Expression stack utilities
│   ├── semantic/               # Semantic analysis module
│   │   └── semantic_analysis.c # Type checking and semantic validation
│   ├── codegen/                # Code generation module
│   │   └── generator.c         # IFJcode23 code generator
│   └── utils/                  # Utility modules
│       ├── dynamic_string.c    # Dynamic string implementation
│       ├── substring.c         # String manipulation utilities
│       ├── symtable.c          # Symbol table implementation
│       ├── symtablestack.c     # Symbol table stack for scope management
│       ├── function_list.c     # Function management utilities
│       ├── istack.c            # Integer stack implementation
│       └── global_variables.c  # Global state management
├── include/                     # All header files
│   ├── scanner.h               # Lexical analyzer interface
│   ├── token_types.h           # Token definitions
│   ├── synt_analysis.h         # Main syntactic analysis interface
│   ├── synt_recursive_parser.h # Recursive parser interface
│   ├── synt_precedence_parser.h # Precedence parser interface
│   ├── synt_recur_rules.h      # Recursive parser rules
│   ├── synt_prec_rules.h       # Precedence parser rules
│   ├── synt_prec_stack.h       # Precedence parser stack
│   ├── synt_prec_table.h       # Precedence table definitions
│   ├── expr.h                  # Expression evaluation interface
│   ├── expr_stack.h            # Expression stack interface
│   ├── semantic_analysis.h     # Semantic analysis interface
│   ├── generator.h             # Code generator interface
│   ├── dynamic_string.h        # Dynamic string interface
│   ├── substring.h             # String utilities interface
│   ├── symtable.h              # Symbol table interface
│   ├── symtablestack.h         # Symbol table stack interface
│   ├── function_list.h         # Function management interface
│   ├── istack.h                # Integer stack interface
│   ├── global_variables.h      # Global variables interface
│   ├── error_codes.h           # Error code definitions
│   └── macros.h                # Common macros and definitions
├── tests/                      # Unit tests
│   ├── colors.h                # Test output coloring
│   ├── operator_precedence_tests.c  # Operator precedence unit tests
│   ├── synt_prec_stack_test.c      # Precedence stack unit tests
│   └── synt_precedence_parser_test.c # Precedence parser unit tests
├── tests_code/                 # Test Swift programs
│   ├── example1.swift          # Basic language examples
│   ├── example2.swift          # More complex examples
│   ├── example3.swift          # Advanced examples
│   ├── factorial.swift         # Factorial implementation
│   ├── factorial_recursive.swift # Recursive factorial
│   ├── builtin.swift           # Built-in functions test
│   ├── ifj23.swift             # Language specification examples
│   └── ...                     # Many more test files
├── tests_integration/          # Integration test suite
│   ├── test_0_assign.swift     # Assignment tests
│   ├── test_0_builtins.swift   # Built-in function tests
│   ├── test_0_call.swift       # Function call tests
│   └── ...                     # Comprehensive test suite
├── docs/                       # Documentation
│   ├── dokumentace.pdf         # Main project documentation (Czech)
│   ├── ifj_LL_table.pdf       # LL parsing table
│   ├── LL_table.jpg            # LL table visualization
│   ├── lex_and_synt.pdf       # Lexical and syntactic analysis docs
│   ├── prezentacia.pdf         # Project presentation
│   └── newRules.md             # Grammar rules documentation
├── build/                      # Build artifacts (created during compilation)
├── Makefile                    # Build configuration
├── .gitignore                  # Git ignore rules
├── README.md                   # Main project documentation
├── rozdeleni                   # Team member contributions
├── test_integration.py         # Integration test runner
├── test.py                     # Additional test script
└── zip_it.py                   # Project packaging script
```

## Module Organization

### Lexical Analysis (`src/lexical/`)
- **Purpose**: Convert source code into tokens
- **Files**: `scanner.c`, `token_types.c`
- **Key Features**: Finite state machine, keyword recognition, literal parsing

### Syntactic Analysis (`src/syntactic/`)
- **Purpose**: Parse tokens into abstract syntax representation
- **Architecture**: Hybrid approach using both recursive descent and operator precedence
- **Files**:
  - Recursive descent parser for statements and declarations
  - Precedence parser for expressions
  - Supporting stack and rule implementations

### Semantic Analysis (`src/semantic/`)
- **Purpose**: Type checking, scope analysis, semantic validation
- **Files**: `semantic_analysis.c`
- **Key Features**: Symbol table management, type compatibility checking

### Code Generation (`src/codegen/`)
- **Purpose**: Generate IFJcode23 intermediate representation
- **Files**: `generator.c`
- **Output**: Stack-based intermediate code

### Utilities (`src/utils/`)
- **Purpose**: Common data structures and helper functions
- **Key Components**:
  - Dynamic strings for efficient string handling
  - Symbol tables for identifier management
  - Stacks for parsing and evaluation
  - Function management utilities

## Build System

The project uses a modular Makefile that:
- Organizes compilation by module
- Supports debug builds
- Includes unit and integration testing
- Generates dependency files automatically
- Creates organized build artifacts in `build/` directory

## Testing Strategy

1. **Unit Tests**: Test individual components in isolation
2. **Integration Tests**: Test complete compilation pipeline
3. **Code Tests**: Validate against Swift language examples
4. **Error Tests**: Verify proper error handling and reporting

This organization improves:
- Code maintainability and readability
- Build system efficiency
- Testing capabilities
- Project navigation and understanding
