/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: main.c
* Description: main program
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#include <stdio.h>
#include <stdbool.h>
#include "error_codes.h"
#include <stdlib.h>
#include "synt_analysis.h"

int main() {

    // lex_token t;
    // unsigned num_tokens = 20;
    // for (unsigned i = 0; i < num_tokens; i++) {

    //     t = get_next_token();
    //     LOG("Token: %s", TokenName(t.type));
    // }

    // return 0;

    /* lex_token t = get_next_token(); */
    /* while (t.type != token_EOF) { */
    /*   printf("%s %s", TokenName(t.type), t.type == token_ID ? t.value.STR_VAL : ""); */
    /*   t = get_next_token(); */
    /* } */
    /* return EXIT_SUCCESS; */

    if (!recursiveParser()) {
        // LOG("Syntax Error: code %d", SYNTAX_ANALYSIS_ERR);
        return SYNTAX_ANALYSIS_ERR;
    }
    // else {
    //     LOG("Program syntax is correct: code %d", EXIT_SUCCESS);
    // }
    return EXIT_SUCCESS;
}
