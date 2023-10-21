/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: synt_analysis.h
* Description: Header file for syntactic analysis
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 15.10.2023

* Comments: 

***************************************************************/

#ifndef _SYNT_ANALYSIS_H
#define _SYNT_ANALYSIS_H

#include <stdbool.h>
#include "macros.h"
#include "token_types.h" // remove later

// Main function
bool syntacticAnalysis();


// Two methods - one for expressions, second for others

bool precedenceParser();
bool recursiveParser();

/**
 * @brief Mocking function for testing purposes
*/
tokenStruct *mock_precedence_nextToken();

/**
 * @brief Mocking function for testing purposes
*/
tokenStruct *mock_recursive_nextToken();

#endif // _SYNT_ANALYSIS_H