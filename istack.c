#include "istack.h"
#include "macros.h"
#include <stdlib.h>

IStack* istackInit(void) {
  IStack* result = malloc(sizeof(IStack));
  result->capacity = 10;
  result->data = malloc(result->capacity);
  CHECK_MEMORY_ALLOC(result->data);
  result->size = 0;
  return result;
}

unsigned istackPop(IStack* stack) {
  if (stack->size == 0) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  stack->size--;
  return stack->data[stack->size];
}

void istackPush(IStack* stack, unsigned value) {
  if (stack->size <= stack->capacity) {
    stack->capacity *= 2;
    stack->data = realloc(stack->data, stack->capacity);
    CHECK_MEMORY_ALLOC(stack->data);
  }

  stack->data[stack->size++] = value;
}

unsigned istackTop(IStack* stack) {
  if (stack->size == 0) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  return stack->data[stack->size - 1];
}

void istackFree(IStack* stack) {
  free(stack->data);
  stack->data = NULL;
  stack->size = 0;
  stack->capacity = 0;
  free(stack);
}
