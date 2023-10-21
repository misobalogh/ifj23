/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: rules.c
* Description: rules for syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments: 

***************************************************************/


#include "rules.h"
#include "synt_analysis.h"


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

















// const precedenceType precedenceTable[6][6] = {
// //+-----+-----+-----+-------+-------+-------+-------+
// //      |  +  |  *  |   (   |   )   |   id  |   $   |
// //+-----+-----+-----+-------+-------+-------+-------+
// /* + */ { HIGH, LOW,   LOW,    HIGH,   LOW,   HIGH  },
// /* * */ { HIGH, HIGH,  LOW,    HIGH,   LOW,   HIGH  },
// /* ( */ { LOW,  LOW,   LOW,    EQUAL,  LOW,   EMPTY },
// /* ) */ { HIGH, HIGH,  EMPTY,  HIGH,   EMPTY, HIGH  },
// /* id */{ HIGH, HIGH,  EMPTY,  HIGH,   EMPTY, HIGH  },
// /* $  */{ LOW,  LOW,   LOW,    EMPTY,  LOW,   EMPTY },
// //+-----+-----+-----+-------+-------+-------+------ -+
// };


const precedenceType precedenceTable[9][9] = {
//+-----+-----+------+------+------+-------+-------+------+-------+------+
//      |  +- |  */  |  ??  |   !  |  REL  |   (   |   )  |   i   |   $  |
//+-----+-----+------+------+------+-------+-------+------+-------+------+---+------
        {HIGH,  LOW,   HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   HIGH},// |  +-  |
        {HIGH,  HIGH,  HIGH,  LOW,   HIGH,   LOW,    HIGH,   LOW,   HIGH},// |  */  |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    HIGH,   LOW,   HIGH},// |  ??  |
        {HIGH,  HIGH,  HIGH,  EMPTY, HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  !   |
        {LOW,   LOW,   HIGH,  LOW,   EMPTY,  LOW,    HIGH,   LOW,   HIGH},// |  REL |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EQUAL,  LOW,   EMPTY},//|  (   |
        {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  )   |
        {HIGH,  HIGH,  HIGH,  HIGH,  HIGH,   EMPTY,  HIGH,   EMPTY, HIGH},// |  i   |
        {LOW,   LOW,   LOW,   LOW,   LOW,    LOW,    EMPTY,  LOW,   EMPTY}// |  $   |
};

/**
 * @brief Rule: E -> id
*/
void rule_ID(stack* s) {
    stackItem* top = stackTopTerminal(s);
    top->flag = false;
    top->type = token_NONTERMINAL;
}

/**
 * @brief Rule: E -> E arithmetic_operator E
 * Rule: E -> E + E
 * Rule: E -> E - E
 * Rule: E -> E * E
 * Rule: E -> E / E
*/
void rule_ID_OP_ID(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}

/**
 * @brief Rule: E -> (E)
*/
void rule_PAR_ID_PAR(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}

/**
 * @brief Rule: E -> E ?? E
 */
void rule_ID_CONCAT_ID(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}


/**
 * @brief Rule: E -> E!
 */
void rule_ID_FORCE_UNWRAP(stack* s) {
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}


void rule_ID_REL_ID(stack* s){
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPush(s, token_NONTERMINAL);
}