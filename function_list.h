
/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef FUNCTION_STACK_H
#define FUNCTION_STACK_H

#include "symtable.h"
#include <stdbool.h>

typedef struct FunctionLListItem {
  char* name;
  Param* params;
  unsigned paramCount;
  struct FunctionLListItem* next;
} FunctionLListItem;

typedef struct FunctionLList {
  FunctionLListItem* first;
} FunctionLList;

FunctionLList* functionStackInit(void);
FunctionLListItem * functionStackRemove(FunctionLList* stack, const char* fnName);
void functionStackDeinit(FunctionLList* stack);
bool functionStackPush(FunctionLList* stack, const char* name, Param* params, unsigned count);

#endif
