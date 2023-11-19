/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#include "expr_stack.h"

#include <malloc.h>
#include <stdlib.h>
#include "error_codes.h"
#include "macros.h"
#include <string.h>


ExprStack* exprStackInit(void) {
  ExprStack* result = malloc(sizeof(ExprStack));
  CHECK_MEMORY_ALLOC(result);
  result->first = NULL;
  return result;
}

void exprStackPush(ExprStack* stack, ExprItem item) {
  ExprStackItem* it = malloc(sizeof(ExprStackItem));
  it->item = item;

  if (it->item.type == expr_ID) {
    it->item.value.idName = malloc(strlen(item.value.idName) + 1);
    CHECK_MEMORY_ALLOC(it->item.value.idName);
    strcpy(it->item.value.idName, item.value.idName);
  }

  it->next = stack->first;
  stack->first = it;
}

ExprItem exprStackPop(ExprStack* stack) {
  if (stack->first == NULL) {
    exit(INTERNAL_ERROR);
  }

  ExprStackItem* first = stack->first;
  stack->first = first->next;
  return first->item;
}

ExprItem exprStackTop(ExprStack* stack) {
  return stack->first->item;
}

