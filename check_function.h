
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