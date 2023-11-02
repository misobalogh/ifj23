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

  char temp[3];
  if (!parseType(type, temp)) {
    stringFree(&typeStr);
    return SEMANTIC_ERR;
  }

  stringConcatCStr(&typeStr, temp);
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

  char temp[3];
  if (!parseType(type, temp)) {
    stringFree(&typeStr);
    return SEMANTIC_ERR;
  }

  stringConcatCStr(&typeStr, temp);
  stringConcatChar(&typeStr, ';');

  symtableInsert(global_table, fnIdname, stringCStr(&typeStr), fn->data);
  stringFree(&typeStr);

  return SUCCESS;
}

bool parseType(const char* typeStr, char out[3]) {
  const char* map[][2] = {
    { "Int", "I" },
    { "Double", "D" },
    { "String", "S" },
    { "Int?", "I?" },
    { "Double?", "D?" },
    { "String?", "S?" }
  };

  for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
    if (strcmp(typeStr, map[i][0]) == 0) {
      strncpy(out, map[i][1], 3);
      return true;
    }
  }

  return false;
}
