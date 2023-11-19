#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <stdbool.h>
#include "dynamic_string.h"
#include "error_codes.h"
#include "expr.h"
#include "symtable.h"
#include "synt_prec_stack.h"

extern Type lastExprType;

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

void analyseAssignBegin(void);
void analyseAssignLet(bool let);
void analyseAssignId(const char* idname);
void analyseAssignHint(tokenType type);
void analyseAssignRightId(const char* idname);
void analyseAssignType(Type type);
void analyseAssignEndExpr(void);
void analyseAssignEndNodef(void);
void analyseAssignEndCall(Type returnedType);

void analyseCallFnId(const char* idname);
void analyseCallConst(tokenType type);
void analyseCallIdOrLabel(const char* value);
void analyseCallEpsAfterId(void);
void analyseCallIdAfterLabel(const char* idname);
void analyseCallConstAfterLabel(tokenType type);
Type analyseCallEnd(void);

void analyseExprBegin(void);
void analyseExprOperand(lex_token token);
void analyseExprOperator(lex_token token);
void analyseExprDefault();
Type analyseExprEnd(void);

void analyseReassignStart();
void analyseReassignId(const char* idname);
void analyseReassignRightId(const char* idname);
void analyseReassignEnd(void);
void analyseReassignEndType(Type type);

void analyseCondition(void);

void analyseIfLetBegin(void);
void analyseIfLet(const char* idname);
void pushIfLet(void);

void pushFnParams(const char* idname);

bool _compareParams(Param* fnParams, unsigned fnCount, Param* callParams, unsigned callCount);
const char* _typeShort(tokenType type);
void _checkPostponed(const char* fnId, SymbolData data);
Type _analyseOperation(OperatorType optype, ExprItem a, ExprItem b);
Type strToType(const char* typeStr);
String typeToStr(Type type);
Type tokenToType(tokenType token);
Type variableType(const char* idname);

#endif
