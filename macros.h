/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: macros.h
* Description: macros for the whole project
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include "error_codes.h"
#include "tests/colors.h"
#include "token_types.h"


const char* errorToString(error_codes err);

#define EXIT_WITH_MESSAGE(err) do { \
    fprintf(stderr, "Error code: %i %s in %s\n", err, errorToString(err), __func__); \
    exit(err); \
  } while (0)

/**
 * @brief Check if memory allocation was successful.
 *
 * Macro to check if memory allocation was successful. If not, it prints an error message to
 * the stderr and exits with the specified error code.
 *
 * @param ptr Pointer to allocated memory
 */
#define CHECK_MEMORY_ALLOC(ptr)                                                                 \
    do {                                                                                        \
        if (!(ptr)) {                                                                           \
            fprintf(stderr, "ERR_CODE: %d - Memory allocation failed\n", INTERNAL_ERROR);       \
            exit(INTERNAL_ERROR);                                                               \
        }                                                                                       \
    } while (0)                                                                                 \


 /**
  * @brief Compares two strings, if they are equal, returns true.
  */
#define STRING_EQ(str1, str2) (strcmp(str1, str2) == 0)

  //================= DEBUGGING =================
  /**
   * @brief Debug macro for printing to stderr.
   */
#define LOG(fmt, ...) fprintf(stderr,fmt "\n", __VA_ARGS__)


#define LOG_ERR(message) do {                           \
    FILE *logFile = fopen("error.log", "a");            \
    if (logFile) {                                      \
        fprintf(logFile, "ERR: %s\n", message);         \
        fclose(logFile);                                \
    } else {                                            \
        fprintf(stderr, "Failed to open log file.\n");  \
    }                                                   \
} while (0)                                             \


#define PLOG(message) do {                           \
    FILE *logFile = fopen("precedenceParser.log", "a");            \
    if (logFile) {                                      \
        fprintf(logFile, "%s\n", message);         \
        fclose(logFile);                                \
    } else {                                            \
        fprintf(stderr, "Failed to open log file.\n");  \
    }                                                   \
} while (0)

#define RLOG(message) do {                           \
    FILE *logFile = fopen("recursiveParser.log", "a");            \
    if (logFile) {                                      \
        fprintf(logFile, "%s\n", message);         \
        fclose(logFile);                                \
    } else {                                            \
        fprintf(stderr, "Failed to open log file.\n");  \
    }                                                   \
} while (0)

#define PRINT_TOKEN(token) \
do { \
    switch(token) { \
        case token_LEX_ERROR: printf("token_LEX_ERROR\n"); break; \
        case token_OP_START: printf("token_OP_START\n"); break; \
        case token_PLUS: printf("token_PLUS\n"); break; \
        case token_MINUS: printf("token_MINUS\n"); break; \
        case token_MUL: printf("token_MUL\n"); break; \
        case token_DIV: printf("token_DIV\n"); break; \
        case token_DEFAULT_VALUE: printf("token_DEFAULT_VALUE\n"); break; \
        case token_FORCE_UNWRAP: printf("token_FORCE_UNWRAP\n"); break; \
        case token_REL: printf("token_REL\n"); break; \
        case token_EQ: printf("token_EQ\n"); break; \
        case token_NEQ: printf("token_NEQ\n"); break; \
        case token_LESS: printf("token_LESS\n"); break; \
        case token_MORE: printf("token_MORE\n"); break; \
        case token_LESS_EQ: printf("token_LESS_EQ\n"); break; \
        case token_MORE_EQ: printf("token_MORE_EQ\n"); break; \
        case token_OP_END: printf("token_OP_END\n"); break; \
        case token_PARENTHESES_L: printf("token_PARENTHESES_L\n"); break; \
        case token_PARENTHESES_R: printf("token_PARENTHESES_R\n"); break; \
        case token_ID: printf("token_ID\n"); break; \
        case token_ID_OR_CONST: printf("token_ID_OR_CONST\n"); break; \
        case token_CONST: printf("token_CONST\n"); break; \
        case token_CONST_WHOLE_NUMBER: printf("token_CONST_WHOLE_NUMBER\n"); break; \
        case token_CONST_DEC_NUMBER: printf("token_CONST_DEC_NUMBER\n"); break; \
        case token_CONST_SCIENTIFIC_NOTATION: printf("token_CONST_SCIENTIFIC_NOTATION\n"); break; \
        case token_DOLLAR: printf("token_DOLLAR\n"); break; \
        case token_TERMINAL: printf("token_TERMINAL\n"); break; \
        case token_NONTERMINAL: printf("token_NONTERMINAL\n"); break; \
        case token_LET: printf("token_LET\n"); break; \
        case token_VAR: printf("token_VAR\n"); break; \
        case token_IF: printf("token_IF\n"); break; \
        case token_ELSE: printf("token_ELSE\n"); break; \
        case token_WHILE: printf("token_WHILE\n"); break; \
        case token_NIL: printf("token_NIL\n"); break; \
        case token_FUNC: printf("token_FUNC\n"); break; \
        case token_RETURN: printf("token_RETURN\n"); break; \
        case token_BRACKET_L: printf("token_BRACKET_L\n"); break; \
        case token_BRACKET_R: printf("token_BRACKET_R\n"); break; \
        case token_COMMA: printf("token_COMMA\n"); break; \
        case token_ARROW: printf("token_ARROW\n"); break; \
        case token_ASSIGN: printf("token_ASSIGN\n"); break; \
        case token_COLON: printf("token_COLON\n"); break; \
        case token_SEMICOLON: printf("token_SEMICOLON\n"); break; \
        case token_UNDERSCORE: printf("token_UNDERSCORE\n"); break; \
        case token_TYPE: printf("token_TYPE\n"); break; \
        case token_TYPE_INT: printf("token_TYPE_INT\n"); break; \
        case token_TYPE_INT_Q: printf("token_TYPE_INT_Q\n"); break; \
        case token_TYPE_DOUBLE: printf("token_TYPE_DOUBLE\n"); break; \
        case token_TYPE_DOUBLE_Q: printf("token_TYPE_DOUBLE_Q\n"); break; \
        case token_TYPE_STRING_LINE: printf("token_TYPE_STRING_LINE\n"); break; \
        case token_TYPE_STRING: printf("token_TYPE_STRING\n"); break; \
        case token_TYPE_STRING_Q: printf("token_TYPE_STRING_Q\n"); break; \
        case token_EOL: printf("token_EOL\n"); break; \
        case token_EOF: printf("token_EOF\n"); break; \
        case token_EMPTY: printf("token_EMPTY\n"); break; \
        default: printf("Unknown token\n"); break; \
    } \
} while(0)


#endif // MACROS_H
