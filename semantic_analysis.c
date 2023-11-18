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

#include "expr_stack.h"
#include "symtable.h"
#include "global_variables.h"
#include "dynamic_string.h"
#include "function_stack.h"
#include "expr.h"

#define NOT_FALSE(expr) do { if (!expr) exit(INTERNAL_ERROR); } while (0)
#define NOT_NULL(expr) do { if (expr == NULL) exit(INTERNAL_ERROR); } while(0)

static String* functionId = NULL;
static char* callId = NULL;
static String* callParams;
static String* paramLabel;
static String* functionParam;
static int functionParamCount = 0;
static struct {
  bool let;
  String idname;
  Type hintedType;
  Type type;
} assignment;
static ExprList* exprList;

static FunctionStack* postponedCheckStack;

bool semanticAnalysisInit(void) {
  global_initSymtable();
  global_initSymtableStack();

  exprList = exprListInit();

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
    || !stringInit(callParams, "")) {
      semanticAnalysisDeinit();
      return false;
  }

  symtableInsert(global_table, "readString", "f;S?;", 0);
  symtableInsert(global_table, "readInt", "f;I?;", 0);
  symtableInsert(global_table, "readDouble", "f;D?;", 0);
  symtableInsert(global_table, "write", "f;D?;*;", 1);
  symtableInsert(global_table, "Int2Double", "f;D;_,term:I;", 1);
  symtableInsert(global_table, "Double2Int", "f;I;_,term:D;", 1);
  symtableInsert(global_table, "length", "f;I;_,s:S;", 1);
  symtableInsert(global_table, "substring",
      "f;S?;of,s:S;startingAt,i:I,endingBefore,j:I", 1);
  symtableInsert(global_table, "ord", "f;I;_,c:S;", 1);
  symtableInsert(global_table, "chr", "f;S;_,i:I;", 1);

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


void analyseAssignBegin(void) {
  stringClear(&assignment.idname);
  assignment.let = false;
  assignment.hintedType.type = '\0';
  /* assignment.type = (Type) {}; */
}

void analyseAssignLet(bool let) {
  assignment.let = let;
}

void analyseAssignId(const char* idname) {
  NOT_FALSE(stringReinit(&assignment.idname, idname));
}

void analyseAssignHint(tokenType type) {
  assignment.hintedType = strToType(_typeShort(type));
}

void analyseAssignRightId(const char* idname) {
  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    exit(UNDEFINED_VAR);
  }

  assignment.type = strToType(it->type);
}

void analyseAssignType(Type type) {
  assignment.type = type;
}

void analyseAssignEnd(void) {
  if (assignment.hintedType.type != '\0'
    && assignment.hintedType.type != assignment.type.type) {
      // type hint is different than expression type
      exit(7);
  }

  String typeString = typeToStr(assignment.type);
  global_insertTop(stringCStr(&assignment.idname), stringCStr(&typeString), 0);
  stringFree(&typeString);
}

void analyseAssignEndNoVal(void) {
  String typeString = typeToStr(assignment.hintedType);
  global_insertTop(stringCStr(&assignment.idname), stringCStr(&typeString), 0);
  stringFree(&typeString);
}

// function definition

void analyseFunctionId(const char* idname) {
  NOT_FALSE(stringReinit(functionId, idname));
  symtableInsert(global_table, idname, "f;", 0);
  ;
}

void analyseFunctionParamLabel(const char* label) {
  stringClear(functionParam);

  if (label == NULL) {
    NOT_FALSE(stringConcatChar(functionParam, '_'));
  }
  else {
    NOT_FALSE(stringConcatCStr(functionParam, label));
  }

  NOT_FALSE(stringConcatChar(functionParam, ','));
  ;
}

void analyseFunctionParamName(const char* name) {
  NOT_FALSE(stringConcatCStr(functionParam, name));
  NOT_FALSE(stringConcatChar(functionParam, ':'));
  ;
}

void analyseFunctionParamType(tokenType type) {
  const char* ts = _typeShort(type);
  if (ts == NULL) {
    exit(SYNTAX_ANALYSIS_ERR);
  }

  NOT_FALSE(stringConcatCStr(functionParam, ts));
  functionParamCount++;
  ;
}

