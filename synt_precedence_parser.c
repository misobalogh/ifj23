// Implementation of precedence parser for expressions

#include <stdio.h>
#include <stdlib.h>
#include "synt_analysis.h"
#include "synt_prec_stack.c"
#include "operator_precedence.h"
#include "token_types.h"


token *mock_nextToken() {
    static token tokens[] = {
        {token_ID, "a"},
        {token_PLUS, "+"},
        {token_ID, "b"},
        {token_MUL, "*"},
        {token_ID, "c"},
        {token_DOLLAR, "$"},
        {token_PARENTHESES_L, "("},
        {token_PARENTHESES_R, ")"},
        {token_EOF, "EOF"},
    };
    static int mock_i = 0;
    return &tokens[mock_i++];
}




bool precedenceParser() {
    token *t;
    while (t = mock_nextToken(), t->type != token_EOF) {
        switch (t->type)
        {
        case token_ID:
            break;
        case token_PLUS:
            break;
        case token_MUL:
            break;
        case token_PARENTHESES_L:
            break;
        case token_PARENTHESES_R:
            break;
        case token_DOLLAR:
            break;
        default:
            break;
        }
    }
    return false;
}


int main() {
    stack *stack = malloc(sizeof(stack));
    stackInit(stack);    

    stackPush(stack, token_ID);
    stackPush(stack, token_PLUS);
    stackPush(stack, token_MUL);


    

    LOG("%d", stackTopTerminal(stack)->type);
    LOG("%d", stackFirst(stack)->type);
    LOG("%d", stackSecond(stack)->type);
    LOG("%d", stackThird(stack)->type);

    stackFree(stack);
}