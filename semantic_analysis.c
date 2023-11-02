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
  global_initSymtable();

  /* semStack = malloc(sizeof(SemStack)); */
  /* if (semStack == NULL) { */
  /*   return INTERNAL_ERROR; */
  /* } */

  /* semStackInit(semStack); */
  return SUCCESS;
}

error_codes analyseLet(const char* idname) {
  symtableItem* item = symtableSearch(global_table, idname);

  if (item != NULL) {
    return SEMANTIC_ERR;
  }

  /* symtableInsert(global_table, idname, ) */
}

error_codes analyseVar(const char* idname) {
}

error_codes analyseId(const char* idname) {
  symtableItem* item = symtableSearch(global_table, idname);

  if (item == NULL) {
    return SEMANTIC_ERR;
  }

  return SUCCESS;
}

error_codes analyseFunctionAddId(const char* idname) {
  symtableItem* item = symtableSearch(global_table, idname);

  if (item != NULL) {
    return SEMANTIC_ERR;
  }

  symtableInsert(global_table, idname, "f;", 0);
  return SUCCESS;
}

error_codes analyseFunctionAddParam(const char* fnIdname, const char* ida, const char* idb, const char* type) {
  symtableItem* fn = symtableSearch(global_table, fnIdname);

  if (fn == NULL) {
    return INTERNAL_ERROR;
  }

  String typeStr;
  if (!stringInit(&typeStr, fn->type)) {
    return INTERNAL_ERROR;
  }

  stringConcatCStr(&typeStr, ida);
  stringConcatChar(&typeStr, ';');
  stringConcatCStr(&typeStr, idb);
  stringConcatChar(&typeStr, ';');
  stringConcatChar(&typeStr, *type);
  stringConcatChar(&typeStr, ';');

  if (!isValidType(type)) {
    return SEMANTIC_ERR;
  }

  symtableInsert(global_table, fn->key, stringCStr(&typeStr), fn->data + 1);

  stringFree(&typeStr);
  return SUCCESS;
}

error_codes analyseFunctionAddReturn(const char* fnIdname, const char* type) {
  symtableItem* fn = symtableSearch(global_table, fnIdname);

  if (fn == NULL) {
    return INTERNAL_ERROR;
  }

  if (*type == '\0') {
    return SUCCESS;
  }

  String typeStr;
  if (!stringInit(&typeStr, fn->type)) {
    return INTERNAL_ERROR;
  }

  if (!isValidType(type)) {
    return SEMANTIC_ERR;
  }

  stringConcatChar(&typeStr, *type);
  stringConcatChar(&typeStr, ';');
  return SUCCESS;
}

bool isValidType(const char* typeStr) {
  return strcmp(typeStr, "String") == 0
    || strcmp(typeStr, "Int")
    || strcmp(typeStr, "Double");
}
