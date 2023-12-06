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

/**
 * @ Stack for evaluating expression in postfix notation
 */
typedef struct ExprStack {
  struct ExprStackItem* first;
} ExprStack;

/**
 * @brief initialize stack
 */
ExprStack* exprStackInit(void);

/**
 * @brief push value to stack
 * @param stack stack
 * @param item value to push to stack
 */
void exprStackPush(ExprStack* stack, ExprItem item);

/**
 * @brief Remove top of stack and return its value
 */
ExprItem exprStackPop(ExprStack* stack);

/**
 * @brief get value on top of stack
 */
ExprItem exprStackTop(ExprStack* stack);

#endif
