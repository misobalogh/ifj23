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
 * @param token_CONCAT optional
 * @param token_DOLLAR end of expression
 * @param token_PARENTHESES_L left parentheses
 * @param token_PARENTHESES_R right parentheses
 * @param token_ID identifier
 */
typedef enum
{
    token_LEX_ERROR = -1,
    token_OP_START = 0,
    token_PLUS_MINUS = 0,
    token_PLUS = 0,
    token_MINUS = 0,
    token_MUL_DIV = 1,
    token_MUL = 1,
    token_DIV = 1,
    token_CONCAT = 2,
    token_DEFAULT_VALUE = token_CONCAT, // remove token CONCAT later
    token_FORCE_UNWRAP = 3,
    token_REL = 4,
    token_EQ = 4,
    token_NEQ = 4,
    token_LESS = 4,
    token_MORE = 4,
    token_LESS_EQ = 4,
    token_MORE_EQ = 4,
    token_OP_END = 4,
    token_PARENTHESES_L = 5,
    token_PARENTHESES_R = 6,
    token_ID = 7,
    token_ID_OR_CONST = 7,
    token_DOLLAR = 8,
    // ...
    token_TERMINAL = 9,
    // ...
    token_NONTERMINAL = 10, // remove later
    token_LET,
    token_VAR,
    token_IF,
    token_ELSE,
    token_WHILE,
    token_CONST,
    token_NOT,
    token_NIL,
    token_CONST_WHOLE_NUMBER = token_CONST,
    token_CONST_DEC_NUMBER = token_CONST,
    token_CONST_SCIENTIFIC_NOTATION = token_CONST,
    token_FUNC,
    token_RETURN,
    token_BRACKET_L,
    token_BRACKET_R,
    token_COMMA,
    token_ARROW,
    token_ASSIGN, // "="
    token_COLON,
    token_SEMICOLON,
    token_UNDERSCORE, // "_" for function param name
    token_TYPE,
    token_TYPE_INT,
    token_TYPE_INT_Q,
    token_TYPE_DOUBLE,
    token_TYPE_DOUBLE_Q,
    token_TYPE_STRING_LINE,
    token_TYPE_STRING,
    token_TYPE_STRING_Q,
    token_EOL,
    token_EOF,
} tokenType;

typedef struct token
{
    tokenType type;
    char *value;
} tokenStruct;

bool isTerminal(tokenType type);

// union for specific token data type
typedef union Value_of_token
{
    int INT_VAL;
    float FLOAT_VAL;
    char *STR_VAL;
} value_of_token;

// structure of token
typedef struct Token
{
    value_of_token token_value;
    tokenType token_type;
    int line;
} lex_token;

// structure of dynamic string
typedef struct Dynamic_String
{
    char *data;            // current cuntent
    unsigned int size;     // current size
    unsigned int capacity; // max capacity
} dynamic_string;

// function to get new token
lex_token get_next_token();

// current token
extern lex_token current_lex_token;

// current state
// extern states current_lex_state;

#endif // _TOKEN_TYPES_H