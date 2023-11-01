/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: global_variables.h
* Description: interface for global variables and global table of symbols
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 14.10.2023

* Comments: 

***************************************************************/


#ifndef IFJ23_GLOBAL_VARIABLES_H
#define IFJ23_GLOBAL_VARIABLES_H

#include "symtable.h"
#include <stdbool.h>

/**
 * @brief Global table of symbols
*/
extern symtable *global_table; 

bool global_addVar(char* id, char* type, int value);

void global_generateInstruction();

#endif // IFJ23_GLOBAL_VARIABLES_H
