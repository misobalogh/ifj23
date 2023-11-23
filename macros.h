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
#include "token_types.h"


#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define RESET   "\033[0m"


const char* errorToString(error_codes err);

#define EXIT_WITH_MESSAGE(err) do { \
    fprintf(stderr,"Error code: %i %s in %s\n", err, errorToString(err), __func__); \
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
        case token_LEX_ERROR: fprintf(stderr,"token_LEX_ERROR\n"); break; \
        case token_OP_START: fprintf(stderr,"token_OP_START\n"); break; \
        case token_PLUS: fprintf(stderr,"token_PLUS\n"); break; \
        case token_MINUS: fprintf(stderr,"token_MINUS\n"); break; \
        case token_MUL: fprintf(stderr,"token_MUL\n"); break; \
        case token_DIV: fprintf(stderr,"token_DIV\n"); break; \
        case token_DEFAULT_VALUE: fprintf(stderr,"token_DEFAULT_VALUE\n"); break; \
        case token_FORCE_UNWRAP: fprintf(stderr,"token_FORCE_UNWRAP\n"); break; \
        case token_REL: fprintf(stderr,"token_REL\n"); break; \
        case token_EQ: fprintf(stderr,"token_EQ\n"); break; \
        case token_NEQ: fprintf(stderr,"token_NEQ\n"); break; \
        case token_LESS: fprintf(stderr,"token_LESS\n"); break; \
        case token_MORE: fprintf(stderr,"token_MORE\n"); break; \
        case token_LESS_EQ: fprintf(stderr,"token_LESS_EQ\n"); break; \
        case token_MORE_EQ: fprintf(stderr,"token_MORE_EQ\n"); break; \
        case token_OP_END: fprintf(stderr,"token_OP_END\n"); break; \
        case token_PARENTHESES_L: fprintf(stderr,"token_PARENTHESES_L\n"); break; \
        case token_PARENTHESES_R: fprintf(stderr,"token_PARENTHESES_R\n"); break; \
        case token_ID: fprintf(stderr,"token_ID\n"); break; \
        case token_ID_OR_CONST: fprintf(stderr,"token_ID_OR_CONST\n"); break; \
        case token_CONST: fprintf(stderr,"token_CONST\n"); break; \
        case token_CONST_WHOLE_NUMBER: fprintf(stderr,"token_CONST_WHOLE_NUMBER\n"); break; \
        case token_CONST_DEC_NUMBER: fprintf(stderr,"token_CONST_DEC_NUMBER\n"); break; \
        case token_CONST_SCIENTIFIC_NOTATION: fprintf(stderr,"token_CONST_SCIENTIFIC_NOTATION\n"); break; \
        case token_DOLLAR: fprintf(stderr,"token_DOLLAR\n"); break; \
        case token_TERMINAL: fprintf(stderr,"token_TERMINAL\n"); break; \
        case token_NONTERMINAL: fprintf(stderr,"token_NONTERMINAL\n"); break; \
        case token_LET: fprintf(stderr,"token_LET\n"); break; \
        case token_VAR: fprintf(stderr,"token_VAR\n"); break; \
        case token_IF: fprintf(stderr,"token_IF\n"); break; \
        case token_ELSE: fprintf(stderr,"token_ELSE\n"); break; \
        case token_WHILE: fprintf(stderr,"token_WHILE\n"); break; \
        case token_NIL: fprintf(stderr,"token_NIL\n"); break; \
        case token_FUNC: fprintf(stderr,"token_FUNC\n"); break; \
        case token_RETURN: fprintf(stderr,"token_RETURN\n"); break; \
        case token_BRACKET_L: fprintf(stderr,"token_BRACKET_L\n"); break; \
        case token_BRACKET_R: fprintf(stderr,"token_BRACKET_R\n"); break; \
        case token_COMMA: fprintf(stderr,"token_COMMA\n"); break; \
        case token_ARROW: fprintf(stderr,"token_ARROW\n"); break; \
        case token_ASSIGN: fprintf(stderr,"token_ASSIGN\n"); break; \
        case token_COLON: fprintf(stderr,"token_COLON\n"); break; \
        case token_SEMICOLON: fprintf(stderr,"token_SEMICOLON\n"); break; \
        case token_UNDERSCORE: fprintf(stderr,"token_UNDERSCORE\n"); break; \
        case token_TYPE: fprintf(stderr,"token_TYPE\n"); break; \
        case token_TYPE_INT: fprintf(stderr,"token_TYPE_INT\n"); break; \
        case token_TYPE_INT_Q: fprintf(stderr,"token_TYPE_INT_Q\n"); break; \
        case token_TYPE_DOUBLE: fprintf(stderr,"token_TYPE_DOUBLE\n"); break; \
        case token_TYPE_DOUBLE_Q: fprintf(stderr,"token_TYPE_DOUBLE_Q\n"); break; \
        case token_TYPE_STRING_LINE: fprintf(stderr,"token_TYPE_STRING_LINE\n"); break; \
        case token_TYPE_STRING: fprintf(stderr,"token_TYPE_STRING\n"); break; \
        case token_TYPE_STRING_Q: fprintf(stderr,"token_TYPE_STRING_Q\n"); break; \
        case token_EOL: fprintf(stderr,"token_EOL\n"); break; \
        case token_EOF: fprintf(stderr,"token_EOF\n"); break; \
        case token_EMPTY: fprintf(stderr,"token_EMPTY\n"); break; \
        default: fprintf(stderr,"Unknown token\n"); break; \
    } \
} while(0)


#endif // MACROS_H
