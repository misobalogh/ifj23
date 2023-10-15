
#include "synt_analysis.h"

token* mock_nextToken() {
    static token tokens[] = {
        // TEST 1
        {token_ID, "a"},
        {token_DOLLAR, "$"},
        // TEST 2
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_DOLLAR, "$"},
        // TEST 3
        {token_DOLLAR, "$"},
        // TEST 4
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_PARENTHESES_R, ")"},
        {token_DOLLAR, "$"},
        // TEST 5
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_PARENTHESES_R, ")"},
        {token_PLUS, "+"},
        {token_ID, "c"},
        {token_DOLLAR, "$"},
        // TEST 6
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_PARENTHESES_R, ")"},
        {token_PLUS, "+"},
        {token_PARENTHESES_L, "("},
        {token_ID, "c"},
        {token_PLUS, "+"},
        {token_ID, "d"},
        {token_PARENTHESES_R, ")"},
        {token_DOLLAR, "$"},
        // TEST 7 
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_DOLLAR, "$"},
        // ... 
        {token_EOF, "EOF"},
    };
    static int mock_i = 0;
    return &tokens[mock_i++];
}