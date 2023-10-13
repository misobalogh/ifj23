
#ifndef _SYNT_ANALYSIS_H
#define _SYNT_ANALYSIS_H

#include <stdbool.h>
#include "macros.h"

// Main function
bool syntacticAnalysis();


// Two methods - one for expressions, second for others

bool precedenceParser();
bool recursiveParser();


// private functions called by precedenceParser(); and recursiveParser();
bool parseStatement();
bool parseExpression();
bool parseFactor();
bool parseTerm();
bool parseDeclaration();
bool parseIfStatement();
bool parseWhileLoop();
bool parseFunctionCall();


#endif // _SYNT_ANALYSIS_H