void analyseFunctionType(tokenType type) {
  const char* ts;

  if (type == token_EMPTY) {
    ts = "v";
  }
  else {
    ts = _typeShort(type);
    if (ts == NULL) {
      exit(SYNTAX_ANALYSIS_ERR);
    }
  }

  symtableItem* fn = symtableSearch(global_table, stringCStr(functionId));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));
  NOT_FALSE(stringConcatCStr(&typeString, ts));
  NOT_FALSE(stringConcatChar(&typeString, ';'));

  symtableInsert(global_table, stringCStr(functionId), stringCStr(&typeString), 0);
  ;
}


void analyseFunctionEnd(void) {
  symtableItem* fn = symtableSearch(global_table, stringCStr(functionId));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));

  NOT_FALSE(stringConcat(&typeString, functionParam));

  symtableInsert(global_table, stringCStr(functionId),
                 stringCStr(&typeString), functionParamCount);

  _checkPostponed(stringCStr(functionId), stringCStr(&typeString));

  stringFree(&typeString);
  functionParamCount = 0;
  stringClear(functionId);
  stringClear(functionParam);

  ;
}

// function call

void analyseCallConst(tokenType type) {
  const char* ts = _typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcatCStr(callParams, "_:"));
  NOT_FALSE(stringConcatCStr(callParams, ts));
  NOT_FALSE(stringConcatChar(callParams, ';'));

  ;
}
void analyseCallIdOrLabel(const char* value) {
  stringClear(paramLabel);
  NOT_FALSE(stringConcatCStr(paramLabel, value));
  ;
}

bool isTypeOfVariable(const char* typeStr) {
  return strcmp(typeStr, "I") == 0
    || strcmp(typeStr, "D") == 0
    || strcmp(typeStr, "S") == 0;
}

void analyseCallEpsAfterId(void) {
  // paramLabel is the variable passed as param

  symtableItem* it = global_symbolSearch(stringCStr(paramLabel));
  NOT_NULL(it);

  NOT_FALSE(isTypeOfVariable(it->type));

  NOT_FALSE(stringConcatCStr(callParams, "_:"));
  NOT_FALSE(stringConcatCStr(callParams, it->type));
  NOT_FALSE(stringConcatChar(callParams, ';'));

  ;
}

void analyseCallIdAfterLabel(const char* idname) {
  // paramLabel is label

  symtableItem* it = global_symbolSearch(idname);
  NOT_NULL(it);

  NOT_FALSE(isTypeOfVariable(it->type));

  NOT_FALSE(stringConcat(callParams, paramLabel));
  NOT_FALSE(stringConcatChar(callParams, ':'));
  NOT_FALSE(stringConcatCStr(callParams, it->type));

  ;
}
void analyseCallConstAfterLabel(tokenType type) {
  // paramLabel is label

  const char* ts = _typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcat(callParams, paramLabel));
  NOT_FALSE(stringConcatChar(callParams, ':'));
  NOT_FALSE(stringConcatCStr(callParams, ts));

  ;
}

void analyseCallEnd(void) {
  symtableItem* item = global_symbolSearch(callId);

  // function was called before declaration
  if (item == NULL) {
    // postpone semantic check until function declaration
    NOT_FALSE(functionStackPush(postponedCheckStack, callId, stringCStr(callParams)));
    ;
  }

  _compareParams(stringCStr(callParams), item->type);
  ;
}

// expression

void analyseExprBegin(void) {
  exprListClear(exprList);
}

void analyseExprOperand(lex_token token) {
  if (token.type == token_ID) {
    exprListAddId(exprList, token.value.STR_VAL);
  }
  else if (token.type == token_CONST_WHOLE_NUMBER) {
    exprListAddConst(exprList, (Type) { 'I', false });
  }
  else if (token.type == token_CONST_DEC_NUMBER
    || token.type == token_CONST_SCIENTIFIC_NOTATION) {
      exprListAddConst(exprList, (Type) { 'D', false });
  }
  else if (token.type == token_TYPE_STRING_LINE) {
    exprListAddConst(exprList, (Type) { 'S', false});
  }
  else {
    exit(INTERNAL_ERROR);
  }
}

void analyseExprOperator(lex_token token) {
  exprListAddOperator(exprList, tokenToOperator(token.type));
}

void analyseExprDefault(void) {
  exprListAddOperator(exprList, op_DEFAULT);
}

