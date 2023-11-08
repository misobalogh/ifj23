#include "stack.h"
#include <string.h>
#include <malloc.h>

FunctionStack* functionSackInit(void) {
  FunctionStack* stack = malloc(sizeof(FunctionStack));
  if (stack == NULL) {
    return NULL;
  }

  stack->first = malloc(sizeof(FunctionStackItem));
  if (stack->first == NULL) {
    free(stack);
    return NULL;
  }

  return stack;
}

bool functionStackPush(FunctionStack* stack, const char* name, const char* params) {
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

  stack->first->params = malloc(strlen(params));
  if (stack->first->params == NULL) {
    free(stack->first->name);
    free(stack->first);
    stack->first = first;
    return false;
  }

  strcpy(stack->first->name, name);
  strcpy(stack->first->params, params);
  stack->first->next = first;

  return true;
}

char* functionStackFindAndPop(FunctionStack* stack, const char* text) {
  FunctionStackItem* previous = NULL;
  FunctionStackItem* current = stack->first;

  while (current != NULL) {
    if (strcmp(current->name, text) == 0) {
      if (previous != NULL) {
        previous->next = current->next;
      }
      else {
        stack->first = current->next;
      }

      free(current->name);
      char* params = current->params;
      free(current);
      return params;
    }

    previous = current;
    current = current->next;
  }

  return NULL;
}
