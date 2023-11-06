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