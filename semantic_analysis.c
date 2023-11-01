/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * File: semantic_analysis.c
 * Authors: Michal Cenek xcenek04
 * Last modified by: Michal Cenek xcenek04
 * Last modification time: 2023-10-29T13:40:10.153716
*/

#include "semantic_analysis.h"
#include <malloc.h>
#include "symtable.h"
#include "global_variables.h"
#include "dynamic_string.h"

SemStack* semStack;
stack* semanticStack;

error_codes semanticAnalysisInit(void) {
  global_table = symtableInit(SYMTABLE_SIZE);

  semStack = malloc(sizeof(SemStack));
  if (semStack == NULL) {
    return INTERNAL_ERROR;
  }

  semStackInit(semStack);
  return SUCCESS;
}

error_codes analyseFunction(const char* idname) {
  symtableItem* item = symtableSearch(global_table, idname);

  if (item != NULL) {
    // redefinition
    return SEMANTIC_ERR;
  }

  String typeString;
  stringInit(&typeString, "f;");

  int paramCount = 0;

  while (!semStackIsEmpty(semStack)) {
    SemStackElement* elem = semStackPopTop(semStack);

    if (elem->value.valueType != SEM_STACK_TOKEN) {
      return INTERNAL_ERROR;
    }

    paramCount++;

    stringConcatCStr(&typeString, elem->value.key);
    stringConcatChar(&typeString, ';');
  }

  symtableInsert(global_table, idname, stringCStr(&typeString), paramCount);

  stringFree(&typeString);

  return SUCCESS;
}

/* error_codes analyseAssignment(const char* leftkey, const char* rightkey) { */
/*   symtableItem* leftvar = symtableSearch(global_table, leftkey); */
/*   symtableItem* rightvar = symtableSearch(global_table, rightkey); */

/*   if (leftvar == NULL) { */
/*     if (rightvar == NULL) { */
/*       return TYPE_COMPATIBILITY_ERR; */
/*     } */

/*     symtableInsert(global_table, leftkey, rightvar->type, 0); */
/*     return SUCCESS; */
/*   } */

/*   return leftvar->type[0] == rightvar->type[0] */
/*     ? SUCCESS */
/*     : TYPE_COMPATIBILITY_ERR; */
/* } */
