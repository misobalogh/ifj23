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

const char* TokenName(tokenType token) {
    switch (token) {
    case token_LEX_ERROR: return "LEX_ERROR";
    case token_OP_START: return "OP_START";
    case token_PLUS: return "PLUS";
    case token_MINUS: return "MINUS";
    case token_MUL: return "MUL";
    case token_DIV: return "DIV";
    case token_DEFAULT_VALUE: return "DEFAULT_VALUE";
    case token_FORCE_UNWRAP: return "FORCE_UNWRAP";
    case token_REL: return "REL";
    case token_EQ: return "EQ";
    case token_NEQ: return "NEQ";
    case token_LESS: return "LESS";
    case token_MORE: return "MORE";
    case token_LESS_EQ: return "LESS_EQ";
    case token_MORE_EQ: return "MORE_EQ";
    case token_OP_END: return "OP_END";
    case token_PARENTHESES_L: return "PARENTHESES_L";
    case token_PARENTHESES_R: return "PARENTHESES_R";
    case token_ID: return "ID";
    case token_ID_OR_CONST: return "ID_OR_CONST";
    case token_CONST: return "CONST";
    case token_CONST_WHOLE_NUMBER: return "CONST_WHOLE_NUMBER";
    case token_CONST_DEC_NUMBER: return "CONST_DEC_NUMBER";
    case token_CONST_SCIENTIFIC_NOTATION: return "CONST_SCIENTIFIC_NOTATION";
    case token_DOLLAR: return "DOLLAR";
    case token_TERMINAL: return "TERMINAL";
    case token_NONTERMINAL: return "NONTERMINAL";
    case token_LET: return "LET";
    case token_VAR: return "VAR";
    case token_IF: return "IF";
    case token_ELSE: return "ELSE";
    case token_WHILE: return "WHILE";
    case token_NIL: return "NIL";
    case token_FUNC: return "FUNC";
    case token_RETURN: return "RETURN";
    case token_BRACKET_L: return "BRACKET_L";
    case token_BRACKET_R: return "BRACKET_R";
    case token_COMMA: return "COMMA";
    case token_ARROW: return "ARROW";
    case token_ASSIGN: return "ASSIGN";
    case token_COLON: return "COLON";
    case token_SEMICOLON: return "SEMICOLON";
    case token_UNDERSCORE: return "UNDERSCORE";
    case token_TYPE: return "TYPE";
    case token_TYPE_INT: return "TYPE_INT";
    case token_TYPE_INT_Q: return "TYPE_INT_Q";
    case token_TYPE_DOUBLE: return "TYPE_DOUBLE";
    case token_TYPE_DOUBLE_Q: return "TYPE_DOUBLE_Q";
    case token_TYPE_STRING_LINE: return "TYPE_STRING_LINE";
    case token_TYPE_STRING: return "TYPE_STRING";
    case token_TYPE_STRING_Q: return "TYPE_STRING_Q";
    case token_EOL: return "EOL";
    case token_EOF: return "EOF";
    case token_EMPTY: return "EMPTY";
    default: return "Unknown token";
    }
}