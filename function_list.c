/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "function_list.h"
#include "symtable.h"
#include <string.h>
#include <malloc.h>

FunctionLList* functionListInit(void) {
  FunctionLList* stack = malloc(sizeof(FunctionLList));
  if (stack == NULL) {
    return NULL;
  }

  stack->first = NULL;

  return stack;
}

void functionListDeinit(FunctionLList* stack) {
  FunctionLListItem* current = stack->first;

  while (current != NULL) {
    FunctionLListItem* next = stack->first->next;

    free(current->name);
    free(current->params);

    current = next;
  }

  free(stack);
}

bool functionListPush(FunctionLList* stack, const char* name, Param* params, unsigned count) {
    functionListRemove(stack, name);

  FunctionLListItem* first = stack->first;

  stack->first = malloc(sizeof(FunctionLListItem));
  if (stack->first == NULL) {
    return false;
  }

  stack->first->name = malloc(strlen(name) + 1);
  if (stack->first->name == NULL) {
    free(stack->first);
    stack->first = first;
    return false;
  }

  stack->first->params = malloc(sizeof(Param) * count);
  if (stack->first->params == NULL) {
    free(stack->first->name);
    free(stack->first);
    stack->first = first;
    return false;
  }

  strcpy(stack->first->name, name);
  memcpy(stack->first->params, params, sizeof(Param) * count);
  stack->first->paramCount = count;
  stack->first->next = first;

  return true;
}

FunctionLListItem* functionListRemove(FunctionLList* stack, const char* name) {
  FunctionLListItem* current = stack->first;
  FunctionLListItem* prev = NULL;

  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      if (prev == NULL) {
        stack->first = current->next;
      }
      else {
        prev->next = current->next;
      }

      return current;
    }

    current = current->next;
  }

  return NULL;
}
