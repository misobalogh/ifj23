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

  const char* typeShort = parseType(type);
  if (typeShort[0] == '\0') {
    stringFree(&typeStr);
    return SEMANTIC_ERR;
  }

  stringConcatCStr(&typeStr, typeShort);
  stringConcatChar(&typeStr, ';');

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

  const char* typeShort = parseType(type);
  if (typeShort[0] == '\0') {
    stringFree(&typeStr);
    return SEMANTIC_ERR;
  }

  stringConcatCStr(&typeStr, typeShort);
  stringConcatChar(&typeStr, ';');

  symtableInsert(global_table, fnIdname, stringCStr(&typeStr), fn->data);
  stringFree(&typeStr);

  return SUCCESS;
}

const char* parseType(const char* typeStr) {
  struct { const char* key; const char* value; } map[] = {
    { "Int", "I" },
    { "Double", "D" },
    { "String", "S" },
    { "Int?", "I?" },
    { "Double?", "D?" },
    { "String?", "S?" }
  };

  for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
    if (strcmp(typeStr, map[i].key) == 0) {
      return map[i].value;
    }
  }

  return "";
}
