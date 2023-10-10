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
+-----------+---------------+--------------+
| Priorita  |   Operátory   | Asociativita |
+-----------+---------------+--------------+
|     0     |      !        |   bez asoc.  |
+-----------+---------------+--------------+
|     1     |     * /       |     levá     |
+-----------+---------------+--------------+
|     2     |     + -       |     levá     |
+-----------+---------------+--------------+
|     3     |== != < > <= >=|   bez asoc.  |
+-----------+---------------+--------------+
|     4     |      ??       |    pravá     |
+-----------+---------------+--------------+
*/

#include "string.h"

#ifndef OPERATOR_PRECEDENCE_H
#define OPERATOR_PRECEDENCE_H

/**
 * @brief Enum of operator precedence.
 *
 * Operator precedence is in inverse order as in table,
 * so the higher the number, the higher the precedence
 * It is because its more natural to ask, if precedence of operator1
 * is higher (not lower) than precedence of operator2.
*/
enum operator_precedence {
    OP_NOT = 4,
    OP_MUL_DIV = 3,
    OP_PLUS_MINUS = 2,
    OP_REL = 1,
    OP_CONCAT = 0
};

/* Operators */
#define NOT "!"
#define MUL "*" 
#define DIV "/"
#define PLUS "+"
#define MINUS "-"
#define REL_EQ "==" 
#define REL_NEQ "!="
#define REL_LESS "<"
#define REL_MORE ">"
#define REL_LESS_EQ "<="
#define REL_MORE_EQ ">="
#define CONCAT "??"

/**
 * @brief Macro for getting precedence of operator.
*/
#define PRECEDENCE(operator) ( strcmp(operator, NOT) == 0 ? OP_NOT : \
                                strcmp(operator, MUL) == 0 ? OP_MUL_DIV : \
                                strcmp(operator, DIV) == 0 ? OP_MUL_DIV : \
                                strcmp(operator, PLUS) == 0 ? OP_PLUS_MINUS : \
                                strcmp(operator, MINUS) == 0 ? OP_PLUS_MINUS : \
                                strcmp(operator, REL_EQ) == 0 ? OP_REL : \
                                strcmp(operator, REL_NEQ) == 0 ? OP_REL : \
                                strcmp(operator, REL_LESS) == 0 ? OP_REL : \
                                strcmp(operator, REL_MORE) == 0 ? OP_REL : \
                                strcmp(operator, REL_LESS_EQ) == 0 ? OP_REL : \
                                strcmp(operator, REL_MORE_EQ) == 0 ? OP_REL : \
                                strcmp(operator, CONCAT) == 0 ? OP_CONCAT : -1 )




#endif // OPERATOR_PRECEDENCE_H
