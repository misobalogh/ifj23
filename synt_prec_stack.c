#include "synt_prec_stack.h"
#include "token_types.c"
#include <stdlib.h>
#include "macros.h"

void stackInit(stack* s) {
    s->size = 0;
    s->top = NULL;
    stackPush(s, token_DOLLAR);
}

void stackPush(stack* s, tokenType type) {
    stackItem* newItem = malloc(sizeof(stackItem));
    CHECK_MEMORY_ALLOC(newItem);
    newItem->type = type;
    newItem->lower = s->top;
    s->top = newItem;
    s->size++;
}

stackItem* stackPop(stack* s) {
    if (stackIsEmpty(s)) {
        return NULL;
    }
    stackItem* topItem = s->top;
    s->top = s->top->lower;
    s->size--;
    return topItem;
}

stackItem* stackTop(stack* s) {
    return s->top;
}

bool stackIsEmpty(stack* s) {
    return s->size == 0;
}

void stackFree(stack* s) {
    stackItem* item;
    while (!stackIsEmpty(s)) {
        item = stackPop(s);
        free(item);
    }
    free(s);
}

stackItem* stackTopTerminal(stack* s) {
    stackItem* item = s->top;
    while (!isTerminal(item->type)) {
        item = item->lower;
    }
    return item;
}

stackItem* stackFirst(stack* s) {
    return s->top;
}

stackItem* stackSecond(stack* s) {
    if (s->size < 2) {
        return NULL;
    }
    return s->top->lower;
}

stackItem* stackThird(stack* s) {
    if (s->size < 3) {
        return NULL;
    }
    return s->top->lower->lower;
}
