#ifndef IFJ23_SYNT_PREC_STACK_H_
#define IFJ23_SYNT_PREC_STACK_H_

#include <stdbool.h>
#include "token_types.h"


typedef struct stackItem {
    tokenType type;
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

stackItem* stackFirst(stack *s);

stackItem* stackSecond(stack *s);

stackItem* stackThird(stack *s);


#endif // IFJ23_SYNT_PREC_STACK_H_