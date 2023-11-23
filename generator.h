/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef GENERATOR_H
#define GENERATOR_H

#include "expr.h"
#include "stdbool.h"
#include "stdio.h"
#include "symtable.h"

void genSubstring(void);
void genFunction(const char* idname);
void genExprOperand(ExprItem e);
void genExprOperator(OperatorType optype);
void genAssign(const char* idname);
void genDef(const char* idname);
void genReturn(void);
void genEntry(void);
void genFirst(void);
void genCall(const char* idname, Param* params, unsigned paramCount);

#endif
