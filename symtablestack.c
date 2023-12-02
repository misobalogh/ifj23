/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/


#include "symtablestack.h"
#include <malloc.h>
#include "global_variables.h"

static unsigned counter = 1;

void symtableStackInit(SymtableStack* stack) {
  stack->first = NULL;
}

bool symtableStackPush(SymtableStack* stack, bool flag) {
  SymtableStackItem* item = malloc(sizeof(SymtableStackItem));

  if (item == NULL) {
    return false;
  }

  item->next = stack->first;
  item->table = symtableInit(SYMTABLE_SIZE);
  item->flag = flag;
  if (item->flag) {
      item->id = -1;
  }
  else {
      item->id = counter++;
  }

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

symtableItem* symtableStackSearch(SymtableStack* stack, const char* key, unsigned* out_id) {
    SymtableStackItem* item = stack->first;

    while (item != NULL) {
        symtableItem* result = symtableSearch(item->table, key);
        if (result != NULL) {
            if (out_id != NULL) {
                *out_id = item->id;
            }
            return result;
        }

        item = item->next;
    }

    return NULL;
}

bool symtableStackIsLocal(SymtableStack* stack, const char* key) {
    SymtableStackItem* item = stack->first;
    bool found = false;

    while (item != NULL) {
        if (found) {
            if (item->flag) {
                return true;
            }
        }
        else {
            symtableItem* result = symtableSearch(item->table, key);
            if (result != NULL) {
                found = true;
                continue;
            }
        }

        item = item->next;
    }

    return false;

}
