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

#define NOT_FALSE(expr) do { if (!(expr)) EXIT_WITH_MESSAGE(INTERNAL_ERROR); } while (0)
#define NOT_NULL(expr) do { if ((expr) == NULL) EXIT_WITH_MESSAGE(INTERNAL_ERROR); } while(0)

static struct {
  String idname;
  String parameters;
  String idOrLabel;
} fnCall;

static struct {
  String param;
  int paramCount;
  String idname;
} fnDef;

static struct {
  bool started;
  bool let;
  String idname;
  Type hintedType;
  Type type;
} assignment;

static struct {
  bool started;
  String idname;
  Type type;
} reassignment;

static ExprList* exprList;

static FunctionStack* postponedCheckStack;

bool semanticAnalysisInit(void) {
  global_initSymtable();
  global_initSymtableStack();

  exprList = exprListInit();


  postponedCheckStack = functionStackInit();

  if (postponedCheckStack == NULL) {
    semanticAnalysisDeinit();
    return false;
  }

  if (!stringInit(&fnCall.parameters, "")
    || !stringInit(&fnDef.idname, "")
    || !stringInit(&fnDef.param, "")
    || !stringInit(&fnCall.idOrLabel, "")) {
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
  stringFree(&fnCall.parameters);
  stringFree(&fnDef.idname);
  stringFree(&fnDef.param);
  stringFree(&fnCall.idOrLabel);

  // call check was postponed but declaration never encountered
  if (postponedCheckStack->first != NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_FN);
  }

  functionStackDeinit(postponedCheckStack);
}

// variable declaration and assignment


void analyseAssignBegin(void) {
  assignment.started = true;
  assignment.let = false;
  stringClear(&assignment.idname);
  assignment.hintedType = (Type) { 'u', false };
  assignment.type = (Type) { 'u', false };
}

void analyseAssignLet(bool let) {
  assignment.let = let;
}

void analyseAssignId(const char* idname) {
  if (!assignment.started) {
    return;
  }

  NOT_FALSE(stringReinit(&assignment.idname, idname));
}

void analyseAssignHint(tokenType type) {
  if (!assignment.started) {
    return;
  }
  assignment.hintedType = strToType(_typeShort(type));
}

void analyseAssignRightId(const char* idname) {
  if (!assignment.started) {
    return;
  }

  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  assignment.type = strToType(it->type);
}

void analyseAssignType(Type type) {
  assignment.type = type;
}

void analyseAssignEndExpr(void) {
  if (!assignment.started) {
    return;
  }

  if (typeIsValue(assignment.hintedType)
    && typeEq(assignment.hintedType, assignment.type)) {
      // type hint is different than expression type
      EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  String typeString = typeToStr(assignment.type);
  global_insertTop(stringCStr(&assignment.idname), stringCStr(&typeString), 0);
  stringFree(&typeString);

  assignment.started = false;
}

void analyseAssignEndNodef(void) {
  if (!assignment.started) {
    return;
  }

  String typeString = typeToStr(assignment.hintedType);
  global_insertTop(stringCStr(&assignment.idname), stringCStr(&typeString), 0);
  stringFree(&typeString);

  assignment.started = false;
}

void analyseAssignEndCall(Type returnedType) {
  if (!assignment.started) {
    return;
  }

  if (typeIsValue(assignment.hintedType)
    && typeEq(assignment.hintedType, returnedType)) {
      EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  String typeString = typeToStr(returnedType);
  global_insertTop(stringCStr(&assignment.idname), stringCStr(&typeString), 0);
  stringFree(&typeString);

  assignment.started = false;
}

// function definition

void analyseFunctionId(const char* idname) {
  NOT_FALSE(stringReinit(&fnDef.idname, idname));
  symtableInsert(global_table, idname, "f;", 0);
  ;
}

void analyseFunctionParamLabel(const char* label) {
  stringClear(&fnDef.param);

  if (label == NULL) {
    NOT_FALSE(stringConcatChar(&fnDef.param, '_'));
  }
  else {
    NOT_FALSE(stringConcatCStr(&fnDef.param, label));
  }

  NOT_FALSE(stringConcatChar(&fnDef.param, ','));
  ;
}

void analyseFunctionParamName(const char* name) {
  NOT_FALSE(stringConcatCStr(&fnDef.param, name));
  NOT_FALSE(stringConcatChar(&fnDef.param, ':'));
  ;
}

void analyseFunctionParamType(tokenType type) {
  const char* ts = _typeShort(type);
  if (ts == NULL) {
    EXIT_WITH_MESSAGE(SYNTAX_ANALYSIS_ERR);
  }

  NOT_FALSE(stringConcatCStr(&fnDef.param, ts));
  fnDef.paramCount++;
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
      EXIT_WITH_MESSAGE(SYNTAX_ANALYSIS_ERR);
    }
  }

  symtableItem* fn = symtableSearch(global_table, stringCStr(&fnDef.idname));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));
  NOT_FALSE(stringConcatCStr(&typeString, ts));
  NOT_FALSE(stringConcatChar(&typeString, ';'));

  symtableInsert(global_table, stringCStr(&fnDef.idname), stringCStr(&typeString), 0);
  ;
}


