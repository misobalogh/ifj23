/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/


#ifndef SYMTABLESTACK_H
#define SYMTABLESTACK_H

#include "symtable.h"

typedef struct SymtableStackItem {
  symtable* table;
  bool flag; // is this function frame
  struct SymtableStackItem* next;
} SymtableStackItem;

typedef struct SymtableStack {
  SymtableStackItem* first;
} SymtableStack;

void symtableStackInit(SymtableStack* stack);
bool symtableStackPush(SymtableStack* stack, bool flag);
void symtableStackPop(SymtableStack* stack);
symtableItem* symtableStackSearch(SymtableStack* stack, const char* key);
bool symtableStackIsLocal(SymtableStack* stack, const char* key);

#endif
