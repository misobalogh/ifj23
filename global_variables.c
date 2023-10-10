/********************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23  *
* File Name: global_variables.c                                     *  
* Description: Table of symbols                                     *
* Author:  Michal Balogh xbalog06                                   *
* Faculty: FIT VUT                                                  *
* Date:    10.10.2023                                               *
*                                                                   *
* Comments: operator precedence is in inverse order as in table,    *
*           so the higher the number, the higher the precedence     *
*                                                                   *
********************************************************************/

#include "global_variables.h"



/**
 * @brief Global table of symbols
*/
symtable *global_table; 

/**
 * @brief Initialize global table of symbols
 */
static void global_initSymtable() {
    global_table = symtableInit(SYMTABLE_SIZE);
}


/**
 * @brief Checks if variable is already declared in global table of symbols
 * 
 * @param id Variable name
 * 
 * @return true if variable is already declared
 */
static bool global_doubleDeclaration(char* id) {
    symtableItem *found = symtableSearch(global_table, id);
    if (found != NULL) {
        return true;
    }
    return false;
}

/**
 * @brief Adds variable to global table of symbols
 * 
 * @param id Variable name
 * @param value Variable value
 * 
 * @return true if variable was added successfully
 */
bool global_addVar(char* id, char* type, int value) {
    if (global_table == NULL) {
        global_initSymtable();
    }

    if (global_doubleDeclaration(id)) {
        return false;
    }
    if (!symtableInsert(global_table, id, type, value)) {
        return false;
    }

    //TODO: @Michal418
    // global_generateInstruction();
    return true;
}

/**
 * generate instruction for interpret from global variable
 */
void global_generateInstruction();





