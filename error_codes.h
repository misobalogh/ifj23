/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: error_codes.h
* Description: error codes defined in point 2 - Zadání of the assignment
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#ifndef IFJ23_ERROR_CODES_H
#define IFJ23_ERROR_CODES_H

/**
 * @brief Error codes defined in point 2 - Zadání of the assignment
 *
 * @param LEX_ANALYSIS_ERR Error in the program within lexical analysis (incorrect structure of the current lexeme)
 * @param SYNTAX_ANALYSIS_ERR Error in the program within syntactic analysis (incorrect program syntax, missing header...)
 * @param UNDEFINED_FN Semantic error in the program – undefined function
 * @param VAR_REDEFINITION Semantic error in the program – redefinition of variable
 * @param WRONG_PARAM Semantic error in the program – wrong number/type of parameters in function call
 * @param WRONG_RETURN_TYPE Semantic error in the program – wrong type of return value in function call
 * @param UNDEFINED_VAR Semantic error in the program – undefined variable
 * @param UNINITIALIZED_VAR Semantic error in the program – uninitialized variable
 * @param RETURN_EXPRESSION_ERR Semantic error in the program – missing/excess expression in the return statement from a function
 * @param TYPE_COMPATIBILITY_ERR Semantic error of type compatibility in arithmetic, string, and relational expressions
 * @param TYPE_INFERENCE_ERR Semantic error of type inference – the type of a variable or parameter is not specified and cannot be inferred from the used expression
 * @param SEMANTIC_ERR Other semantic errors
 * @param INTERNAL_ERROR Internal error of compailer, not influenced by the input program (memory allocation error...)
*/
typedef enum {
    LEX_ANALYSIS_ERR = 1, // Error in the program within lexical analysis (incorrect structure of the current lexeme)
    SYNTAX_ANALYSIS_ERR = 2, // Error in the program within syntactic analysis (incorrect program syntax, missing header...)
    UNDEFINED_FN = 3, // Semantic error in the program – undefined function
    VAR_REDEFINITION = 3, // Semantic error in the program – redefinition of variable
    WRONG_PARAM = 4, // Semantic error in the program – wrong number/type of parameters in function call
    WRONG_RETURN_TYPE = 4, // Semantic error in the program – wrong type of return value in function call
    UNDEFINED_VAR = 5, // Semantic error in the program – undefined variable
    UNINITIALIZED_VAR = 5, // Semantic error in the program – uninitialized variable
    RETURN_EXPRESSION_ERR = 6, // Semantic error in the program – missing/excess expression in the return statement from a function
    TYPE_COMPATIBILITY_ERR = 7, // Semantic error of type compatibility in arithmetic, string, and relational expressions
    TYPE_INFERENCE_ERR = 8, // Semantic error of type inference – the type of a variable or parameter is not specified and cannot be inferred from the used expression
    SEMANTIC_ERR = 9, // Other semantic errors
    INTERNAL_ERROR = 99 // Internal error of compailer, not influenced by the input program (memory allocation error...)
} error_codes;


#endif //IFJ23_ERROR_CODES_H