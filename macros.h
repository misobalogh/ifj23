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

/**
 * @brief Check if memory allocation was successful.
 * 
 * Macro to check if memory allocation was successful. If not, it prints an error message to
 * the stderr and exits with the specified error code.
 * 
 * @param ptr Pointer to allocated memory
 * @param error_code Error code to exit with
 */
#define CHECK_MEMORY_ALLOC(ptr, error_code)                      \
    do {                                                         \
        if (!(ptr)) {                                            \
            fprintf(stderr, "ERR_CODE: %d - Memory allocation failed\n", error_code);       \
            exit(error_code);                                    \
        }                                                        \
    } while (0)

#endif // MACROS_H