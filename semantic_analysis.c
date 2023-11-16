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
#include <string.h>

#define NOT_FALSE(expr) do { if (!expr) return INTERNAL_ERROR; } while (0)
#define NOT_NULL(expr) do { if (expr == NULL) return INTERNAL_ERROR; } while(0)

static String* functionId = NULL;
static char* callId = NULL;
static String* callParams;
static String* paramLabel;
static String* functionParam;
static int functionParamCount = 0;
static struct {
  bool let;
  char* idname;
  lex_token type;
  char* assignId;
  lex_token assignType;
  bool assignConst;
} assignment;

static FunctionStack* postponedCheckStack;

error_codes semanticAnalysisInit(void) {
  global_initSymtable();

  paramLabel = malloc(sizeof(String));
  NOT_NULL(paramLabel);

  functionId = malloc(sizeof(String));
  NOT_NULL(functionId);

  functionParam = malloc(sizeof(String));
  NOT_NULL(functionParam);

  NOT_FALSE(stringInit(paramLabel, ""));
  NOT_FALSE(stringInit(functionId, ""));
  NOT_FALSE(stringInit(functionParam, ""));

  postponedCheckStack = functionStackInit();
  NOT_NULL(postponedCheckStack);

  callParams = malloc(sizeof(String));
  NOT_NULL(callParams);
  NOT_FALSE(stringInit(callParams, ""));

  return SUCCESS;
}

void semanticAnalysisDeinit(void) {
  global_freeSymtable();
  stringFree(paramLabel);
  stringFree(functionId);
  stringFree(functionParam);
  free(paramLabel);
  free(functionId);
  free(functionParam);
  functionStackDeinit(postponedCheckStack);
  stringFree(callParams);
  free(callParams);
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

  NOT_FALSE(stringConcatChar(functionParam, ','));
  return SUCCESS;
}

error_codes analyseFunctionParamName(const char* name) {
  NOT_FALSE(stringConcatCStr(functionParam, name));
  NOT_FALSE(stringConcatChar(functionParam, ':'));
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

char* getLabelType(char* params, char* out_label, char* out_type) {
  // label
  int i = 0;
  while (*params != ':' && *params != '\0') {
    out_label[i++] = *params;
    params++;
  }
  out_label[i] = '\0';

  // check premature end
  if (*params == '\0' || *++params == '\0') {
    return NULL;
  }

  // type (single char)
  *out_type = *params;

  if (*params++ == ';') {
    // return pointer first char of next label
    return ++params;
  }

  if (*params == '\0') {
    // return end of string
    return params;
  }

  // bad format
  return NULL;
}

char* getLabelNameType(char* params, char* out_label, char* out_name, char* out_type) {
  // label
  int i = 0;
  while (*params != ',' && *params != '\0') {
    out_label[i++] = *params;
  }
  out_label[i] = '\0';

  // check premature end
  if (*params == '\0') {
    return NULL;
  }

  // ignore semarator
  params++;

  // name
  i = 0;
  while (*params != ':' && *params != '\0') {
    out_name[i++] = *params;
    params++;
  }
  out_name[i] = '\0';

  // check premature end
  if (*params == '\0' || *++params == '\0') {
    return NULL;
  }

  // type (single char)
  *out_type = *params;

  if (*params == ';') {
    // return pointer to first char of next label
    return ++params;
  }

  if (*params == '\0') {
    // return pointer to end of string
    return params;
  }

  // bad format
  return NULL;
}

bool compareParams(const char* callParams, const char* functionParams) {
  if (*functionParams++ != 'f') {
    return false;
  }

  if (*functionParams++ != ';') {
    return false;
  }

  // ignore return type
  callParams++;

  // copy constant strings to get strings that can be modified
  char* mutCallParams = malloc(strlen(callParams) + 1);
  char* mutFunctionParams = malloc(strlen(functionParams) + 1);

  if (mutCallParams == NULL || mutFunctionParams == NULL) {
    free(mutCallParams);
    free(mutFunctionParams);
    return false;
  }

  strcpy(mutCallParams, callParams);
  strcpy(mutFunctionParams, functionParams);

  char* callLabel = malloc(strlen(callParams));
  char callType;
  char* fnLabel = malloc(strlen(functionParams));
  char* fnName = malloc(strlen(functionParams));
  char fnType;
  bool result = true;

  if (callLabel == NULL || fnLabel == NULL || fnName == NULL) {
    free(callLabel);
    free(fnLabel);
    free(fnName);
    result = false;
    goto end;
  }

  // cp and fp will be modified, mutCallParams and mutFunctionParams must be kept for free
  char* cp = mutCallParams;
  char* fp = mutFunctionParams;

  while (*cp != '\0' && *fp != '\0') {
    cp = getLabelType(cp, callLabel, &callType);
    fp = getLabelNameType(fp, fnLabel, fnName, &fnType);

    // params were in wrong format
    if (cp == NULL || fp == NULL) {
      result = false;
      goto end;
    }

    if (strcmp(callLabel, fnLabel) != 0 || callType != fnType) {
      result = false;
      goto end;
    }
  }

end:
  free(mutCallParams);
  free(mutFunctionParams);
  free(callLabel);
  free(fnLabel);
  free(fnName);

  return result;
}

error_codes analyseCallConst(tokenType type) {
  const char* ts = typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcatCStr(callParams, "_:"));
  NOT_FALSE(stringConcatCStr(callParams, ts));
  NOT_FALSE(stringConcatChar(callParams, ';'));

  return SUCCESS;
}
error_codes analyseCallIdOrLabel(const char* value) {
  stringClear(paramLabel);
  NOT_FALSE(stringConcatCStr(paramLabel, value));
  return SUCCESS;
}

bool isTypeOfVariable(const char* typeStr) {
  return strcmp(typeStr, "I") == 0
    || strcmp(typeStr, "D") == 0
    || strcmp(typeStr, "S") == 0;
}

error_codes analyseCallEpsAfterId(void) {
  // paramLabel is the variable passed as param

  symtableItem* it = global_symbolSearch(stringCStr(paramLabel));
  NOT_NULL(it);

  NOT_FALSE(isTypeOfVariable(it->type));

  NOT_FALSE(stringConcatCStr(callParams, "_:"));
  NOT_FALSE(stringConcatCStr(callParams, it->type));
  NOT_FALSE(stringConcatChar(callParams, ';'));

  return SUCCESS;
}

error_codes analyseCallIdAfterLabel(const char* idname) {
  // paramLabel is label

  symtableItem* it = global_symbolSearch(idname);
  NOT_NULL(it);

  NOT_FALSE(isTypeOfVariable(it->type));

  NOT_FALSE(stringConcat(callParams, paramLabel));
  NOT_FALSE(stringConcatChar(callParams, ':'));
  NOT_FALSE(stringConcatCStr(callParams, it->type));

  return SUCCESS;
}
error_codes analyseCallConstAfterLabel(tokenType type) {
  // paramLabel is label

  const char* ts = typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcat(callParams, paramLabel));
  NOT_FALSE(stringConcatChar(callParams, ':'));
  NOT_FALSE(stringConcatCStr(callParams, ts));

  return SUCCESS;
}

error_codes analyseCallEnd(void) {
  symtableItem* item = global_symbolSearch(callId);

  // function was called before declaration
  if (item == NULL) {
    // postpone semantic check until function declaration
    NOT_FALSE(functionStackPush(postponedCheckStack, callId, stringCStr(callParams)));
    return SUCCESS;
  }

  NOT_FALSE(compareParams(stringCStr(callParams), item->type));
  return SUCCESS;
}

