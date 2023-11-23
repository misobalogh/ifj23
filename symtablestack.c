/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/


#include "symtablestack.h"
#include <malloc.h>
#include "global_variables.h"

void symtableStackInit(SymtableStack* stack) {
  stack->first = NULL;
}

bool symtableStackPush(SymtableStack* stack) {
  SymtableStackItem* item = malloc(sizeof(SymtableStackItem));

  if (item == NULL) {
    return false;
  }

  item->next = stack->first;
  item->table = symtableInit(SYMTABLE_SIZE);

  stack->first = item;

  return true;
}

void symtableStackPop(SymtableStack* stack) {
  SymtableStackItem* first = stack->first;

  if (first == NULL) {
    return;
  }

  stack->first = first->next;
  symtableFree(first->table);
  free(first);
}

symtableItem* symtableStackSearch(SymtableStack* stack, const char* key) {
  SymtableStackItem* item = stack->first;

  while (item != NULL) {
    symtableItem* result = symtableSearch(item->table, key);
    if (result != NULL) {
      return result;
    }

    item = item->next;
  }

  return NULL;
}

