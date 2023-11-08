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
#include "function_stack.h"

SemStack* semStack;
stack* semanticStack;

static char* functionId = NULL;
static char* callId = NULL;
static String* callParams;
static String* paramLabel;

static FunctionStack* postponedCheckStack;

error_codes semanticAnalysisInit(void) {
  global_initSymtable();

  paramLabel = malloc(sizeof(String));
  if (paramLabel == NULL) {
    return INTERNAL_ERROR;
  }

  if (!stringInit(paramLabel, "")) {
    return INTERNAL_ERROR;
  }

  postponedCheckStack = functionStackInit();
  if (postponedCheckStack == NULL) {
    return INTERNAL_ERROR;
  }

  callParams = malloc(sizeof(String));
  if (callParams == NULL || !stringInit(callParams, "")) {
    return INTERNAL_ERROR;
  }

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
  functionId = malloc(strlen(idname));
  if (functionId == NULL) {
    return INTERNAL_ERROR;
  }

  strcpy(functionId, idname);

  symtableItem* item = symtableSearch(global_table, idname);

  if (item != NULL) {
    return SEMANTIC_ERR;
  }

  symtableInsert(global_table, idname, "f;", 0);
  return SUCCESS;
}

error_codes analyseFunctionAddParam(const char* label, const char* name, const char* type) {
  symtableItem* fn = symtableSearch(global_table, functionId);

  if (fn == NULL) {
    return INTERNAL_ERROR;
  }

  String typeStr;
  if (!stringInit(&typeStr, fn->type)) {
    return INTERNAL_ERROR;
  }

  stringConcatCStr(&typeStr, label);
  stringConcatChar(&typeStr, ';');
  stringConcatCStr(&typeStr, name);
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

error_codes analyseFunctionAddReturn(const char* type) {
  symtableItem* fn = symtableSearch(global_table, functionId);

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

  symtableInsert(global_table, functionId, stringCStr(&typeStr), fn->data);
  stringFree(&typeStr);

  return SUCCESS;
}

error_codes analyseFunctionEnd(void) {
  free(functionId);
  functionId = NULL;
  return SUCCESS;
}

error_codes analyseCallId(const char* idname) {
  callId = malloc(sizeof(strlen(idname)));
  if (callId == NULL) {
    return INTERNAL_ERROR;
  }

  strcpy(callId, idname);
  return SUCCESS;
}

error_codes analyseCallLabel(const char* label) {
  stringClear(paramLabel);
  if (!stringConcatCStr(paramLabel, label))
    return SUCCESS;
  return INTERNAL_ERROR;
}

static bool _callEnd(const char* param) {
  return stringConcat(callParams, paramLabel)
    && stringConcatChar(callParams, ':')
    && stringConcatCStr(callParams, param)
    && stringConcatChar(callParams, ';');
}

error_codes analyseCallParamConst(const char* data) {
  stringClear(paramLabel);
  if (_callEnd(data)) {
    return SUCCESS;
  }
  return INTERNAL_ERROR;
}

error_codes analyseCallParam(const char* paramIdname) {
  symtableItem* item = global_symbolSearch(paramIdname);
  if (item == NULL) {
    return SEMANTIC_ERR;
  }

  if (_callEnd(paramIdname)) {
    return SUCCESS;
  }
  return INTERNAL_ERROR;
}

error_codes analyseCallEpsilon(void) {
  if (!stringConcatCStr(callParams, "_:")
    || !stringConcat(callParams, paramLabel)) {
      return INTERNAL_ERROR;
  }
  return SUCCESS;
}

bool compareParams(const char* callParams, const char* functionParams) {
  return false;
}

error_codes analyseCallEnd(void) {
  symtableItem* item = global_symbolSearch(callId);

  // function was called before declaration
  if (item == NULL) {
    const char* params = stringCStr(callParams);
    if (!functionStackPush(postponedCheckStack, callId, params)) {
      return INTERNAL_ERROR;
    }

    return SUCCESS;
  }

  if (compareParams(stringCStr(callParams), item->type)) {
    return SUCCESS;
  }
  return SEMANTIC_ERR;
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
