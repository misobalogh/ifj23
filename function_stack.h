#ifndef FUNCTION_STACK_H
#define FUNCTION_STACK_H

#include "symtable.h"
#include <stdbool.h>

// stack je nejjednodušší ale hashovací tabulka by byla nejefektivnější, pokud bude čas přepsat

typedef struct FunctionStackItem {
  char* name;
  Param* params;
  unsigned paramCount;
  struct FunctionStackItem* next;
} FunctionStackItem;

typedef struct FunctionStack {
  FunctionStackItem* first;
} FunctionStack;

FunctionStack* functionStackInit(void);
FunctionStackItem* functionStackRemove(FunctionStack* stack, const char* fnName);
void functionStackDeinit(FunctionStack* stack);
bool functionStackPush(FunctionStack* stack, const char* name, Param* params, unsigned count);

#endif