void analyseFunctionEnd(void) {
  symtableItem* fn = symtableSearch(global_table, stringCStr(&fnDef.idname));
  NOT_NULL(fn);

  String typeString;
  NOT_FALSE(stringInit(&typeString, fn->type));

  NOT_FALSE(stringConcat(&typeString, &fnDef.param));

  symtableInsert(global_table, stringCStr(&fnDef.idname),
                 stringCStr(&typeString), fnDef.paramCount);

  _checkPostponed(stringCStr(&fnDef.idname), stringCStr(&typeString));

  stringFree(&typeString);
  fnDef.paramCount = 0;
  stringClear(&fnDef.idname);
  stringClear(&fnDef.param);
}

// function call

void analyseCallFnId(const char* idname) {
  NOT_FALSE(stringReinit(&fnCall.idname, idname));
}

void analyseCallConst(tokenType type) {
  const char* ts = _typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcatCStr(&fnCall.parameters, "_:"));
  NOT_FALSE(stringConcatCStr(&fnCall.parameters, ts));
  NOT_FALSE(stringConcatChar(&fnCall.parameters, ';'));
}

void analyseCallIdOrLabel(const char* value) {
  NOT_FALSE(stringReinit(&fnCall.idOrLabel, value));
}

void analyseCallEpsAfterId(void) {
  //idOrLabel was id

  symtableItem* it = global_symbolSearch(stringCStr(&fnCall.idOrLabel));

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  typeIsVariable(strToType(it->type));

  NOT_FALSE(stringConcatCStr(&fnCall.parameters, "_:"));
  NOT_FALSE(stringConcatCStr(&fnCall.parameters, it->type));
  NOT_FALSE(stringConcatChar(&fnCall.parameters, ';'));
}

void analyseCallIdAfterLabel(const char* idname) {
  // idOrLabel was label

  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  typeIsVariable(strToType(it->type));

  NOT_FALSE(stringConcat(&fnCall.parameters, &fnCall.idOrLabel));
  NOT_FALSE(stringConcatChar(&fnCall.parameters, ':'));
  NOT_FALSE(stringConcatCStr(&fnCall.parameters, it->type));

  ;
}
void analyseCallConstAfterLabel(tokenType type) {
  // idOrLabel was label

  const char* ts = _typeShort(type);
  NOT_NULL(ts);

  NOT_FALSE(stringConcat(&fnCall.parameters, &fnCall.idOrLabel));
  NOT_FALSE(stringConcatChar(&fnCall.parameters, ':'));
  NOT_FALSE(stringConcatCStr(&fnCall.parameters, ts));
}

Type analyseCallEnd(void) {
  symtableItem* item = global_symbolSearch(stringCStr(&fnCall.idname));

  // function was called before declaration
  if (item == NULL) {
    // postpone semantic check until function declaration
    NOT_FALSE(functionStackPush(postponedCheckStack, item->key,
              stringCStr(&fnCall.parameters)));
    return (Type) { 'u', false };
  }

  _compareParams(stringCStr(&fnCall.parameters), item->type);

  return strToType(item->type);
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
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
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
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }
  return top.value.constType;
}

