#ifndef IFJ23_SYNT_PREC_STACK_H_
#define IFJ23_SYNT_PREC_STACK_H_

#include <stdbool.h>
#include "token_types.h"

/**
 * @brief Item of stack.
 * 
 * @param type Type of token.
 * @param flag Flag wheter "<" sign from precedence table is set.
 * @param lower Pointer to item below this item.
*/
typedef struct stackItem {
    tokenType type;
    bool flag; 
    struct stackItem *lower;
} stackItem;

typedef struct stack {
    stackItem* top;
    unsigned size;
} stack;

void stackInit(stack *s);

void stackPush(stack *s, tokenType type);

void stackPop(stack *s);

stackItem* stackTop(stack *s);

bool stackIsEmpty(stack *s);

void stackFreeItems(stack *s);

stackItem* stackTopTerminal(stack *s);

void stackTopTerminalSetFlag(stack* s);

stackItem* stackFirst(stack *s);

stackItem* stackSecond(stack *s);

stackItem* stackThird(stack *s);

void stackPrint(stack *s);  


#endif // IFJ23_SYNT_PREC_STACK_H_