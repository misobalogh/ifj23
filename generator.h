/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef GENERATOR_H
#define GENERATOR_H

#include "expr.h"
#include "stdbool.h"
#include "stdio.h"

void genFunction(const char* idname);
void genExprOperand(ExprItem e);
void genExprOperator(OperatorType optype);
void genAssign(const char* idname);
void genDef(const char* idname);
void genReturn(void);

#endif
