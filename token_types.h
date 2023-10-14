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
    token_PLUS,
    token_MUL,  
    token_PARENTHESES_L,
    token_PARENTHESES_R,
    token_ID, 
    token_DOLLAR,
    token_FORCE_UNWRAP, 
    token_DIV, 
    token_MINUS,
    token_EQ,
    token_NEQ,
    token_LESS,
    token_MORE,
    token_LESS_EQ,
    token_MORE_EQ,
    token_CONCAT,
    // ...
    token_TERMINAL,
    // ...

    token_NONTERMINAL,
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