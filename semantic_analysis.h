#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <stdbool.h>
#include "error_codes.h"
#include "symtable.h"
#include "synt_prec_stack.h"

extern stack* semanticStack;

/**
 * @brief Initializes semantic analysis variables. Must be called before
 * any calls to any other semantic analysis functions
 */
error_codes semanticAnalysisInit(void);

/**
 * @brief Frees all memory allocated by semantic analysis.
 * Must be called after semantic analysis finished.
 */
void semanticAnalysisDeinit(void);

error_codes checkDeclaration(void);

/**
 * @brief Passes name of function in declaration for semantic check.
 * @param idname name of the function
 */
error_codes analyseFunctionId(const char* idname);
/**
 * brief Finalizes the semantic analysis of single functoin declaration.
 */
error_codes analyseFunctionEnd(void);
/**
 * @brief Passes parameter label for semantic analysis.
 * @param name Label Identificator of the label
 */
error_codes analyseFunctionParamLabel(const char* label);
/**
 * @brief Passes parameter name for semantic analysis.
 * @param name Identificator of the parameter name.
 */
error_codes analyseFunctionParamName(const char* name);
/**
 * @brief Passes parameter type for semantic analysis.
 * @param type Type of parameter
 */
error_codes analyseFunctionParamType(tokenType type);
/**
 * @brief Passes function type for semantic analysis.
 * @param type Type of the function.
 */
error_codes analyseFunctionType(tokenType type);

error_codes analyseLetId(const char* idname);
error_codes analyseVarId(const char* idname);
error_codes analyseTypeHint(tokenType type);
error_codes analyseAssignConst(tokenType type);
error_codes analyseAssignId(const char* idname);

error_codes analyseAssignment(const char* left, const char* right);
error_codes analyseLet(const char* idname);
error_codes analyseVar(const char* idname);
error_codes analyseId(const char* idname);

/* error_codes analyseCallId(const char* idname); */
/* error_codes analyseCallLabel(const char* label); */
/* error_codes analyseCallParam(const char* paramIdname); */
/* error_codes analyseCallParamConst(const char* data); */
/* error_codes analyseCallEpsilon(void); */


error_codes analyseCallConst(tokenType type);
error_codes analyseCallIdOrLabel(const char* value);
error_codes analyseCallEpsAfterId(void);
error_codes analyseCallIdAfterLabel(const char* idname);
error_codes analyseCallConstAfterLabel(tokenType type);
error_codes analyseCallEnd(void);

const char* typeShort(tokenType type);

#endif
