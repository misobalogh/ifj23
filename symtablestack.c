#include "symtablestack.h"
#include <malloc.h>
#include "global_variables.h"

void symtableStackInit(symtableStack* stack) {
  stack->first = NULL;
}

bool symtableStackPush(symtableStack* stack) {
  symtableStackItem* item = malloc(sizeof(symtableStackItem));

  if (item == NULL) {
    return false;
  }

  item->next = stack->first;
  item->table = symtableInit(SYMTABLE_SIZE);

  stack->first = item;

  return true;
}

void symtableStackPop(symtableStack* stack) {
  symtableStackItem* first = stack->first;

  if (first == NULL) {
    return;
  }

  stack->first = first->next;
  symtableFree(first->table);
  free(first);
}

symtableItem* symtableStackSearch(symtableStack* stack, const char* key) {
  symtableStackItem* item = stack->first;

  while (item != NULL) {
    symtableItem* result = symtableSearch(item->table, key);
    if (result != NULL) {
      return result;
    }

    item = item->next;
  }

  return NULL;
}

