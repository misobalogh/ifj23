/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef EXPR_LIST_H
#define EXPR_LIST_H

#include "token_types.h"
#include <stddef.h>

#define LIST_GROWTH 2

typedef enum ExprType { expr_CONST, expr_ID, expr_OPERATOR, expr_INTERMEDIATE } ExprType;

typedef struct Type {
  /*
   * I: Int
   * D: Double
   * S: String
   * N: nil
   * u: undefined
   * v: void
   */
  char base;
  bool nullable;
} Type;

bool typeIsValid(Type t);
bool typeIsVariable(Type t);
bool typeEq(Type a, Type b);
bool typeBaseEq(Type a, Type b);
bool typeIsValue(Type t);

typedef enum OperatorType {
    op_PLUS,
    op_MINUS,
    op_MUL,
    op_DIV,
    op_CONCAT,
    op_EQ,
    op_NEQ,
    op_LESS,
    op_MORE,
    op_LESS_EQ,
    op_MORE_EQ,
    op_DEFAULT,
    op_UNWRAP
} OperatorType;

OperatorType tokenToOperator(tokenType type);

typedef struct ExprItem {
  ExprType type;
  union {
    Type constType;
    char* idName;
    OperatorType operatorType;
  } value;
} ExprItem;

typedef struct ExprArray {
  size_t size;
  size_t capacity;
  ExprItem* data;
} ExprArray;

ExprArray* exprListInit(void);
void exprListResize(ExprArray* list);
void exprListFree(ExprArray* list);
void exprListAddConst(ExprArray* list, Type type);
void exprListAddId(ExprArray* list, const char* idname);
void exprListAddOperator(ExprArray* list, OperatorType optype);
void exprListClear(ExprArray* list);

#endif
