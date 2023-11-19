/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#ifndef EXPR_STACK_H
#define EXPR_STACK_H

#include "expr.h"

typedef struct ExprStackItem {
  ExprItem item;
  struct ExprStackItem* next;
} ExprStackItem;

typedef struct ExprStack {
  struct ExprStackItem* first;
} ExprStack;

ExprStack* exprStackInit(void);
void exprStackPush(ExprStack* stack, ExprItem item);
ExprItem exprStackPop(ExprStack* stack);
ExprItem exprStackTop(ExprStack* stack);

#endif
