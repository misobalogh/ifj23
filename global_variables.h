#include "symtable.h"
#include <stdbool.h>


#ifndef GLOBAL_VARIABLES_H
#define GLOBAL_VARIABLES_H

/**
 * @brief Global table of symbols
*/
symtable *global_table; 

bool global_addVar(char* id, char* type, int value);
void global_generateInstruction();


#endif // GLOBAL_VARIABLES_H