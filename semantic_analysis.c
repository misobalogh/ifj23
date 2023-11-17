/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * File: semantic_analysis.c
 * Authors: Michal Cenek xcenek04
 * Last modified by: Michal Cenek xcenek04
 * Last modification time: 2023-10-29T13:40:10.153716
*/

#include "semantic_analysis.h"

#include <malloc.h>
#include <string.h>

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
static struct {
  bool let;
  String idLeft;
  tokenType hintedType;
  String idRight;
  tokenType typeRight;
  bool isRightConst;
} assignment;

static FunctionStack* postponedCheckStack;

bool semanticAnalysisInit(void) {
  global_initSymtable();
  global_initSymtableStack();

  paramLabel = NULL;
  functionId = NULL;
  functionParam = NULL;
  callParams = NULL;

  paramLabel = malloc(sizeof(String));
  functionId = malloc(sizeof(String));
  functionParam = malloc(sizeof(String));
  callParams = malloc(sizeof(String));

  postponedCheckStack = functionStackInit();

  if (paramLabel == NULL || functionId == NULL ||
    functionParam == NULL || callParams == NULL
    || postponedCheckStack == NULL) {
      semanticAnalysisDeinit();
      return false;
  }

  if (!stringInit(paramLabel, "")
    || !stringInit(functionId, "")
    || !stringInit(functionParam, "")
    || !stringInit(&assignment.idRight, "")
    || !stringInit(&assignment.idLeft, "")
    || !stringInit(callParams, "")) {
      semanticAnalysisDeinit();
      return false;
  }

  return true;
}

void semanticAnalysisDeinit(void) {
  global_freeSymtable();
  global_freeSymtableStack();
  stringFree(paramLabel);
  stringFree(functionId);
  stringFree(functionParam);
  stringFree(callParams);
  free(paramLabel);
  free(functionId);
  free(functionParam);
  free(callParams);
  functionStackDeinit(postponedCheckStack);
}

// variable declaration and assignment

error_codes analyseLetId(const char* idname) {
  NOT_FALSE(stringReinit(&assignment.idLeft, idname));
  assignment.let = true;
  return SUCCESS;
}

error_codes analyseVarId(const char* idname) {
  NOT_FALSE(stringReinit(&assignment.idLeft, idname));
  assignment.let = false;
  return SUCCESS;
}

error_codes analyseTypeHint(tokenType type) {
  assignment.hintedType = type;
  return SUCCESS;
}

error_codes analyseAssignConst(tokenType type) {
  assignment.typeRight = type;
  assignment.isRightConst = true;
  return SUCCESS;
}

error_codes analyseAssignId(const char* idname) {
  symtableItem* right = global_symbolSearch(idname);

  if (right == NULL) {
    return UNDEFINED_VAR;
  }

  const char* ts = _typeShort(assignment.hintedType);
  if (ts != NULL) {
    if (strcmp(ts, right->type) != 0) {
      return TYPE_COMPATIBILITY_ERR;
    }
  }

  global_insertTop(stringCStr(&assignment.idLeft), right->type, 0);

  return SUCCESS;
}

// function definition

error_codes analyseFunctionId(const char* idname) {
  NOT_FALSE(stringReinit(functionId, idname));
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

error_codes analyseFunctionParamType(tokenType type) {
  const char* ts = _typeShort(type);
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
    ts = _typeShort(type);
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

  error_codes ret = _checkPostponed(stringCStr(functionId), stringCStr(&typeString));
  if (ret != SUCCESS) {
    return ret;
  }

  stringFree(&typeString);
  functionParamCount = 0;
  stringClear(functionId);
  stringClear(functionParam);

  return SUCCESS;
}

// function call

error_codes analyseCallConst(tokenType type) {
  const char* ts = _typeShort(type);
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

  const char* ts = _typeShort(type);
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

  NOT_FALSE(_compareParams(stringCStr(callParams), item->type));
  return SUCCESS;
}

// helper functions

char* _getLabelType(char* params, char* out_label, char* out_type) {
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

  params++;

  if (*params == ';') {
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

char* _getLabelNameType(char* params, char* out_label, char* out_name, char* out_type) {
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

error_codes _compareParams(const char* callParams, const char* functionParams) {
  if (*functionParams++ != 'f') {
    return INTERNAL_ERROR;
  }

  if (*functionParams++ != ';') {
    return INTERNAL_ERROR;
  }

  // ignore return type
  functionParams++;

  if (*functionParams++ != ';') {
    return INTERNAL_ERROR;
  }

  // copy constant strings to get mutable strings
  char* mutCallParams = malloc(strlen(callParams) + 1);
  char* mutFunctionParams = malloc(strlen(functionParams) + 1);

  if (mutCallParams == NULL || mutFunctionParams == NULL) {
    free(mutCallParams);
    free(mutFunctionParams);
    return INTERNAL_ERROR;
  }

  strcpy(mutCallParams, callParams);
  strcpy(mutFunctionParams, functionParams);

  char* callLabel = malloc(strlen(callParams));
  char* fnLabel = malloc(strlen(functionParams));
  char* fnName = malloc(strlen(functionParams));
  char callType;
  char fnType;
  error_codes result = SUCCESS;

  if (callLabel == NULL || fnLabel == NULL || fnName == NULL) {
    result = INTERNAL_ERROR;
    goto end;
  }

  // cp and fp will be modified, mutCallParams and mutFunctionParams must be kept for free
  char* cp = mutCallParams;
  char* fp = mutFunctionParams;

  while (*cp != '\0' && *fp != '\0') {
    cp = _getLabelType(cp, callLabel, &callType);
    fp = _getLabelNameType(fp, fnLabel, fnName, &fnType);

    // params were in wrong format
    if (cp == NULL || fp == NULL) {
      result = INTERNAL_ERROR;
      goto end;
    }

    if (strcmp(callLabel, fnLabel) != 0 || callType != fnType) {
      result = SEMANTIC_ERR;
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

const char* _typeShort(tokenType type) {
  switch (type) {
    case token_TYPE_STRING:
    case token_TYPE_STRING_LINE:
      return "S";

    case token_TYPE_STRING_Q:
      return "S?";

    case token_CONST_WHOLE_NUMBER:
    case token_TYPE_INT:
      return "I";

    case token_TYPE_INT_Q:
      return "I?";

    case token_CONST_SCIENTIFIC_NOTATION:
    case token_TYPE_DOUBLE:
      return "D";

    case token_TYPE_DOUBLE_Q:
      return "D?";

    default:
      return NULL;
  }
}

error_codes _checkPostponed(const char* fnId, const char* fnType) {
  FunctionStackItem* current = postponedCheckStack->first;
  while (current != NULL) {
    if (strcmp(current->name, fnId) == 0) {
      return _compareParams(current->params, fnType) != SUCCESS;
      // TODO pop current
    }
  }

  return SUCCESS;
}
