// Implementation of precedence parser for expressions

#include <stdio.h>
#include <stdlib.h>
#include "synt_analysis.h"
#include "synt_prec_stack.h"
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


// bool precedenceParser() {
//     token *t;
//     while (t = mock_nextToken(), t->type != token_EOF) {
//         switch (t->type)
//         {
//         case token_ID:
//             break;
//         case token_PLUS:
//             break;
//         case token_MUL:
//             break;
//         case token_PARENTHESES_L:
//             break;
//         case token_PARENTHESES_R:
//             break;
//         case token_DOLLAR:
//             break;
//         default:
//             break;
//         }
//     }
//     return false;
// }

// int main() {
//     stack *s = malloc(sizeof(stack));
//     CHECK_MEMORY_ALLOC(s);
//     stackInit(s);    

//     stackPush(s, token_ID);
//     stackPush(s, token_PLUS);
//     stackPush(s, token_MUL);
//     stackPush(s, token_PARENTHESES_L);
//     stackPush(s, token_PARENTHESES_R);

//     LOG("%s", TOKEN_TYPE_NAME(stackFirst(s)->type));
//     LOG("%s", TOKEN_TYPE_NAME(stackSecond(s)->type));
//     LOG("%s", TOKEN_TYPE_NAME(stackThird(s)->type));
//     LOG("%s", TOKEN_TYPE_NAME(stackTopTerminal(s)->type));


//     stackFreeItems(s);
//     free(s);
// }