#ifndef FUNCTION_STACK_H
#define FUNCTION_STACK_H

#include <stdbool.h>

typedef struct FunctionStackItem {
  char* name;
  char* params;
  struct FunctionStackItem* next;
} FunctionStackItem;

typedef struct FunctionStack {
  FunctionStackItem* first;
} FunctionStack;

FunctionStack* functionStackInit(void);
char* functionStackFindAndPop(FunctionStack* stack, const char* name);
bool functionStackPush(FunctionStack* stack, const char* name, const char* params);

#endif
