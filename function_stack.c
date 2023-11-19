#include "function_stack.h"
#include "symtable.h"
#include <string.h>
#include <malloc.h>

FunctionStack* functionStackInit(void) {
  FunctionStack* stack = malloc(sizeof(FunctionStack));
  if (stack == NULL) {
    return NULL;
  }

  stack->first = NULL;

  return stack;
}

void functionStackDeinit(FunctionStack* stack) {
  FunctionStackItem* current = stack->first;

  while (current != NULL) {
    FunctionStackItem* next = stack->first->next;

    free(current->name);
    free(current->params);

    current = next;
  }

  free(stack);
}

bool functionStackPush(FunctionStack* stack, const char* name, Param* params, unsigned count) {
  FunctionStackItem* first = stack->first;

  stack->first = malloc(sizeof(FunctionStackItem));
  if (stack->first == NULL) {
    return false;
  }

  stack->first->name = malloc(strlen(name));
  if (stack->first->name == NULL) {
    free(stack->first);
    stack->first = first;
    return false;
  }

  stack->first->params = malloc(count);
  if (stack->first->params == NULL) {
    free(stack->first->name);
    free(stack->first);
    stack->first = first;
    return false;
  }

  strcpy(stack->first->name, name);
  memcpy(stack->first->params, params, sizeof(Param) * count);
  stack->first->next = first;

  return true;
}

FunctionStackItem* functionStackRemove(FunctionStack* stack, const char* name) {
  FunctionStackItem* current = stack->first;
  FunctionStackItem* prev = stack->first;

  while (current != NULL) {
    if (strcmp(current->name, name)) {
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
