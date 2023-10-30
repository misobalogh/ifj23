/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: mock_next_token.c
* Description: mocking function nextToken
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments: 

***************************************************************/



#include "synt_analysis.h"

tokenStruct* mock_precedence_nextToken() {
    static tokenStruct tokens[] = {
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


tokenStruct* mock_recursive_nextToken() {
    static tokenStruct tokensRec[] = {

        // EXPRESSION ASSIGNMENT
        {token_LET, "let"},
        {token_ID, "a"},
        {token_COLON, ":"},
        {token_TYPE, "Int"},
        {token_ASSIGN, "="},
        {token_CONST, "const"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},
        {token_ID, "a"},
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_PARENTHESES_R, ")"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},
        
        {token_PLUS, "+"},
        {token_CONST, "10"},
        {token_EOL, "EOL"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},

        // <statement> -> if <condition> { <stat_list> } else { <stat_list> }
        {token_IF, "if"},
        {token_PARENTHESES_L, "("},
        {token_ID, "b"},
        {token_EQ, "=="},
        {token_PARENTHESES_L, "("},
        {token_CONST, "10"},
        {token_NEQ, "!="},
        {token_CONST, "20"},
        {token_PARENTHESES_R, ")"},
        {token_PARENTHESES_R, ")"},
        {token_BRACKET_L, "{"},
        {token_EOL, "stat_list"},
        {token_BRACKET_R, "}"},
        {token_ELSE, "else"},
        {token_BRACKET_L, "{"},
        {token_EOL, "stat_list"},
        {token_BRACKET_R, "}"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},

        // <statement> -> while <condition> { <stat_list> }
        {token_WHILE, "while"},
        {token_PARENTHESES_L, "("},
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_PARENTHESES_R, ")"},
        {token_BRACKET_L, "{"},
        {token_EOL, "stat_list"},
        {token_BRACKET_R, "}"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},
        


        

        // FUNCTION DEFINITION
        {token_FUNC, "func"},
        {token_ID, "id"}, 
        {token_PARENTHESES_L, "("}, 
        {token_PARENTHESES_R, ")"},
        {token_ARROW, "->"},
        {token_TYPE, "Int"},
        {token_BRACKET_L, "{"},
        {token_EOL, "func_stat_list"},
        {token_BRACKET_R, "}"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},

        // VARIALBE DECLARATION
        {token_LET, "let"},
        {token_ID, "a"},
        {token_ASSIGN, "="},
        {token_ID, "10"},
        {token_EOL, "\n"},
        {token_EOF, "EOF"},      
                
        // VOID FUNCTION
        {token_FUNC, "func"},
        {token_ID, "id"}, 
        {token_PARENTHESES_L, "("}, 
        {token_EOL, "param_list"},
        {token_PARENTHESES_R, ")"},
        {token_BRACKET_L, "{"},
        {token_EOL, "func_stat_list"},
        {token_BRACKET_R, "}"},
        {token_EOL, "EOL"},
        {token_EOF, "EOF"},
    };
    static int mock_j = 0;
    return &tokensRec[mock_j++];
}