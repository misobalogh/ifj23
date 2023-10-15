/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: check_function.h
* Description: header file for check_function.c
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 10.10.2023

* Comments: 

***************************************************************/

#ifndef FUNCTION_CHECK_H
#define FUNCTION_CHECK_H

#include "swift_keywords.h" 
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>
#include "macros.h"

char* funcParamsToString();

char** getParamsFromString(const char* string, int* numberOfParams);

char* parseFuncParams();

#endif // FUNCTION_CHECK_H