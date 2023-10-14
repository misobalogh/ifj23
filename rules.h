
#ifndef IFJ23_RULES_H
#define IFJ23_RULES_H

#include "synt_prec_table.h"
#include "synt_prec_stack.h"


void rule_ID(stack* s);

void rule_ID_OP_ID(stack* s);

void rule_PAR_ID_PAR(stack* s);


#endif //IFJ23_RULES_H
