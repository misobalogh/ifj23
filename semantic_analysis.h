#include <stdbool.h>
#include "semantic_analysis_stack.h"
#include "error_codes.h"
#include "synt_prec_stack.h"

extern SemStack* semStack;
extern stack* semanticStack;

error_codes semanticAnalysisInit(void);
error_codes checkDeclaration(void);
error_codes analyseFunction(const char* idname);
error_codes analyseAssignment(const char* left, const char* right);
error_codes analyseLetOrVar(void);
