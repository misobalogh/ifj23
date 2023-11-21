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
#include "symtablestack.h"
#include <stdbool.h>

/**
 * @brief Global table of symbols
*/
extern symtable *global_table;
extern SymtableStack* global_symtableStack;

void global_initSymtableStack(void);
void global_initSymtable(void);
void global_freeSymtable(void);
void global_freeSymtableStack(void);
bool global_addVar(char* id, char* type, int value);

void global_generateInstruction();

symtableItem* global_symbolSearch(const char* key);
void global_insertTop(const char* key, SymbolData data);
symtableItem* global_searchTop(const char* key);

const char* errorToString(error_codes err);

#endif // IFJ23_GLOBAL_VARIABLES_H
