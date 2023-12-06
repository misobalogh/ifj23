/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_recursive_parser.c
* Description: Recursive parser
* Author: MICHAL BALOGH, xbalog06, MICHAL CENEK xcenek04
* Faculty: FIT VUT
* Date: 14.10.2023

* Comments:

***************************************************************/

#include "generator.h"
#include "synt_analysis.h"
#include "synt_recur_rules.h"
#include "semantic_analysis.h"

bool recursiveParser(void) {
    if (!semanticAnalysisInit()) {
        return false;
    }

    genInit();

    // starting point of syntactic analysis
    if (!rule_PROGRAM()) {
        return false;
    }

    semanticAnalysisDeinit();
    genDeinit();

    return true;
}


