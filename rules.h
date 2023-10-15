
#ifndef IFJ23_RULES_H
#define IFJ23_RULES_H

#include "synt_prec_table.h"
#include "synt_prec_stack.h"


void rule_ID(stack* s);

void rule_ID_OP_ID(stack* s);

void rule_PAR_ID_PAR(stack* s);

void rule_ID_CONCAT_ID(stack* s);

void rule_ID_FORCE_UNWRAP(stack* s);

void rule_ID_REL_ID(stack* s);


#endif //IFJ23_RULES_H
