/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: rules.h
* Description: header file for rules.c
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments:

***************************************************************/

#ifndef IFJ23_RULES_H
#define IFJ23_RULES_H

#include "synt_prec_table.h"
#include "synt_prec_stack.h"

bool rule_PROGRAM();
bool rule_STAT_LIST();
bool rule_STATEMENT();
bool rule_EOF();
bool rule_EOL();
bool rec_rule_ID();
bool rule_VAR_DEFINITION();
bool rule_VAR_ASSIGNMENT();
bool rule_LET();
bool rule_VAR();
bool rule_EXPRESSION();
bool rule_ASSIGN();



/* EXPRESSIONS */
void rule_ID(stack* s);

void rule_ID_OP_ID(stack* s);

void rule_PAR_ID_PAR(stack* s);

void rule_ID_CONCAT_ID(stack* s);

void rule_ID_FORCE_UNWRAP(stack* s);

void rule_ID_REL_ID(stack* s);


#endif //IFJ23_RULES_H