// reassignment


void analyseReassignStart(void) {
  reassignment.started = true;
  stringClear(&reassignment.idname);
  reassignment.type = (Type) { 'u', false };
}

void analyseReassignId(const char* idname) {
  if (!reassignment.started) {
    return;
  }

  NOT_FALSE(stringReinit(&reassignment.idname, idname));
}

void analyseReassignRightId(const char* idname) {
  if (!reassignment.started) {
    return;
  }

  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_FN);
  }

  reassignment.type = strToType(it->type);
}

void analyseReassignEnd(void) {
  if (!reassignment.started) {
    return;
  }

  symtableItem* it = global_symbolSearch(stringCStr(&reassignment.idname));

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  Type leftType = strToType(it->type);

  if (!typeEq(leftType, reassignment.type)) {
    EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  reassignment.started = false;
}

void analyseReassignEndType(Type rightType) {
  if (!reassignment.started) {
    return;
  }

  symtableItem* it = global_symbolSearch(stringCStr(&reassignment.idname));

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  // fail if it is let

  Type leftType = strToType(it->type);

  if (!typeEq(leftType, rightType)) {
    EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  reassignment.started = false;
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
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  if (*functionParams++ != ';') {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  // ignore return type
  functionParams++;

  if (*functionParams++ != ';') {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  // copy constant strings to get mutable strings
  char* mutCallParams = malloc(strlen(callParams) + 1);
  char* mutFunctionParams = malloc(strlen(functionParams) + 1);

  if (mutCallParams == NULL || mutFunctionParams == NULL) {
    free(mutCallParams);
    free(mutFunctionParams);
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
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
    EXIT_WITH_MESSAGE(result);
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
    EXIT_WITH_MESSAGE(UNDEFINED_FN);
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

  EXIT_WITH_MESSAGE(INTERNAL_ERROR);
}

Type variableType(const char* idname) {
  symtableItem* it = global_symbolSearch(idname);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  return strToType(it->type);
}

Type _analyseOperation(OperatorType optype, ExprItem a, ExprItem b) {
  if (a.type == expr_OPERATOR || b.type == expr_OPERATOR) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  Type typeA = (a.type == expr_CONST || a.type == expr_INTERMEDIATE)
    ? a.value.constType
    : variableType(a.value.idName);

  Type typeB = (b.type == expr_CONST || b.type == expr_INTERMEDIATE)
    ? b.value.constType
    : variableType(b.value.idName);

  if (optype == op_CONCAT) {
    if (typeA.base == 'S' && typeB.base == 'S') {
      return (Type) { 'S', (typeA.nullable || typeB.nullable) };
    }
  }

  if (optype == op_PLUS || optype == op_MINUS || optype == op_MUL || optype == op_DIV ) {
      if (typeA.base == typeB.base) {
        return (Type) { typeA.base, (typeA.nullable || typeB.nullable) };
      }

      if ((typeA.base == 'I' && typeB.base == 'D')
        || (typeA.base == 'D' && typeB.base == 'I')) {
          return (Type) { 'D', (typeA.nullable || typeB.nullable) };
      }
  }

  if (optype == op_DEFAULT) {
    if (typeA.base == typeB.base && !typeB.nullable) {
      return typeB;
    }
  }

  if (optype == op_EQ || optype == op_NEQ) {
    if (typeA.base == typeB.base) {
      return (Type) { 'B', false };
    }
  }

  if (optype == op_LESS || optype == op_MORE || optype == op_LESS_EQ || optype == op_MORE_EQ) {
    if (typeA.base == typeB.base && !typeA.nullable && !typeB.nullable) {
      return (Type) { 'B', false };
    }
  }

  EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
}

String typeToStr(Type type) {
  String result;
  NOT_FALSE(stringInit(&result, ""));

  if (type.base != 'I' && type.base != 'D' && type.base != 'S') {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  stringConcatChar(&result, type.base);

  if (type.nullable) {
    stringConcatChar(&result, '?');
  }

  return result;
}


