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


#define PLOG(message) do {                              \
    FILE *logFile = fopen("precedenceParser.log", "a"); \
    if (logFile) {                                      \
        fprintf(logFile, "%s\n", message);              \
        fclose(logFile);                                \
    } else {                                            \
        fprintf(stderr, "Failed to open log file.\n");  \
    }                                                   \
} while (0)

#define RLOG(message) do {                              \
    FILE *logFile = fopen("recursiveParser.log", "a");  \
    if (logFile) {                                      \
        fprintf(logFile, "%s\n", message);              \
        fclose(logFile);                                \
    } else {                                            \
        fprintf(stderr, "Failed to open log file.\n");  \
    }                                                   \
} while (0)

#endif // MACROS_H
