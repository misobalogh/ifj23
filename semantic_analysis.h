#include <stdbool.h>
#include "semantic_analysis_stack.h"
#include "error_codes.h"
#include "symtable.h"
#include "synt_prec_stack.h"

extern SemStack* semStack;
extern stack* semanticStack;

error_codes semanticAnalysisInit(void);
error_codes checkDeclaration(void);

error_codes analyseFunctionAddId(const char* idname);
error_codes analyseFunctionAddParam(const char* fnIdname,
    const char* ida, const char* idb, const char* type);
error_codes analyseFunctionAddReturn(const char* fnIdname, const char* type);

error_codes analyseAssignment(const char* left, const char* right);
error_codes analyseLet(const char* idname);
error_codes analyseVar(const char* idname);
error_codes analyseId(const char* idname);

bool parseType(const char* typeStr, char out[3]);
