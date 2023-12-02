/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_precedence_parser.c
* Description: Precedence syntactic analysis (parser for expression)
* Author: MICHAL BALOGH, xbalog06
* Faculty: FIT VUT
* Date: 14.10.2023

* Comments:

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "synt_analysis.h"
#include "synt_prec_stack.h"
#include "synt_prec_table.h"
#include "operator_precedence.h"
#include "token_types.h"
#include "macros.h"
#include "synt_recur_rules.h"
#include "semantic_analysis.h"

bool recursiveParser(void) {
  if (!semanticAnalysisInit()) {
    return false;
  }

  bool result = rule_PROGRAM();
  semanticAnalysisDeinit();
  return result;
}


