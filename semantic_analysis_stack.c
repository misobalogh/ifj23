/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * File: semantic_analysis_stack.c
 * Authors: Michal Cenek xcenek04
 * Last modified by: Michal Cenek xcenek04
 * Last modification time: 2023-10-28T12:37:25.658484
*/

#include "semantic_analysis_stack.h"

#include <malloc.h>
#include <string.h>

void semStackInit(SemStack* stack) {
  stack->size = 0;
  stack->top = NULL;
}

bool semStackPush(SemStack* stack, SemStackValue value) {
  SemStackElement* elem = malloc(sizeof(SemStackElement));

  if (elem == NULL) {
    return false;
  }

  elem->value = value;
  elem->next = stack->top;

  return true;
}

bool semStackPushToken(SemStack* stack, struct token t) {
  SemStackValue stackValue = { SEM_STACK_TOKEN, .swift_type=0, .key=NULL };
  stackValue.key = malloc(sizeof(strlen(t.value) + 1));

  if (stackValue.key == NULL) {
    return false;
  }

  strcpy(stackValue.key, t.value);
  semStackPush(stack, stackValue);

  stackValue.token_type = t.type;

  return true;
}

void semStackInt(SemStack* stack, int32_t i) {
  SemStackValue value = { SEM_STACK_LITERAL, .swift_type=SWIFT_INT, .intValue=i };
  semStackPush(stack, value);
}

void semStackPushDouble(SemStack* stack, double f) {
  SemStackValue value = { SEM_STACK_LITERAL, .swift_type=SWIFT_DOUBLE, .floatValue=f };
  semStackPush(stack, value);
}

void semStackPop(SemStack* stack) {
  SemStackElement* top = stack->top;

  if (top == NULL) {
    return;
  }

  if (top->value.valueType == SEM_STACK_TOKEN) {
    free(top->value.key);
  }

  stack->top = top->next;
  stack->size--;

  free(top);
}

SemStackElement* semStackTop(SemStack* stack) {
  return stack->top;
}

SemStackElement* semStackPopTop(SemStack* stack) {
  SemStackElement* result = semStackTop(stack);
  semStackPop(stack);
  return result;
}

bool semStackIsEmpty(SemStack* stack) {
  return stack->size == 0;
}

void semStackFreeItems(SemStack* stack) {
  while (!semStackIsEmpty(stack)) {
    semStackPop(stack);
  }
}
