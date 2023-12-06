/*
 * Project: Implementace překladače imperativního jazyka IFJ23
 * Authors: Michal Cenek xcenek04
*/


#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <stdbool.h>
#include "dynamic_string.h"
#include "error_codes.h"
#include "expr.h"
#include "symtable.h"
#include "synt_prec_stack.h"

/**
 * @brief Initializes semantic analysis variables. Must be called before
 * any calls to any other semantic analysis functions
 */
bool semanticAnalysisInit(void);

/**
 * @brief Frees all memory allocated by semantic analysis.
 * Must be called after semantic analysis finished.
 */
void semanticAnalysisDeinit(void);

void checkDeclaration(void);

/**
 * @brief Passes name of function in declaration for semantic check.
 * @param idname name of the function
 */
void analyseFunctionId(const char* idname);
/**
 * brief Finalizes the semantic analysis of single functoin declaration.
 */
void analyseFunctionEnd(void);
/**
 * @brief Passes parameter label for semantic analysis.
 * @param name Label Identificator of the label
 */
void analyseFunctionParamLabel(const char* label);
/**
 * @brief Passes parameter name for semantic analysis.
 * @param name Identificator of the parameter name.
 */
void analyseFunctionParamName(const char* name);
/**
 * @brief Passes parameter type for semantic analysis.
 * @param type Type of parameter
 */
void analyseFunctionParamType(tokenType type);
/**
 * @brief Passes function type for semantic analysis.
 * @param type Type of the function.
 */
void analyseFunctionType(tokenType type);

// begin analysing variable declaration and assignment
void analyseAssignBegin(void);

// set f declared variable is modifiable
void analyseAssignLet(bool let);

// set identificator of variable which is being assigned to
void analyseAssignId(const char* idname);

// set type of type annotation
void analyseAssignHint(tokenType type);

// set identificator R-ID in `<let|var> L-ID = R-ID` type of assignment
void analyseAssignRightId(const char* idname);

// get type from identificator previously set with `analyseAssignRightId` function
void analyseAssignIdType(void);

// set type of assigned value
void analyseAssignType(Type type);

// analyse assignment
void analyseAssignEnd(void);

// set id of called function
void analyseCallFnId(const char* idname);

// set parameter value when parameter is const
void analyseCallConst(lex_token token);

// set identifier which can be either variable name or parameter name
void analyseCallIdOrLabel(const char* value);

// get variable identifier name from value previously provided with `analyseCallIdOrLabel` function
void analyseCallEpsAfterId(void);

// set parameter name from value previously provided with `analyseCallIdOrLabel` function
// and set variable id as idname
void analyseCallIdAfterLabel(const char* idname);

// set parameter name from value previously provided with `analyseCallIdOrLabel` function
// and set parameter value as const value in token
void analyseCallConstAfterLabel(lex_token token);

// analyse function call
Type analyseCallEnd(void);

// begin analysing expression
void analyseExprBegin(void);

// analyse operand in expression
void analyseExprOperand(lex_token token);

// analyse operator in expression
void analyseExprOperator(lex_token token);

// analyse the `??` operator in expression
void analyseExprDefault();

// analyse expression and return type of result
Type analyseExprEnd(void);

// begin analysing assignment
void analyseReassignStart();

// set variable identifier as idname
void analyseReassignId(const char* idname);

// set R-ID in assignment of type `<let|var> L-ID = R-ID` to idname
void analyseReassignRightId(const char* idname);

// get type of assigned value from variable identifier previously provided with `analyseReassignRightId` function
void analyseReassignIdType(void);

// analyse assignment
void analyseReassignEnd(void);

// set type of assigned value
void analyseReassignType(Type type);

// abort analysing assignment
void analyseReassignAbort(void);

// check if type of last expression is boolean value
void analyseCondition(void);

// begin analysing if-let expression
void analyseIfLetBegin(void);

// analyse if-let expression
void analyseIfLet(const char* idname);

// push new symbol table to stack of symbol tables with variable that shadows the variable in if-let condition and is not null
void pushIfLet(void);

// begin analysing return statement
void analyseReturnBegin(void);

// check if type returned type matches functions return type
void analyseReturn(Type type);

// push new symbol table to stack of symbol tables which contains function parameters
void pushFnParams(const char* idname);

// prepare for analysing new statement
void prepareStatement(void);

// set identifier of current function
void setCurrentFunction(String* idname);


// "private" functions


// check if function call parameters match function definition parameters
bool _compareParams(Param* fnParams, unsigned fnCount, Param* callParams, unsigned callCount);

// check if function is in list of function calls
void _checkPostponed(const char* fnId, Param* params, unsigned paramCount);

// analyse binary operation
Type _analyseOperation(OperatorType optype, ExprItem* a, ExprItem* b);

// analyse the `!` unary operation
Type _analyseUnwrap(ExprItem e);

// get constants datatype from its token
Type tokenToType(tokenType token);

// get variables datatype from its identifier
Type variableType(const char* idname);

#endif
