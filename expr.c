/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "expr.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

OperatorType tokenToOperator(tokenType type) {
  switch (type) {
    case token_PLUS:
      return op_PLUS;
    case token_MINUS:
      return op_MINUS;
    case token_MUL:
      return op_MUL;
    case token_DIV:
      return op_DIV;
    case token_CONCAT:
      return op_CONCAT;
    case token_EQ:
      return op_EQ;
    case token_NEQ:
      return op_NEQ;
    case token_LESS:
      return op_LESS;
    case token_MORE:
      return op_MORE;
    case token_LESS_EQ:
      return op_LESS_EQ;
    case token_MORE_EQ:
      return op_MORE_EQ;
    case token_FORCE_UNWRAP:
      return op_UNWRAP;
    default:
      exit(99);
  }
}

ExprArray* exprListInit(void) {
  ExprArray* result = malloc(sizeof(ExprArray));

  if (result == NULL) {
    exit(99);
  }

  result->size = 0;
  result->capacity = 64;

  result->data = malloc(sizeof(ExprItem) * result->capacity);

  if (result->data == NULL) {
    exit(99);
  }

  return result;
}

void exprListFree(ExprArray* list) {
  free(list->data);
  list->data = NULL;
  free(list);
}

void exprListResize(ExprArray* list) {
  if (list->size + 1 >= list->capacity) {
    void* ptr = realloc(list->data, list->capacity * LIST_GROWTH);
    if (ptr == NULL)  {
      exit(99);
    }
    list->data = ptr;
  }
}

void exprListAddConst(ExprArray* list, Type type) {
  if (type.base != 'S' && type.base != 'I' && type.base != 'D') {
    exit(99);
  }

  exprListResize(list);

  ExprItem val = { .type=expr_CONST, .value={ .constType=type } };
  list->data[list->size++] = val;
}

void exprListAddId(ExprArray* list, const char* idname) {
  exprListResize(list);

  ExprItem val = { .type=expr_ID, .value={ 0 } };
  size_t len = strlen(idname) + 1;
  val.value.idName = malloc(len);
  CHECK_MEMORY_ALLOC(val.value.idName);
  strncpy(val.value.idName, idname, len);
  list->data[list->size++] = val;
}

void exprListAddOperator(ExprArray* list, OperatorType optype) {
  exprListResize(list);

  ExprItem val = { .type=expr_OPERATOR, .value={ .operatorType=optype } };
  list->data[list->size++] = val;
}

void exprListClear(ExprArray* list) {
  list->size = 0;
}

bool typeIsValid(Type t) {
  return t.base == 'I' || t.base == 'D' || t.base == 'S'
    || t.base == 'u' || t.base == 'N' || t.base == 'v';
}

bool typeIsVariable(Type t) {
  return typeIsValid(t) && t.base != 'v';
}

bool typeIsValue(Type t) {
  return typeIsValid(t) && t.base != 'v' && t.base != 'u';
}

bool typeEq(Type a, Type b) {
  return a.base == b.base && a.nullable == b.nullable;
}

bool typeBaseEq(Type a, Type b) {
  return a.base == b.base;
}

