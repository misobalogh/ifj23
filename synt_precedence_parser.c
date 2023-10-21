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
#include "synt_prec_rules.h"

/**
 * @brief Reduce items on stack with one of defined rules.
 * Called when ">" sign is in precedence table.
 * It gets top three items from stack and calls rule function, and then chooses,
 * which rule should be applied, according to types of items.
 * 
 * @param s Pointer to stack.
 * 
 * @return True if rule was applied, false otherwise if rule does not exist
*/
bool reduce(stack* s) {
    stackItem* top = stackTopTerminal(s);
    stackItem* first = stackFirst(s);
    stackItem* second = stackSecond(s);
    stackItem* third = stackThird(s);


    if (top->type == token_ID && top->flag == true) {
        PLOG("rule ID\n");
        rule_E_ID(s);
    }
    else if (
        first->type == token_NONTERMINAL &&
        (second->type == token_PLUS_MINUS || second->type == token_MUL_DIV) &&
        third->type == token_NONTERMINAL) {
        PLOG("rule E op E\n");
        rule_ID_OP_ID(s);
    }
    else if (
        first->type == token_NONTERMINAL &&
        second->type == token_CONCAT &&
        third->type == token_NONTERMINAL) {
        PLOG("rule E ?? E\n");
        rule_ID_CONCAT_ID(s);
    }
    else if (
        first->type == token_PARENTHESES_R && 
        second->type == token_NONTERMINAL && 
        third->type == token_PARENTHESES_L) {
        PLOG("rule (E)\n");
        rule_PAR_ID_PAR(s);
    }
    else if (
        first->type == token_FORCE_UNWRAP &&
        second->type == token_NONTERMINAL) {
        PLOG("rule E!\n");
        rule_ID_FORCE_UNWRAP(s);
    }
    else if (
        first->type == token_NONTERMINAL &&
        second->type == token_REL &&
        third->type == token_NONTERMINAL) {
        PLOG("rule ID rel ID\n");
        rule_ID_REL_ID(s);
    }
    else {
        PLOG("ERROR: unknown rule\n");
        return false;
    }

    return true;
}


bool precedenceParser() {
    // prepare stack
    stack s;
    stackInit(&s);
    stackPush(&s, token_DOLLAR);

    // debug
    PLOG("INIT: "); 
    stackPrint(&s);

    tokenStruct* t = mock_precedence_nextToken();
    while (!(t->type == token_DOLLAR && stackTopTerminal(&s)->type == token_DOLLAR)) {
        switch (precedenceTable[stackTopTerminal(&s)->type][t->type])
        {
        case EQUAL: // "="
            PLOG("EQUAL: ");
            stackPrint(&s);
            stackPush(&s, t->type);
            t = mock_precedence_nextToken();
            break;
        case LOW: // expand  "<"
            stackPush(&s, t->type);
            stackTopTerminalSetFlag(&s);
            t = mock_precedence_nextToken();
            PLOG("LOW: ");
            stackPrint(&s);
            break;
        case HIGH: // reduce ">"
            PLOG("HIGH: ");
            stackPrint(&s);
            if (reduce(&s)) {
                break;
            }
            else {
                stackFreeItems(&s);
                return false;
            }
        case EMPTY: // error
            PLOG("EMPTY: ");
            stackPrint(&s);
            stackFreeItems(&s);
            return false;
            // default:
            //     break;
        }
    }
    stackFreeItems(&s);
    return true;
}

