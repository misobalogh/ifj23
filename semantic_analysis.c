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

#define NOT_FALSE(expr) do { if (!expr) return INTERNAL_ERROR; } while (0)
#define NOT_NULL(expr) do { if (expr == NULL) return INTERNAL_ERROR; } while(0)

static String* functionId = NULL;
static char* callId = NULL;
static String* callParams;
static String* paramLabel;
static String* functionParam;
static int functionParamCount = 0;

static FunctionStack* postponedCheckStack;

error_codes semanticAnalysisInit(void) {
  global_initSymtable();

  paramLabel = malloc(sizeof(String));
  if (paramLabel == NULL) {
    return INTERNAL_ERROR;
  }

  functionId = malloc(sizeof(String));
  if (functionId == NULL) {
    return INTERNAL_ERROR;
  }

  functionParam = malloc(sizeof(String));
  if (functionParam == NULL) {
    return INTERNAL_ERROR;
  }

  if (!stringInit(paramLabel, "")) {
    return INTERNAL_ERROR;
  }

  if (!stringInit(functionId, "")) {
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

error_codes analyseFunctionId(const char* idname) {
  stringClear(functionId);
  stringConcatCStr(functionId, idname);

  symtableInsert(global_table, idname, "f;", 0);
  return SUCCESS;
}

error_codes analyseFunctionParamLabel(const char* label) {
  stringClear(functionParam);

  if (label == NULL) {
    NOT_FALSE(stringConcatChar(functionParam, '_'));
  }
  else {
    NOT_FALSE(stringConcatCStr(functionParam, label));
  }

  NOT_FALSE(stringConcatChar(functionParam, ';'));
  return SUCCESS;
}

error_codes analyseFunctionParamName(const char* name) {
  NOT_FALSE(stringConcatCStr(functionParam, name));
  NOT_FALSE(stringConcatChar(functionParam, ';'));
  return SUCCESS;
}

const char* typeShort(tokenType type) {
  switch (type) {
    case token_TYPE_STRING:
    case token_TYPE_STRING_Q:
      return "S";

    case token_TYPE_INT:
    case token_TYPE_INT_Q:
      return "I";

    case token_TYPE_DOUBLE:
    case token_TYPE_DOUBLE_Q:
      return "D";

    default:
      return NULL;
  }
}

error_codes analyseFunctionParamType(tokenType type) {
  const char* ts = typeShort(type);
  if (ts == NULL) {
    return SYNTAX_ANALYSIS_ERR;
  }

  NOT_FALSE(stringConcatCStr(functionParam, ts));
  functionParamCount++;
  return SUCCESS;
}

error_codes analyseFunctionType(tokenType type) {
  const char* ts;

  if (type == token_EMPTY) {
    ts = "v";
  }
  else {
    ts = typeShort(type);
    if (ts == NULL) {
      return SYNTAX_ANALYSIS_ERR;
    }
  }

  symtableItem* fn = symtableSearch(global_table, stringCStr(functionId));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));
  NOT_FALSE(stringConcatCStr(&typeString, ts));
  NOT_FALSE(stringConcatChar(&typeString, ';'));

  symtableInsert(global_table, stringCStr(functionId), stringCStr(&typeString), 0);
  return SUCCESS;
}

error_codes analyseFunctionEnd(void) {
  symtableItem* fn = symtableSearch(global_table, stringCStr(functionId));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));

  NOT_FALSE(stringConcat(&typeString, functionParam));

  symtableInsert(global_table, stringCStr(functionId),
                 stringCStr(&typeString), functionParamCount);

  stringFree(&typeString);
  functionParamCount = 0;
  stringClear(functionId);
  stringClear(functionParam);

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

