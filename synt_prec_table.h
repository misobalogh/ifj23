
#ifndef SYNT_PREC_TABLE_H
#define SYNT_PREC_TABLE_H


#include "token_types.h"
#include <stdio.h>

typedef enum precedenceTypes {
    LOW,
    HIGH,
    EQUAL,
    EMPTY,
} precedenceType;


extern const precedenceType precedenceTable[9][9];


//========== DEBUGGING ==========//
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

#endif // SYNT_PREC_TABLE_H
