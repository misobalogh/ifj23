#ifndef SEMANTIC_ANALYSIS_H
#define SEMANTIC_ANALYSIS_H

#include <stdbool.h>
#include "error_codes.h"
#include "symtable.h"
#include "synt_prec_stack.h"

extern stack* semanticStack;

error_codes semanticAnalysisInit(void);
error_codes checkDeclaration(void);

error_codes analyseFunctionAddId(const char* idname);
error_codes analyseFunctionAddParam(const char* id1, const char* id2, const char* type);
error_codes analyseFunctionAddReturn( const char* type);
error_codes analyseFunctionEnd(void);

error_codes analyseAssignment(const char* left, const char* right);
error_codes analyseLet(const char* idname);
error_codes analyseVar(const char* idname);
error_codes analyseId(const char* idname);

error_codes analyseCallId(const char* idname);
error_codes analyseCallLabel(const char* label);
error_codes analyseCallParam(const char* paramIdname);
error_codes analyseCallParamConst(const char* data);
error_codes analyseCallEpsilon(void);
error_codes analyseCallEnd(void);

const char* parseType(const char* typeStr);

#endif