Type analyseExprEnd(void) {
  ExprStack* stack = exprStackInit();

  for (size_t i = 0; i < exprList->size; i++) {
    ExprItem it = exprList->data[i];

    if (it.type == expr_OPERATOR) {
      ExprItem a = exprStackPop(stack);
      ExprItem b = exprStackPop(stack);
      Type resultType = _analyseOperation(it.value.operatorType, a, b);
      exprStackPush(stack, (ExprItem) { .type=expr_CONST, .value={ .constType=resultType } });
    }
    else { // operand
      exprStackPush(stack, it);
    }
  }

  ExprItem top = exprStackPop(stack);
  if (top.type != expr_CONST) {
    exit(INTERNAL_ERROR);
  }
  return top.value.constType;
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

void _compareParams(const char* callParams, const char* functionParams) {
  if (*functionParams++ != 'f') {
    exit(INTERNAL_ERROR);
  }

  if (*functionParams++ != ';') {
    exit(INTERNAL_ERROR);
  }

  // ignore return type
  functionParams++;

  if (*functionParams++ != ';') {
    exit(INTERNAL_ERROR);
  }

  // copy constant strings to get mutable strings
  char* mutCallParams = malloc(strlen(callParams) + 1);
  char* mutFunctionParams = malloc(strlen(functionParams) + 1);

  if (mutCallParams == NULL || mutFunctionParams == NULL) {
    free(mutCallParams);
    free(mutFunctionParams);
    exit(INTERNAL_ERROR);
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

  if (result != SUCCESS) {
    exit(result);
  }
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

void _checkPostponed(const char* fnId, const char* fnType) {
  FunctionStackItem* fn = functionStackRemove(postponedCheckStack, fnId);

  // function was called with wrong arguments
  if (fn != NULL && strcmp(fn->params, fnType) != 0) {
    exit(UNDEFINED_FN);
  }

  // function was not called or it was called with correct arguments
  ;
}

Type strToType(const char* typeStr) {
  struct { const char* key; Type type; } map[] = {
    { "I", { 'I', false } },
    { "I?", { 'I', true } },
    { "D", { 'D', false } },
    { "D?", { 'D', true } },
    { "S", { 'S', false } },
    { "S?", { 'S', true } }
  };

  for (size_t i = 0; i < sizeof(map) / sizeof(map[0]); i++) {
    if (strcmp(typeStr, map[i].key) == 0) {
      return map[i].type;
    }
  }

  exit(INTERNAL_ERROR);
}

Type variableType(const char* idname) {
  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    exit(UNDEFINED_VAR);
  }

  return strToType(it->type);
}

Type _analyseOperation(OperatorType optype, ExprItem a, ExprItem b) {
  if (a.type == expr_OPERATOR || b.type == expr_OPERATOR) {
    exit(INTERNAL_ERROR);
  }

  Type typeA = (a.type == expr_CONST || a.type == expr_INTERMEDIATE)
    ? a.value.constType
    : variableType(a.value.idName);

  Type typeB = (b.type == expr_CONST || b.type == expr_INTERMEDIATE)
    ? b.value.constType
    : variableType(b.value.idName);

  if (optype == op_CONCAT) {
    if (typeA.type == 'S' && typeB.type == 'S') {
      return (Type) { 'S', (typeA.nullable || typeB.nullable) };
    }
  }

  if (optype == op_PLUS || optype == op_MINUS || optype == op_MUL || optype == op_DIV ) {
      if (typeA.type == typeB.type) {
        return (Type) { typeA.type, (typeA.nullable || typeB.nullable) };
      }

      if ((typeA.type == 'I' && typeB.type == 'D')
        || (typeA.type == 'D' && typeB.type == 'I')) {
          return (Type) { 'D', (typeA.nullable || typeB.nullable) };
      }
  }

  if (optype == op_DEFAULT) {
    if (typeA.type == typeB.type && !typeB.nullable) {
      return typeB;
    }
  }

  if (optype == op_EQ || optype == op_NEQ) {
    if (typeA.type == typeB.type) {
      return (Type) { 'B', false };
    }
  }

  if (optype == op_LESS || optype == op_MORE || optype == op_LESS_EQ || optype == op_MORE_EQ) {
    if (typeA.type == typeB.type && !typeA.nullable && !typeB.nullable) {
      return (Type) { 'B', false };
    }
  }

  exit(TYPE_COMPATIBILITY_ERR);
}

String typeToStr(Type type) {
  String result;
  NOT_FALSE(stringInit(&result, ""));

  if (type.type != 'I' && type.type != 'D' && type.type != 'S') {
    exit(INTERNAL_ERROR);
  }

  stringConcatChar(&result, type.type);

  if (type.nullable) {
    stringConcatChar(&result, '?');
  }

  return result;
}


