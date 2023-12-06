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
  int id;
} SymtableStackItem;

// stack of symbol tables
typedef struct SymtableStack {
  SymtableStackItem* first;
} SymtableStack;

// initialize the stack of symbol tables
void symtableStackInit(SymtableStack* stack);

// add new empty symbol table to the top of stack of symbol tables
bool symtableStackPush(SymtableStack* stack, bool flag);

// remove the symbol table at the top of the stack
void symtableStackPop(SymtableStack* stack);

// search for symbol in any of the symbol tables in the stack, starting at top
symtableItem* symtableStackSearch(SymtableStack* stack, const char* key, unsigned* out_id);

// is symbol local to some function
bool symtableStackIsLocal(SymtableStack* stack, const char* key);

#endif
