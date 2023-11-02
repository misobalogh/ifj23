#ifndef SYMTABLESTACK_H
#define SYMTABLESTACK_H

#include "symtable.h"

typedef struct symtableStackItem {
  symtable* table;
  struct symtableStackItem* next;
} symtableStackItem;

typedef struct symtableStack {
  symtableStackItem* first;
} symtableStack;

void symtableStackInit(symtableStack* stack);
bool symtableStackPush(symtableStack* stack);
void symtableStackPop(symtableStack* stack);
symtableItem* symtableStackSearch(symtableStack* stack, const char* key);

#endif
