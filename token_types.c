/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: token_types.c
* Description: implementation of functions that work with token types
* Author: MICHAL BALOGH, xbalog06
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments: 

***************************************************************/

#include "token_types.h"
#include <stdbool.h>

bool isTerminal(tokenType type) {
    return type < token_NONTERMINAL;
}

// bool isNonterminal(tokenType type) {
//     return type > token_TERMINAL && type < token_NONTERMINAL;
// }

// ================ DEBUGGING ================ 
/**
 * @brief For debugging, delete later
*/
const char* tokenTypeNames[] = {
    "token_FORCE_UNWRAP", 
    "token_MUL",  
    "token_DIV", 
    "token_PLUS",
    "token_MINUS",
    "token_EQ",
    "token_NEQ",
    "token_LESS",
    "token_MORE",
    "token_LESS_EQ",
    "token_MORE_EQ",
    "token_CONCAT",
    "token_DOLLAR",
    "token_PARENTHESES_L",
    "token_PARENTHESES_R",
    "token_ID", 
    // ...
    "token_TERMINAL",
    // ...
    "token_NONTERMINAL",
    "token_EOF",
};
