

#include <stdbool.h>
#include "token_types.h"
#include "error_codes.h"

error_codes rule_PROGRAM();
error_codes rule_RETURN_TYPE();
error_codes rule_FUNC_STAT_LIST();
error_codes rule_FUNC_STAT();
error_codes rule_CONDITION();
error_codes rule_RETURN_STAT();
error_codes rule_RET_VAL();
error_codes rule_PARAM_LIST();
error_codes rule_PARAM_NEXT();
error_codes rule_PARAM();
error_codes rule_STAT_LIST();
error_codes rule_STATEMENT();
error_codes rule_VAR_ASSIGNMENT();
error_codes rule_VAL_ASSIGNMENT();
error_codes rule_INPUT_PARAM_LIST();
error_codes rule_INPUT_PARAM_NEXT();
error_codes rule_INPUT_PARAM();
error_codes rule_WITH_NAME();
error_codes rule_ID_OR_CONST();
error_codes rule_LET_OR_VAR();
error_codes rule_EXPRESSION();
error_codes rule_AFTER_ID(lex_token idToken);
error_codes rule_FN_OR_EXP();
error_codes rule_ID_OR_UNDERSCORE();
error_codes rule_BRACK_STATEMENT();
error_codes rule_BRACK_STAT_LIST();
error_codes rule_TYPE();

