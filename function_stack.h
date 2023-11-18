#ifndef FUNCTION_STACK_H
#define FUNCTION_STACK_H

#include <stdbool.h>

// stack je nejjednodušší ale hashovací tabulka by byla nejefektivnější, pokud bude čas přepsat

typedef struct FunctionStackItem {
  char* name;
  char* params;
  struct FunctionStackItem* next;
} FunctionStackItem;

typedef struct FunctionStack {
  FunctionStackItem* first;
} FunctionStack;

FunctionStack* functionStackInit(void);
FunctionStackItem* functionStackRemove(FunctionStack* stack, const char* fnName);
void functionStackDeinit(FunctionStack* stack);
char* functionStackFindAndPop(FunctionStack* stack, const char* name);
bool functionStackPush(FunctionStack* stack, const char* name, const char* params);

#endif
