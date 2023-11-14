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
    token_LEX_ERROR,
    token_OP_START,
    token_PLUS,
    token_MINUS,
    token_MUL,
    token_DIV,
    token_CONCAT,
    token_DEFAULT_VALUE = token_CONCAT, // remove token CONCAT later
    token_FORCE_UNWRAP,
    token_REL,
    token_EQ,
    token_NEQ,
    token_LESS,
    token_MORE,
    token_LESS_EQ,
    token_MORE_EQ,
    token_OP_END,
    token_PARENTHESES_L,
    token_PARENTHESES_R,
    token_ID, // 20
    token_ID_OR_CONST,
    token_CONST,
    token_CONST_WHOLE_NUMBER,
    token_CONST_DEC_NUMBER,
    token_CONST_SCIENTIFIC_NOTATION,
    token_DOLLAR,
    // ...
    token_TERMINAL,  //23
    // ...
    token_NONTERMINAL, // remove later
    token_LET,
    token_VAR,
    token_IF,
    token_ELSE,
    token_WHILE,


    token_NIL,
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
    token_EMPTY,
} tokenType;

typedef struct token
{
    tokenType type;
    char* value;
} tokenStruct;

bool isTerminal(tokenType type);

// union for specific token data type
typedef union Value_of_token
{
    int INT_VAL;
    float FLOAT_VAL;
    char* STR_VAL;
} value_of_token;

// structure of token
typedef struct Token
{
    value_of_token value;
    tokenType type;
    int line;
} lex_token;

// structure of dynamic string
typedef struct Dynamic_String
{
    char* data;            // current cuntent
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