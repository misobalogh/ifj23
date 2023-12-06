/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef EXPR_LIST_H
#define EXPR_LIST_H

#include "token_types.h"
#include <stddef.h>

#define LIST_GROWTH 2

// internal type of expression
typedef enum ExprType { expr_CONST, expr_ID, expr_OPERATOR, expr_INTERMEDIATE } ExprType;

/**
 * @brief IFJ23 data type
 */
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

/**
 * @brief Check if t.base valid value?
 */
bool typeIsValid(Type t);

/**
 * @brief Check if type is actual value
 */
bool typeIsValue(Type t);

/**
 * @brief Check if variable can be this type
 */
bool typeIsVariable(Type t);

/**
 * @brief Check if types equal
 */
bool typeEq(Type a, Type b);

// type of operator
typedef enum OperatorType {
    op_PLUS,
    op_MINUS,
    op_MUL,
    op_DIV,
    op_IDIV,
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

/**
 * @brief get OperatorType from tokenType
 */
OperatorType tokenToOperator(tokenType type);

/**
 * @brief Item in expression - operator or operand
 */
typedef struct ExprItem {
  ExprType type;
  union {
    struct {
      Type type; // datatype of operand
      value_of_token value; // value of const
    } constValue;
    char* idName; // identificator of variable
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
void exprListAddInt(ExprArray* list, int value);
void exprListAddFloat(ExprArray* list, double value);
void exprListAddString(ExprArray* list, const char* value);
void exprListAddId(ExprArray* list, const char* idname);
void exprListAddOperator(ExprArray* list, OperatorType optype);
void exprListAddNil(ExprArray* list);
void exprListClear(ExprArray* list);

#endif
