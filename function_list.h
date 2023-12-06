/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#ifndef FUNCTION_LIST_H
#define FUNCTION_LIST_H

#include "symtable.h"
#include <stdbool.h>

/**
 * @brief Information about function call
 */
typedef struct FunctionLListItem {
  char* name; // name of function called
  Param* params; // parameters
  unsigned paramCount;
  struct FunctionLListItem* next;
} FunctionLListItem;

/**
 * @brief Linked list of information about a function call
 */
typedef struct FunctionLList {
  FunctionLListItem* first;
} FunctionLList;

/**
 * @brief initialize function list
 * @return pointer to initialized function list
 */
FunctionLList* functionListInit(void);

/**
 * @brief Remove value from list. Does nothing if the value is not in the list.
 * It is the callers responsibility to call free on the returned pointer.
 */
FunctionLListItem * functionListRemove(FunctionLList* stack, const char* fnName);

/**
 * @brief frees all memeory allocated during initialization and usage
 */
void functionListDeinit(FunctionLList* stack);

/**
 * @brief add item with provided values to the list
 */
bool functionListPush(FunctionLList* stack, const char* name, Param* params, unsigned count);

#endif
