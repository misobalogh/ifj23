
#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

#include "symtable.h"
#include <stdbool.h>

/**
 * @brief Global table of symbols
*/
extern symtable *global_table; 

bool global_addVar(char* id, char* type, int value);

void global_generateInstruction();

#endif // GLOBAL_VARIABLES_H