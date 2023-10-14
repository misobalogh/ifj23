// Implementation of precedence parser for expressions

#include <stdio.h>
#include <stdlib.h>
#include "synt_analysis.h"
#include "synt_prec_stack.h"
#include "operator_precedence.h"
#include "token_types.h"
#include "synt_prec_table.h"
#include "macros.h"
#include "rules.h"


token* mock_nextToken() {
    static token tokens[] = {
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_PARENTHESES_L, "("},
        {token_PARENTHESES_L, "("},
        {token_PARENTHESES_L, "("},
        {token_PARENTHESES_L, "("},
        {token_ID, "b"},
        {token_MUL, "*"},
        {token_PARENTHESES_L, "("},
        {token_ID, "c"},
        {token_PARENTHESES_R, ")"},
        {token_PARENTHESES_R, ")"},
        {token_PARENTHESES_R, ")"},
        {token_PARENTHESES_R, ")"},
        {token_PARENTHESES_R, ")"},
        {token_DOLLAR, "$"},
        {token_EOF, "EOF"},
    };
    static int mock_i = 0;
    return &tokens[mock_i++];
}



void reduce(stack* s) {
    stackItem* top = stackTopTerminal(s);
    stackItem* first = stackFirst(s);
    stackItem* second = stackSecond(s);
    stackItem* third = stackThird(s);


    if (top->type == token_ID && top->flag == true) {
        printf("rule 4\n");
        rule_ID(s);
    }
    else if (first->type == token_NONTERMINAL && second->type == token_PLUS && third->type == token_NONTERMINAL) {
        printf("rule 1\n");
        rule_ID_OP_ID(s);
    }
    else if (first->type == token_NONTERMINAL && second->type == token_MUL && third->type == token_NONTERMINAL) {
        printf("rule 2\n");
        rule_ID_OP_ID(s);
    }
    else if (first->type == token_PARENTHESES_R && second->type == token_NONTERMINAL && third->type == token_PARENTHESES_L) {
        printf("rule 3\n");
        rule_ID_OP_ID(s);
    }
    else {
        printf("ERROR: unknown rule\n");
        exit(SYNTAX_ANALYSIS_ERR);
    }
}

 
bool precedenceParser() {
    // prepare stack
    stack s;
    stackInit(&s);
    stackPush(&s, token_DOLLAR);

    token* t = mock_nextToken();
    while (!(t->type == token_DOLLAR && stackTopTerminal(&s)->type == token_DOLLAR)) {
        switch (precedenceTable[stackTopTerminal(&s)->type][t->type])
        {
        case EQUAL:
            printf("EQUAL\n");
            stackPush(&s, t->type);
            t = mock_nextToken();
            break;
        case LOW: // expand 
            stackPush(&s, t->type);
            stackTopTerminalSetFlag(&s);   
            t = mock_nextToken();
            printf("LOW\n");
            break;
        case HIGH: // reduce
            reduce(&s);
            printf("HIGH\n");
            stackPrint(&s);
            break;
        case EMPTY:
            stackFreeItems(&s);
            return false;
        // default:
        //     break;
        }
    }
    stackFreeItems(&s);
    return true;
}

