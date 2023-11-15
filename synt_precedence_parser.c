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

int getTableIndex(tokenType token) {
    switch (token)
    {
    case token_PLUS:
    case token_MINUS:
        return 0;
    case token_MUL:
    case token_DIV:
        return 1;
    case token_DEFAULT_VALUE:
        return 2;
    case token_FORCE_UNWRAP:
        return 3;
    case token_REL:
    case token_EQ:
    case token_NEQ:
    case token_LESS:
    case token_MORE:
    case token_LESS_EQ:
    case token_MORE_EQ:
        return 4;
    case token_PARENTHESES_L:
        return 5;
    case token_PARENTHESES_R:
        return 6;
    case token_ID:
    case token_CONST:
    case token_CONST_WHOLE_NUMBER:
    case token_CONST_DEC_NUMBER:
    case token_CONST_SCIENTIFIC_NOTATION:
    case token_TYPE_STRING_LINE:
        return 7;
    case token_DOLLAR:
    case token_EOL:
    case token_BRACKET_L:
    case token_BRACKET_R:
    case token_EOF:
        return 8;
    default:
        return -1;
    }
}



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


    if ((top->type == token_ID
        || top->type == token_CONST
        || top->type == token_CONST_WHOLE_NUMBER
        || top->type == token_CONST_DEC_NUMBER
        || top->type == token_CONST_SCIENTIFIC_NOTATION
        || top->type == token_TYPE_STRING_LINE) && top->flag == true) {
        PLOG("rule ID\n");
        rule_E_ID(s);
    }
    else if (
        first->type == token_NONTERMINAL &&
        (second->type == token_PLUS || second->type == token_MINUS || second->type == token_MUL || second->type == token_DIV) &&
        third->type == token_NONTERMINAL) {
        PLOG("rule E op E\n");
        rule_ID_OP_ID(s);
    }
    else if (
        first->type == token_NONTERMINAL &&
        second->type == token_DEFAULT_VALUE &&
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
        first->type == token_NONTERMINAL
        && (second->type == token_REL
            || second->type == token_EQ
            || second->type == token_NEQ
            || second->type == token_LESS
            || second->type == token_MORE
            || second->type == token_LESS_EQ
            || second->type == token_MORE_EQ)
        && third->type == token_NONTERMINAL) {
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
    PLOG("====INIT====");
    stackPrint(&s);
    lex_token temp = t;
    if (stash.type != token_EMPTY) {
        t = stash;
    }

    while (!((t.type == token_EOL || t.type == token_BRACKET_L || t.type == token_EOF || t.type == token_BRACKET_R) && stackTopTerminal(&s)->type == token_DOLLAR)) {

        int table_index1 = getTableIndex(stackTopTerminal(&s)->type);
        int table_index2 = getTableIndex(t.type);
        if (table_index1 == -1 || table_index2 == -1) {
            return false;
        }

        switch (precedenceTable[table_index1][table_index2])
        {
        case EQUAL: // "="
            PLOG("====EQUAL====");
            stackPrint(&s);
            stackPush(&s, t.type);
            if (stash.type != token_EMPTY) {
                t = temp;
                stash.type = token_EMPTY;
            }
            else {
                t = get_next_token();
            }
            break;
        case LOW: // expand  "<"
            stackPush(&s, t.type);
            stackTopTerminalSetFlag(&s);
            if (stash.type != token_EMPTY) {
                t = temp;
                stash.type = token_EMPTY;
            }
            else {
                t = get_next_token();
            }
            PLOG("====LOW====");
            stackPrint(&s);
            break;
        case HIGH: // reduce ">"
            PLOG("====HIGH====");
            stackPrint(&s);
            if (reduce(&s)) {
                break;
            }
            else {
                stackFreeItems(&s);
                return false;
            }
        case EMPTY: // error
            PLOG("====EMPTY====");
            stackPrint(&s);
            stackFreeItems(&s);
            return false;
        default:
            PLOG("ERROR: unknown precedence table value\n");
            return false;
        }
    }
    stackPrint(&s);
    PLOG("====END====\n");
    stackFreeItems(&s);
    return true;
}

