

#include <stdbool.h>
#include "token_types.h"

bool rule_PROGRAM();
bool rule_STAT_LIST();
bool rule_STATEMENT();
bool rule_EOF();
bool rule_EOL();
bool rec_rule_ID();
bool rule_VAR_DEFINITION();
bool rule_VAR_ASSIGNMENT();
bool rule_LET();
bool rule_VAR();
bool rule_EXPRESSION();
bool rule_ASSIGN();

