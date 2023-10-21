/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_recur_rules.c
* Description: rules for syntactic top-down syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 21.10.2023

* Comments: 

***************************************************************/

#include "synt_analysis.h"
#include "macros.h"
#include "synt_recur_rules.h"

token *t;

bool rule_PROGRAM() {
    t = mock_recursive_nextToken();
    RLOG("rule_PROGRAM\n");
    if (t->type == token_EOL || t->type == token_LET || t->type == token_VAR) {
        return rule_STAT_LIST() && rule_EOF();
    }
    return false;
}

bool rule_STAT_LIST() { 
    RLOG("rule_STAT_LIST\n");
    if (t->type == token_EOF) {
        return true;
    }
    else {
        return rule_STATEMENT() && rule_EOL() && rule_STAT_LIST();
    }
}

bool rule_STATEMENT() {
    RLOG("rule_STATEMENT\n");
    // rule 29
    if (t->type == token_LET || t->type == token_VAR) {
        return rule_VAR_DEFINITION() && rule_VAR_ASSIGNMENT();
    }
    return false;
}

bool rule_VAR_DEFINITION() {
    RLOG("rule_VAR_DEFINITION\n");
    if (t->type == token_LET) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    else if (t->type == token_VAR) {
        t = mock_recursive_nextToken();
        if (t->type == token_ID) {
            t = mock_recursive_nextToken();
            return true;
        }
    }
    return false;
}


bool rule_VAR_ASSIGNMENT() {
    RLOG("rule_VAR_ASSIGNMENT\n");
    if (t->type == token_ASSIGN) {
        return rule_ASSIGN() && rule_EXPRESSION();
    }
    // else if (t->type == token_COLON) {
    //     return rule_TYPE() && rule_VAL_ASSIGNMENT();
    // }
    return false;
}


bool rule_ASSIGN() {
    RLOG("rule_ASSIGN\n");
    bool retVal = t->type == token_ASSIGN;
    t = mock_recursive_nextToken();
    return retVal;
}

bool rule_EXPRESSION() {
    RLOG("rule_EXPRESSION\n");
    t = mock_recursive_nextToken();
    return true;
}


bool rule_EOL() {
    RLOG("rule_EOL\n");
    bool retVal = t->type == token_EOL;
    t = mock_recursive_nextToken();
    return retVal;
}

bool rule_EOF() {
    RLOG("rule_EOF\n");
    return t->type == token_EOF;
}

bool rec_rule_ID() {
    RLOG("rule_ID\n");
    bool retVal = t->type == token_ID;
    t = mock_recursive_nextToken();
    return retVal;
}


bool rule_VAR() {
    RLOG("rule_VAR\n");
    bool retVal = t->type == token_VAR;
    t = mock_recursive_nextToken();
    return retVal;
}

bool rule_LET() {
    RLOG("rule_LET\n");
    bool retVal = t->type == token_LET;
    t = mock_recursive_nextToken();
    return retVal;
}