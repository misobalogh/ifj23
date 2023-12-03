/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/

#include "semantic_analysis.h"

#include <malloc.h>
#include <string.h>

#include "expr_stack.h"
#include "generator.h"
#include "symtable.h"
#include "global_variables.h"
#include "dynamic_string.h"
#include "function_list.h"
#include "expr.h"

#define NOT_FALSE(expr) do { if (!(expr)) EXIT_WITH_MESSAGE(INTERNAL_ERROR); } while (0)
#define NOT_NULL(expr) do { if ((expr) == NULL) EXIT_WITH_MESSAGE(INTERNAL_ERROR); } while(0)

static struct {
  String idname;
  bool started;
} iflet;

static struct {
  String idname;
  String idOrLabel;
  Param* params;
  unsigned paramCount;
  unsigned paramCapacity;
} fnCall;

static struct {
  Param* params;
  unsigned paramCount;
  unsigned paramCapacity;
  String idname;
  Type type;
} fnDef;

static struct {
  bool started;
  bool let;
  String idname;
  String rightId;
  Type hintedType;
  Type type;
} assignment;

static struct {
  bool started;
  String idname;
  String rightId;
  Type type;
} reassignment;

static bool returnStarted;
static bool exprConstOnly;

static ExprArray* exprList;
static FunctionLList* postponedCheckStack;

static Type lastExprType;

static Param* int2DoubleParam, * double2IntParam, * lengthParam, * substringParams, 
  * ordParam, * chrParam;

static String currentFunction;

void prepareStatement(void) {
  iflet.started = false;
  stringClear(&iflet.idname);

  stringClear(&fnCall.idname);
  stringClear(&fnCall.idOrLabel);
  fnCall.paramCount = 0;

  stringClear(&fnDef.idname);
  fnDef.paramCount = 0;
  fnDef.type = (Type) { 'u', false };

  assignment.started = false;
  stringClear(&assignment.idname);
  stringClear(&assignment.rightId);
  assignment.hintedType = (Type) { 0, false };
  assignment.type = (Type) { 0, false };

  reassignment.started = false;
  stringClear(&reassignment.idname);
  stringClear(&reassignment.rightId);
  reassignment.type = (Type) { 'u', false };

  lastExprType = (Type) { 'u', false };

  returnStarted = false;
}

static void fnCallGrow(void) {
  if (fnCall.paramCount + 1 >= fnCall.paramCapacity) {
    fnCall.paramCapacity *= 2;
    fnCall.params = realloc(fnCall.params, sizeof(Param) * fnCall.paramCapacity);
    CHECK_MEMORY_ALLOC(fnCall.params);

    for (unsigned i = fnCall.paramCount + 1; i < fnCall.paramCapacity; i++) {
      (stringInit(&fnCall.params[i].label, ""));
      (stringInit(&fnCall.params[i].name, ""));
    }
  }
}

static void fnDefGrow(void) {
  if (fnDef.paramCount + 1 >= fnDef.paramCapacity) {
    fnDef.paramCapacity *= 2;
    fnDef.params = realloc(fnDef.params, sizeof(Param) * fnDef.paramCapacity);
    CHECK_MEMORY_ALLOC(fnDef.params);

    for (unsigned i = fnDef.paramCount + 1; i < fnDef.paramCapacity; i++) {
      (stringInit(&fnDef.params[i].label, ""));
      (stringInit(&fnDef.params[i].name, ""));
    }
  }
}


