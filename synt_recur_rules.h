

#include <stdbool.h>
#include "token_types.h"

bool rule_PROGRAM();
bool rule_FUNCTION();
bool rule_RETURN_TYPE();
bool rule_FUNC_STAT_LIST();
bool rule_FUNC_STAT();
bool rule_CONDITION();
bool rule_RETURN_STAT();
bool rule_RET_VAL();
bool rule_PARAM_LIST();
bool rule_PARAM_NEXT();
bool rule_PARAM();
bool rule_STAT_LIST();
bool rule_STATEMENT();
bool rule_VAR_ASSIGNMENT();
bool rule_VAR_DEFINITION();
bool rule_VAL_ASSIGNMENT();
bool rule_INPUT_PARAM_LIST();
bool rule_INPUT_PARAM_NEXT();
bool rule_INPUT_PARAM();
bool rule_WITH_NAME();
bool rule_ID_OR_CONST();





// switch to precedence parser
bool rule_EXPRESSION();

