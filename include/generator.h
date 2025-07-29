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

/**
 * @brief initialize generator module
 */
void genInit(void);

/**
 * @brief free all memory allocated during initialization and usage
 */
void genDeinit(void);

/**
 * @generate the substring function
 */
void genSubstring(void);

/**
 * @brief generate the start of function definition
 */
void genFunction(const char* idname);

/**
 * @brief generate instruction that pushes operand value to stack
 */
void genExprOperand(ExprItem e);

/**
 * @brief generate instructions that pop operands from stack and perform the operation
 */
void genExprOperator(OperatorType optype);

/**
 * @brief generate instructions for variable assignment
 */
void genAssign(const char* idname);

/**
 * @brief generate variable definition
 */
void genDef(const char* idname);

/**
 * @brief generate the return instruction
 */
void genReturn(void);

/**
 * @brief generate instructions for function call
 */
void genCall(const char* idname, Param* params, unsigned paramCount);

/**
 * @brief begin generating if-statement
 */
void genIfBegin(void);

/**
 * @brief generate label identifying if-statement block
 */
void genIfBlock(void);

/**
 * @brief generate label identifying else block
 */
void genIfElse(void);

/**
 * @brief generate the end of if-statement
 */
void genIfEnd(void);

/**
 * @brief generate the definition and assignment of a new variable that shadows
 * the variable in condition of if-let statement
 */
void genIfLetShadow(const char* idname, unsigned oldId, unsigned newId);

/**
 * @brief begin generating while-statement
 */
void genWhileBegin(void);

/**
 * @brief generate conditional jump for while-statement
 */
void genWhileStats(void);

/**
 * @brief generate jump at the end of while-statement
 */
void genWhileEnd(void);

/**
 * @brief generate jump to the end of function definition
 */
void genMainJump(void);

/**
 * @brief generate label after end of function definition
 */
void genMainLabel(void);

/**
 * @brief generate the condition for if-let statement
 */
void genIfLet(const char* idname);

/**
 * @brief record information about variable declaration inside while-loop for future declaration generation
 */
void genWhileDeclaration(const char* idname);

/**
 * @brief switch two values at the top of the stack
 */
void genSwitchStackTop(void);

/**
 * @print a string to stdout with special characters escaped
 */
void _printEscaped(const char* str);

#endif
