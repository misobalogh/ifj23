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
#include <stdlib.h>
#include "macros.h"
#include "token_types.h" // remove later
#include "error_codes.h"

#define LEX_ERR_CHECK() do { if (t.type == token_LEX_ERROR) { exit(LEX_ANALYSIS_ERR); } } while(0)



// Main function
bool syntacticAnalysis();


/**
 * @brief Stash for token for expression parser
 * when switching from recursive parser when deciding,
 * whether it is expression or function call
 */
extern lex_token stash;


/**
 * @brief Most recent token from scanner
 */
extern lex_token t;


/**
 * @brief Consumes optional EOL between tokens (not the one on the end of statement)
 *
 * @return void
*/
void consume_optional_EOL();


// Two methods - one for expressions, second for everything else

// Expression parser
bool precedenceParser();

// Recursive descent parser
bool recursiveParser();

/**
 * @brief Mocking function for testing purposes
*/
tokenStruct* mock_precedence_nextToken();

/**
 * @brief Mocking function for testing purposes
*/
tokenStruct* mock_recursive_nextToken();

#endif // _SYNT_ANALYSIS_H