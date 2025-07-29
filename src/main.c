/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: main.c
* Description: main program
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#include "error_codes.h"
#include "synt_analysis.h"

int main() {
    if (!recursiveParser()) {
        return SYNTAX_ANALYSIS_ERR;
    }
    return SUCCESS;
}