bool semanticAnalysisInit(void) {
  global_initSymtable();
  global_initSymtableStack();

  lastExprType =  (Type) { 'u', false };

  exprList = exprListInit();

  stringInit(&currentFunction, "");

  fnDef.paramCapacity = 10;
  fnDef.params = malloc(sizeof(Param) * fnDef.paramCapacity);
  CHECK_MEMORY_ALLOC(fnDef.params);
  fnDef.paramCount = 0;

  for (unsigned i = 0; i < fnDef.paramCapacity; i++) {
    stringInit(&fnDef.params[i].label, "");
    stringInit(&fnDef.params[i].name, "");
  }

  fnCall.paramCapacity = 10;
  fnCall.params = malloc(sizeof(Param) * fnDef.paramCapacity);
  CHECK_MEMORY_ALLOC(fnDef.params);
  fnCall.paramCount = 0;

  reassignment.started = false;
  assignment.started = false;

  for (unsigned i = 0; i < fnCall.paramCapacity; i++) {
    stringInit(&fnCall.params[i].label, "");
    stringInit(&fnCall.params[i].name, "");
  }

  postponedCheckStack = functionStackInit();

  if (postponedCheckStack == NULL) {
    semanticAnalysisDeinit();
    return false;
  }

  iflet.started = false;

   stringInit(&fnDef.idname, "");
   stringInit(&fnCall.idOrLabel, "");
   stringInit(&fnCall.idname, "");
   stringInit(&iflet.idname, "");
   stringInit(&reassignment.rightId, "");
   stringInit(&assignment.rightId, "");

  symtableInsert(global_table, "readString", (SymbolData) {
      (Type) { 'S', true },
      NULL, 0,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  symtableInsert(global_table, "readInt", (SymbolData) {
      (Type) { 'I', true },
      NULL, 0,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  symtableInsert(global_table, "readDouble", (SymbolData) {
      (Type) { 'D', true },
      NULL, 0,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  symtableInsert(global_table, "write", (SymbolData) {
      (Type) { 'D', true },
      NULL, 0,
      symbol_flag_BUILTIN | symbol_flag_VARIADIC,
      symbol_FN
  });

  int2DoubleParam = malloc(sizeof(Param));
  CHECK_MEMORY_ALLOC(int2DoubleParam);
  stringInit(&int2DoubleParam->label, "_");
  stringInit(&int2DoubleParam->name, "term");
  int2DoubleParam->type = (Type) { 'I', false };

  symtableInsert(global_table, "Int2Double", (SymbolData) {
      (Type) { 'D', false },
      int2DoubleParam, 1,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  double2IntParam = malloc(sizeof(Param));
  CHECK_MEMORY_ALLOC(double2IntParam);
  stringInit(&double2IntParam->label, "_");
  stringInit(&double2IntParam->name, "term");
  double2IntParam->type = (Type) { 'D', false };

  symtableInsert(global_table, "Double2Int", (SymbolData) {
      (Type) { 'I', false },
      double2IntParam, 1,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  lengthParam = malloc(sizeof(Param));
  CHECK_MEMORY_ALLOC(lengthParam);
  stringInit(&lengthParam->label, "_");
  stringInit(&lengthParam->name, "s");
  lengthParam->type = (Type) { 'S', false };

  symtableInsert(global_table, "length", (SymbolData) {
      (Type) { 'I', false },
      lengthParam, 1,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  substringParams = malloc(sizeof(Param) * 3);
  CHECK_MEMORY_ALLOC(substringParams);
  stringInit(&substringParams[0].label, "of");
  stringInit(&substringParams[0].name, "s");
  substringParams[0].type = (Type) { 'S', false };
  stringInit(&substringParams[1].label, "startingAt");
  stringInit(&substringParams[1].name, "i");
  substringParams[1].type = (Type) { 'I', false };
  stringInit(&substringParams[2].label, "endingBefore");
  stringInit(&substringParams[2].name, "j");
  substringParams[2].type = (Type) { 'I', false };

  symtableInsert(global_table, "substring", (SymbolData) {
      (Type) { 'S', true },
      substringParams, 3,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  ordParam = malloc(sizeof(Param));
  CHECK_MEMORY_ALLOC(ordParam);
  stringInit(&ordParam->label, "_");
  stringInit(&ordParam->name, "c");
  ordParam->type = (Type) { 'S', false };

  symtableInsert(global_table, "ord", (SymbolData) {
      (Type) { 'I', false },
      ordParam, 1,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  chrParam = malloc(sizeof(Param));
  CHECK_MEMORY_ALLOC(chrParam);
  stringInit(&chrParam->label, "_");
  stringInit(&chrParam->name, "i");
  chrParam->type = (Type) { 'I', false };

  symtableInsert(global_table, "chr", (SymbolData) {
      (Type) { 'S', false },
      chrParam, 1,
      symbol_flag_BUILTIN,
      symbol_FN
  });

  return true;
}

void semanticAnalysisDeinit(void) {
  global_freeSymtable();
  global_freeSymtableStack();
  stringFree(&fnDef.idname);
  stringFree(&fnCall.idOrLabel);

  free(fnDef.params);
  fnDef.params = NULL;

  free(fnCall.params);
  fnCall.params = NULL;

  free(int2DoubleParam);
  free(double2IntParam);
  free(lengthParam);
  free(substringParams);
  free(ordParam);
  free(chrParam);

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
  stringClear(&assignment.rightId);
  assignment.hintedType = (Type) { 0, false };
  assignment.type = (Type) { 0, false };
}

void analyseAssignLet(bool let) {
  assignment.let = let;
}

void analyseAssignId(const char* idname) {
  stringSet(&assignment.idname, idname);
}

void analyseAssignHint(tokenType type) {
  assignment.hintedType = tokenToType(type);
}

void analyseAssignRightId(const char* idname) {
  stringSet(&assignment.rightId, idname);
}

void analyseAssignIdType(void) {
  if (!assignment.started) {
    return;
  }

  symtableItem* it = global_searchTop(stringCStr(&assignment.rightId));

  if (it == NULL
    || (it->data.symbolType != symbol_VAR && it->data.symbolType != symbol_LET)) {
      EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  genExprOperand((ExprItem) {
      .type=expr_ID,
      .value.idName=(char*) stringCStr(&assignment.rightId)
  });

  assignment.type = it->data.dataType;
}

void analyseAssignType(Type type) {
  assignment.type = type;
}

void analyseAssignEnd(void) {
  if (!assignment.started) {
    return;
  }

  symtableItem* it = global_searchTop(stringCStr(&assignment.idname));
  if (it != NULL) {
    // multiple definitions in single scope not allowed
    EXIT_WITH_MESSAGE(3);
  }

  SymbolData data;
  SymbolType st = assignment.let ? symbol_LET : symbol_VAR;
  bool assign = false;

  // tyep was hinted and expression is omitted
  // let a: T
  if (typeIsValue(assignment.hintedType) && assignment.type.base == 0) {
    if (assignment.hintedType.nullable) {
        data = (SymbolData) { assignment.hintedType, NULL, 0, symbol_flag_INITIALIZED, st };
    }
    else {
        data = (SymbolData) { assignment.hintedType, NULL, 0, 0U, st };
    }

  }
  // type was hinted and actual type could not be inffered
  // let a: T = <undefined-expr>
  else if(typeIsValue(assignment.hintedType) && assignment.type.base == 'u') {
      EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }
  // type was hinted and actual type matches or
  // let a: T = <T-expr>
  // type was not hinted and actual type is valid type
  // let a = <expr>
  else if ((typeIsValue(assignment.hintedType) && typeEq(assignment.hintedType, assignment.type))
      || (!typeIsValue(assignment.hintedType) && typeIsValue(assignment.type))) {
    data = (SymbolData) { assignment.type, NULL, 0, symbol_flag_INITIALIZED, st };
    assign = true;
  }
  // hinted type is Double and actual type is Int and assigned value is const
  // let a: Double = <Int-const>
  else if(assignment.hintedType.base == 'D' && assignment.type.base == 'I'
          && assignment.rightId.size == 0 && exprConstOnly) {
      data = (SymbolData) { assignment.hintedType, NULL, 0, symbol_flag_INITIALIZED, st };
      printf("INT2FLOATS\n");
      assign = true;
  }
  else {
    EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  global_insertTop(stringCStr(&assignment.idname), data);

  genDef(stringCStr(&assignment.idname));
  if (assign) {
      genAssign(stringCStr(&assignment.idname));
  }

  prepareStatement();
}

// function definition

void analyseFunctionId(const char* idname) {
  fnDef.paramCount = 0;
  stringSet(&fnDef.idname, idname);
}

void analyseFunctionParamLabel(const char* label) {
  fnDefGrow();
  stringSet(&fnDef.params[fnDef.paramCount].label, label);
}

void analyseFunctionParamName(const char* name) {
  stringSet(&fnDef.params[fnDef.paramCount].name, name);
}

void analyseFunctionParamType(tokenType type) {
  fnDef.params[fnDef.paramCount].type = tokenToType(type);
  fnDef.paramCount++;
}

void analyseFunctionType(tokenType type) {
  if (type == token_EMPTY) {
    fnDef.type = (Type) { 'v', false };
  }
  else {
    fnDef.type = tokenToType(type);
  }
}

void analyseReturnBegin(void) {
  returnStarted = true;
}

void analyseReturn(Type type) {
  if (!returnStarted) {
    return;
  }

  symtableItem* it = symtableSearch(global_table, stringCStr(&currentFunction));
  NOT_NULL(it);

  if ((it->data.dataType.base == 'v' && type.base != 'v')
      || (it->data.dataType.base != 'v' && type.base == 'v')) {
          EXIT_WITH_MESSAGE(6);
  }

  if (type.base != it->data.dataType.base
    || (!it->data.dataType.nullable && type.nullable)) {
      EXIT_WITH_MESSAGE(4);
  }

  genReturn();
  returnStarted = false;
}

void analyseFunctionEnd(void) {
  symtableItem* fn = symtableSearch(global_table, stringCStr(&fnDef.idname));

  if (fn != NULL) {
    EXIT_WITH_MESSAGE(3);
  }

  NOT_FALSE(typeIsValid(fnDef.type));

  SymbolData data = { fnDef.type, fnDef.params, fnDef.paramCount, 0u, symbol_FN };
  symtableInsert(global_table, stringCStr(&fnDef.idname), data);

  _checkPostponed(stringCStr(&fnDef.idname), data);

  genFunction(stringCStr(&fnDef.idname));


  fnDef.paramCount = 0;
  stringClear(&fnDef.idname);
  fnDef.type = (Type) { 0, false };
}

// function call

void analyseCallFnId(const char* idname) {
  stringClear(&fnCall.idname);
  stringClear(&fnCall.idOrLabel);
  stringClear(&fnCall.idOrLabel);
  fnCall.paramCount = 0;
  (stringSet(&fnCall.idname, idname));

  for (unsigned i = 0; i < fnCall.paramCount; i++) {
    stringSet(&fnCall.params[fnCall.paramCount].name, "");
    stringSet(&fnCall.params[fnCall.paramCount].label, "");
    fnCall.params[fnCall.paramCount].type = (Type) { 0, false };
    fnCall.params[fnCall.paramCount].isConst = false;
  }
}

void analyseCallConst(lex_token token) {
  fnCallGrow();
  Type t = tokenToType(token.type);
  fnCall.params[fnCall.paramCount].type = t;
  fnCall.params[fnCall.paramCount].isConst = true;

  stringSet(&fnCall.params[fnCall.paramCount].label, "_");

  if (t.base == 'I') {
    fnCall.params[fnCall.paramCount].intVal = token.value.INT_VAL;
  }
  else if (t.base == 'D') {
    fnCall.params[fnCall.paramCount].floatVal = token.value.FLOAT_VAL;
  }
  else if (t.base == 'S') {
    stringSet(&fnCall.params[fnCall.paramCount].name, token.value.STR_VAL);
  }

  fnCall.paramCount++;
}

void analyseCallIdOrLabel(const char* value) {
  fnCallGrow();
  stringSet(&fnCall.idOrLabel, value);
}

void analyseCallEpsAfterId(void) {
  //idOrLabel was id

  symtableItem* it = global_symbolSearch(stringCStr(&fnCall.idOrLabel), NULL);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  typeIsVariable(it->data.dataType);
  stringSet(&fnCall.params[fnCall.paramCount].label, "_");
  stringSet(&fnCall.params[fnCall.paramCount].name, it->key);
  fnCall.params[fnCall.paramCount].type = it->data.dataType;
  fnCall.params[fnCall.paramCount].isConst = false;

  fnCall.paramCount++;
}

void analyseCallIdAfterLabel(const char* idname) {
  // idOrLabel was label

  symtableItem* it = global_symbolSearch(idname, NULL);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  NOT_FALSE(typeIsVariable(it->data.dataType));

  stringSetS(&fnCall.params[fnCall.paramCount].label, &fnCall.idOrLabel);
  fnCall.params[fnCall.paramCount].type = it->data.dataType;
  stringSet(&fnCall.params[fnCall.paramCount].name, idname);
  fnCall.params[fnCall.paramCount].isConst = false;

  fnCall.paramCount++;
}

void analyseCallConstAfterLabel(lex_token token) {
  // idOrLabel was label

  Param* param = &fnCall.params[fnCall.paramCount];
  stringSetS(&param->label, &fnCall.idOrLabel);
  param->type = tokenToType(token.type);
  fnCall.params[fnCall.paramCount].isConst = true;

  if (param->type.base == 'I') {
    fnCall.params[fnCall.paramCount].intVal = token.value.INT_VAL;
  }
  else if (param->type.base == 'D') {
    fnCall.params[fnCall.paramCount].floatVal = token.value.FLOAT_VAL;
  }
  else if (param->type.base == 'S') {
    stringSet(&fnCall.params[fnCall.paramCount].name, token.value.STR_VAL);
  }

  fnCall.paramCount++;
}

Type analyseCallEnd(void) {
  symtableItem* item = global_symbolSearch(stringCStr(&fnCall.idname), NULL);

  // function was called before declaration
  if (item == NULL) {
    // postpone semantic check until function declaration
    NOT_FALSE(functionStackPush(postponedCheckStack, stringCStr(&fnCall.idname),
              fnCall.params, fnCall.paramCount));
    return (Type) { 'u', false };
  }

  if (item->data.symbolType != symbol_FN) {
    EXIT_WITH_MESSAGE(UNDEFINED_FN);
  }

  if (!(item->data.flags & symbol_flag_VARIADIC) && !_compareParams(item->data.params, item->data.paramCount,
    fnCall.params, fnCall.paramCount)) {
      EXIT_WITH_MESSAGE(4);
  }

  genCall(item->key, fnCall.params, fnCall.paramCount);

  return item->data.dataType;
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
    exprListAddInt(exprList, token.value.INT_VAL);
  }
  else if (token.type == token_CONST_DEC_NUMBER
    || token.type == token_CONST_SCIENTIFIC_NOTATION) {
      exprListAddFloat(exprList, token.value.FLOAT_VAL);
  }
  else if (token.type == token_TYPE_STRING_LINE) {
    exprListAddString(exprList, token.value.STR_VAL);
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
  exprConstOnly = true;

  for (size_t i = 0; i < exprList->size; i++) {
    ExprItem it = exprList->data[i];

    if (it.type == expr_OPERATOR) {
      Type resultType;

      if (it.value.operatorType == op_UNWRAP) {
        ExprItem a = exprStackPop(stack);
        resultType = _analyseUnwrap(a);
        if (a.type == expr_ID) {
            genExprOperand(a);
        }
      }
      else {
        ExprItem a = exprStackPop(stack);
        ExprItem b = exprStackPop(stack);
        resultType = _analyseOperation(it.value.operatorType, &b, &a);

        OperatorType optype = it.value.operatorType;
        if (resultType.base == 'S') {
          optype = op_CONCAT;
        }

        genExprOperand(b);
        genExprOperand(a);
        genExprOperator(optype);
      }
      exprStackPush(stack, (ExprItem) {
          .type=expr_INTERMEDIATE,
          .value={ .constValue.type=resultType }
        });
    }
    else { // operand
      exprStackPush(stack, it);
      if (it.type != expr_CONST) {
          exprConstOnly = false;
      }
    }
  }

  ExprItem top = exprStackPop(stack);

  if (stack->first != NULL) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  genExprOperand(top);

  if (top.type == expr_ID) {
    lastExprType = variableType(top.value.idName);
    return lastExprType;
  }

  if (top.type == expr_INTERMEDIATE || top.type == expr_CONST) {
    lastExprType = top.value.constValue.type;
    return lastExprType;
  }

  EXIT_WITH_MESSAGE(INTERNAL_ERROR);
}

// reassignment


void analyseReassignStart(void) {
  reassignment.started = true;
  stringClear(&reassignment.idname);
  reassignment.type = (Type) { 'u', false };
}

void analyseReassignId(const char* idname) {
  stringSet(&reassignment.idname, idname);
}

void analyseReassignRightId(const char* idname) {
  stringSet(&reassignment.rightId, idname);
}

void analyseReassignIdType(void) {
  if (!reassignment.started) {
    return;
  }

  symtableItem* it = global_symbolSearch(stringCStr(&reassignment.rightId), NULL);

  if (it == NULL
    || (it->data.symbolType != symbol_LET && it->data.symbolType != symbol_VAR)
    || !(it->data.flags & symbol_flag_INITIALIZED)) {
      EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  genExprOperand((ExprItem) { .type=expr_ID, .value.idName=(char*) stringCStr(&reassignment.rightId) });

  reassignment.type = it->data.dataType;
}

void analyseReassignType(Type type) {
  reassignment.type = type;
}

void analyseReassignEnd(void) {
    if (!reassignment.started) {
        return;
    }

    symtableItem* it = global_symbolSearch(stringCStr(&reassignment.idname), NULL);

    if (it == NULL) {
        EXIT_WITH_MESSAGE(UNDEFINED_VAR);
    }
    // only var can be reassigned
    if (it->data.symbolType != symbol_VAR)  {
        EXIT_WITH_MESSAGE(SEMANTIC_ERR);
    }

    if (it->data.dataType.base != reassignment.type.base
    || (!it->data.dataType.nullable && reassignment.type.nullable)) {
        EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
    }

    it->data.flags |= symbol_flag_INITIALIZED;
    /* global_insertTop(it->key, data); */

    /* if (reassignment.rightId.size > 0) { */
    /*     // push the value on stack */
    /*     genExprOperand((ExprItem) { .type=expr_ID, .value.idName=(char*) stringCStr(&assignment.idname) }); */
    /* } */
    genAssign(stringCStr(&reassignment.idname));

    prepareStatement();
}

// helper functions

bool _compareParams(Param* fnParams, unsigned fnCount, Param* callParams, unsigned callCount) {
  if (fnCount != callCount) {
    EXIT_WITH_MESSAGE(4);
  }

  for (unsigned i = 0; i < fnCount; i++) {
    if (!stringEq(&fnParams[i].label, &callParams[i].label)) {
      return false;
    }

    if (fnParams[i].type.base != callParams[i].type.base
      && !(fnParams[i].type.nullable && callParams[i].type.base == 'N')) {
      return false;
    }

    if (!fnParams[i].type.nullable && fnParams[i].type.nullable) {
      return false;
    }
  }

  return true;
}

void _checkPostponed(const char* fnId, SymbolData data) {
  FunctionLListItem* fn = functionStackRemove(postponedCheckStack, fnId);

  // function was called with wrong arguments
  if (fn != NULL) {
    if (!_compareParams(fn->params, fn->paramCount, data.params, data.paramCount)) {
      EXIT_WITH_MESSAGE(UNDEFINED_FN);
    }

    // function was called with correct arguments
    free(fn->params);
    free(fn->name);
    fn->name = NULL;
    fn->params = NULL;
    free(fn);
  }

  // function was not called
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
  symtableItem* it = global_symbolSearch(idname, NULL);

  if (it == NULL) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  if (it->data.symbolType == symbol_FN || !(it->data.flags & symbol_flag_INITIALIZED)) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  return it->data.dataType;
}

void analyseCondition(void) {
  if (lastExprType.base != 'B') {
    EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }
}

Type _analyseOperation(OperatorType optype, ExprItem* a, ExprItem* b) {
  if (a->type == expr_OPERATOR || b->type == expr_OPERATOR) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  Type typeA = (a->type == expr_CONST || a->type == expr_INTERMEDIATE)
    ? a->value.constValue.type
    : variableType(a->value.idName);

  Type typeB = (b->type == expr_CONST || b->type == expr_INTERMEDIATE)
    ? b->value.constValue.type
    : variableType(b->value.idName);

  if (!typeIsValue(typeA) || !typeIsValue(typeB)) {
      EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  if (optype == op_CONCAT || optype == op_PLUS) {
    if (typeA.base == 'S' && typeB.base == 'S') {
      return (Type) { 'S', (typeA.nullable || typeB.nullable) };
    }
  }

  if (optype == op_PLUS || optype == op_MINUS || optype == op_MUL || optype == op_DIV ) {

      // implicit const conversion
    if (typeA.base == 'I' && typeB.base == 'D' && a->type == expr_CONST) {
        typeA.base = 'D';
        a->value.constValue.type = typeA;
        a->value.constValue.value.FLOAT_VAL = (float) a->value.constValue.value.INT_VAL;
    }
    else if (typeA.base == 'D' && typeB.base == 'I' && b->type == expr_CONST) {
        typeB.base = 'D';
        b->value.constValue.type = typeB;
        b->value.constValue.value.FLOAT_VAL = (float) b->value.constValue.value.INT_VAL;
    }

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
      // implicit const conversion
    if (typeA.base == 'I' && typeB.base == 'D' && a->type == expr_CONST) {
        typeA.base = 'D';
        a->value.constValue.type = typeA;
        a->value.constValue.value.FLOAT_VAL = (float) a->value.constValue.value.INT_VAL;
    }
    else if (typeA.base == 'D' && typeB.base == 'I' && b->type == expr_CONST) {
        typeB.base = 'D';
        b->value.constValue.type = typeB;
        b->value.constValue.value.FLOAT_VAL = (float) b->value.constValue.value.INT_VAL;
    }

    if (typeA.base == typeB.base) {
      return (Type) { 'B', false };
    }
  }

  if (optype == op_LESS || optype == op_MORE || optype == op_LESS_EQ || optype == op_MORE_EQ) {
      // implicit const conversion
    if (typeA.base == 'I' && typeB.base == 'D' && a->type == expr_CONST) {
        typeA.base = 'D';
        a->value.constValue.type = typeA;
        a->value.constValue.value.FLOAT_VAL = (float) a->value.constValue.value.INT_VAL;
    }
    else if (typeA.base == 'D' && typeB.base == 'I' && b->type == expr_CONST) {
        typeB.base = 'D';
        b->value.constValue.type = typeB;
        b->value.constValue.value.FLOAT_VAL = (float) b->value.constValue.value.INT_VAL;
    }

    if (typeA.base == typeB.base && !typeA.nullable && !typeB.nullable) {
      return (Type) { 'B', false };
    }
  }

  EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
}

Type _analyseUnwrap(ExprItem e) {
  if (e.type == expr_OPERATOR) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  Type t = (e.type == expr_CONST || e.type == expr_INTERMEDIATE)
    ? e.value.constValue.type
    : variableType(e.value.idName);

  if (t.base == 'N') {
    EXIT_WITH_MESSAGE(TYPE_COMPATIBILITY_ERR);
  }

  t.nullable = false;
  return t;
}

String typeToStr(Type type) {
  String result;
  (stringInit(&result, ""));

  if (type.base != 'I' && type.base != 'D' && type.base != 'S') {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  stringConcatChar(&result, type.base);

  if (type.nullable) {
    stringConcatChar(&result, '?');
  }

  return result;
}

Type tokenToType(tokenType token) {
  switch (token) {
    case token_CONST_WHOLE_NUMBER:
    case token_TYPE_INT:
      return (Type) { 'I', false };

    case token_CONST_DEC_NUMBER:
    case token_CONST_SCIENTIFIC_NOTATION:
    case token_TYPE_DOUBLE:
      return (Type) { 'D', false };

    case token_TYPE_STRING_LINE:
    case token_TYPE_STRING:
      return (Type) { 'S', false };

    case token_TYPE_INT_Q:
      return (Type) { 'I', true };

    case token_TYPE_DOUBLE_Q:
      return (Type) { 'D', true };

    case token_TYPE_STRING_Q:
      return (Type) { 'S', true };

    default:
      EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }
}

void pushFnParams(const char* idname) {
  symtableItem* fn = symtableSearch(global_table, idname);

  if (fn == NULL || fn->data.symbolType != symbol_FN) {
    EXIT_WITH_MESSAGE(UNDEFINED_FN);
  }

  for (unsigned i = 0; i < fn->data.paramCount; i++) {
    Param* param = &fn->data.params[i];

    global_insertTop(stringCStr(&param->name),
        (SymbolData) { param->type, NULL, 0, symbol_flag_INITIALIZED, symbol_LET });
  }

}

void analyseIfLetBegin(void) {
  iflet.started = false;
}

void analyseIfLet(const char* idname) {
  iflet.started = true;
  stringSet(&iflet.idname, idname);
  genIfLet(idname);
}

void pushIfLet(void) {
  if (!iflet.started) {
    return;
  }

  unsigned oldId;
  symtableItem* it = global_symbolSearch(stringCStr(&iflet.idname), &oldId);

  if (it == NULL || it->data.symbolType == symbol_FN) {
    EXIT_WITH_MESSAGE(UNDEFINED_VAR);
  }

  SymbolData data = it->data;
  data.dataType.nullable = false;

  if (data.dataType.base == 'N') {
    data.dataType.base = 'u';
  }

  global_insertTop(stringCStr(&iflet.idname), data);

  unsigned newId;
  global_symbolSearch(stringCStr(&iflet.idname), &newId);

  genIfLetShadow(stringCStr(&iflet.idname), oldId, newId);
}

void analyseReassignAbort(void) {
  assignment.started = false;
}

void setCurrentFunction(String* idname) {
  stringSetS(&currentFunction, idname);
}

