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

void genInit(void);
void genDeinit(void);

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

void genIfBegin(void);
void genIfBlock(void);
void genIfElse(void);
void genIfEnd(void);

void genIfLetShadow(const char* idname, unsigned oldId, unsigned newId);

void genWhileBegin(void);
void genWhileCondition(void);
void genWhileStats(void);
void genWhileEnd(void);

void genMainJump(void);
void genMainLabel(void);

void genExprResult(ExprItem e);

void genIfLet(const char* idname);

void _printEscaped(const char* str);

#endif
