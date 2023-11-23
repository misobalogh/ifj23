/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_prec_table.h
* Description: Precedence table for precedence parser (parser for expression)
* Author: MICHAL BALOGH, xbalog06
* Faculty: FIT VUT
* Date: 14.10.2023

* Comments:

***************************************************************/

#ifndef SYNT_PREC_TABLE_H
#define SYNT_PREC_TABLE_H


#include "token_types.h"
#include <stdio.h>

/**
 * @brief Enum for types of precedence table
 *
 * @param LOW means "<"
 * @param HIGH means ">"
 * @param EQUAL means "="
 * @param EMPTY means " "
 */
typedef enum precedenceTypes {
    LOW,
    HIGH,
    EQUAL,
    EMPTY,
} precedenceType;

/**
 * @brief Precedence table for precedence parser.
 * Precedence parsers uses this table, when it needs to decide,
 * what to do with stack - whether to reduce or shift.
 */
extern const precedenceType precedenceTable[9][9];


//========== DEBUGGING ==========//
/*
#define LOG_TABLE(number) do { \
    if (number == HIGH) \
        printf("HIGH\n"); \
    else if (number == LOW) \
        printf("LOW\n"); \
    else if (number == EQUAL) \
        printf("EQUAL\n"); \
    else if (number == EMPTY) \
        printf("EMPTY\n"); \
    else \
        printf("ERROR\n"); \
} while (0)
*/
#endif // SYNT_PREC_TABLE_H
