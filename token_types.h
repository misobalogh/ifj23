/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: token_types.c
* Description: types of tokens, which are used in syntactic analysis, to know, which rule to apply
* Author: MICHAL BALOGH, xbalog06
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments: 

***************************************************************/

#ifndef _TOKEN_TYPES_H
#define _TOKEN_TYPES_H

#include <stdbool.h>

#define PARENTHESES_LEFT "("
#define PARENTHESES_RIGHT ")"

/**
 * @brief Enum of token types.
 * 
 * @param token_NOT negation
 * @param token_MUL multiplication
 * @param token_DIV division
 * @param token_PLUS addition
 * @param token_MINUS subtraction
 * @param token_EQ equal
 * @param token_NEQ not equal
 * @param token_LESS less than
 * @param token_MORE more than
 * @param token_LESS_EQ less than or equal
 * @param token_MORE_EQ more than or equal
 * @param token_CONCAT concatenation
 * @param token_DOLLAR end of expression
 * @param token_PARENTHESES_L left parentheses
 * @param token_PARENTHESES_R right parentheses
 * @param token_ID identifier
 */
typedef enum {
    token_PLUS_MINUS = 0,
    token_PLUS = 0,
    token_MINUS = 0,
    token_MUL_DIV = 1,
    token_MUL = 1,
    token_DIV = 1,  
    token_CONCAT = 2,
    token_FORCE_UNWRAP = 3,
    token_REL = 4,
    token_EQ = 4,
    token_NEQ = 4,
    token_LESS = 4,
    token_MORE = 4,
    token_LESS_EQ = 4,
    token_MORE_EQ = 4,  
    token_PARENTHESES_L = 5,
    token_PARENTHESES_R = 6,
    token_ID = 7,
    token_DOLLAR = 8,
   
    // ...
    token_TERMINAL = 9,
    // ...
    token_NONTERMINAL = 10,
    token_LET,
    token_VAR,
    token_ASSIGN, // "="
    token_EXPRESSION,
    token_EOL,
    token_EOF,

} tokenType;

typedef struct token
{
    tokenType type;
    char *value;
} token;


bool isTerminal(tokenType type);
bool isNonterminal(tokenType type);



// ================ DEBUGGING ================
/**
 * @brief For debugging, delete later - assigns name to token type
 * 
 * @param type type of token u want to get name of
*/
#define TOKEN_TYPE_NAME(type) tokenTypeNames[type]
// also for debugging
extern const char* tokenTypeNames[];


#endif // _TOKEN_TYPES_H