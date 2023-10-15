/********************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23  *
* File Name: operator_precedence.h                                  *
* Description: Table of operator precedence                         *
* Author:  Michal Balogh xbalog06                                   *
* Faculty: FIT VUT                                                  *
* Date:    10.10.2023                                               *
*                                                                   *
* Comments: operator precedence is in inverse order as in table,    *
*           so the higher the number, the higher the precedence     *
*                                                                   *
********************************************************************/

/*
+-----------+---------------+--------------+-----------------+
| Priorita  |   Operátory   | Asociativita |      Enum       |
+-----------+---------------+--------------+-----------------+
|     0     |      !        |   bez asoc.  | OP_FORCE_UNWRAP |
+-----------+---------------+--------------+-----------------+
|     1     |     * /       |     levá     |   OP_MUL_DIV    |
+-----------+---------------+--------------+-----------------+
|     2     |     + -       |     levá     |  OP_PLUS_MINUS  |
+-----------+---------------+--------------+-----------------+
|     3     |== != < > <= >=|   bez asoc.  |      OP_REL     |
+-----------+---------------+--------------+-----------------+
|     4     |      ??       |    pravá     |    OP_CONCAT    |
+-----------+---------------+--------------+-----------------+
*/

#ifndef OPERATOR_PRECEDENCE_H
#define OPERATOR_PRECEDENCE_H

#include <string.h>
#include "macros.h"
/**
 * @brief Enum of operator precedence.
 *
 * Operator precedence is in inverse order as in table,
 * so the higher the number, the higher the precedence
 * It is because its more natural to ask, if precedence of operator1
 * is higher (not lower) than precedence of operator2.
*/
enum operator_precedence {
    OP_FORCE_UNWRAP = 4,
    OP_MUL_DIV      = 3,
    OP_PLUS_MINUS   = 2,
    OP_REL          = 1,
    OP_CONCAT       = 0
};

/* Operators */
#define MUL             "*" 
#define DIV             "/"
#define PLUS            "+"
#define MINUS           "-"
#define REL_EQ          "==" 
#define REL_NEQ         "!="
#define REL_LESS        "<"
#define REL_MORE        ">"
#define REL_LESS_EQ     "<="
#define REL_MORE_EQ     ">="
#define CONCAT          "??"
#define FORCE_UNWRAP    "!"

/**
 * @brief Macro for getting precedence of operator.
*/
#define PRECEDENCE(operator) ( STRING_EQ(operator, MUL)             ? OP_MUL_DIV :      \
                               STRING_EQ(operator, DIV)             ? OP_MUL_DIV :      \
                               STRING_EQ(operator, PLUS)            ? OP_PLUS_MINUS :   \
                               STRING_EQ(operator, MINUS)           ? OP_PLUS_MINUS :   \
                               STRING_EQ(operator, REL_EQ)          ? OP_REL :          \
                               STRING_EQ(operator, REL_NEQ)         ? OP_REL :          \
                               STRING_EQ(operator, REL_LESS)        ? OP_REL :          \
                               STRING_EQ(operator, REL_MORE)        ? OP_REL :          \
                               STRING_EQ(operator, REL_LESS_EQ)     ? OP_REL :          \
                               STRING_EQ(operator, REL_MORE_EQ)     ? OP_REL :          \
                               STRING_EQ(operator, CONCAT)          ? OP_CONCAT :       \
                               STRING_EQ(operator, FORCE_UNWRAP)    ? OP_FORCE_UNWRAP : \
                               -1 )


// bool IsOperator(char* operator) {
//     return STRING_EQ(operator, MUL)          ||
//            STRING_EQ(operator, DIV)          ||
//            STRING_EQ(operator, PLUS)         ||
//            STRING_EQ(operator, MINUS)        ||
//            STRING_EQ(operator, REL_EQ)       ||
//            STRING_EQ(operator, REL_NEQ)      ||
//            STRING_EQ(operator, REL_LESS)     ||
//            STRING_EQ(operator, REL_MORE)     ||
//            STRING_EQ(operator, REL_LESS_EQ)  ||
//            STRING_EQ(operator, REL_MORE_EQ)  ||
//            STRING_EQ(operator, CONCAT)       ||
//            STRING_EQ(operator, FORCE_UNWRAP);
// }

#endif // OPERATOR_PRECEDENCE_H
