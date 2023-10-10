/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: swift_types.h
* Description: data types for swift and key words
* Author:
* Faculty: FIT VUT
* Date:

* Comments:

***************************************************************/

#ifndef _SWIFT_TYPES_H
#define _SWIFT_TYPES_H

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief Enum for all availible swift types
 */
enum swift_types {
    SWIFT_INT,
    SWIFT_DOUBLE,
    SWIFT_STRING,
};

/**
 * @brief Enum for all availible swift keywords
 */
enum swift_keywords {
    SWIFT_ELSE,
    SWIFT_FUNC,
    SWIFT_IF,
    SWIFT_LET,
    SWIFT_NIL,
    SWIFT_RETURN,
    SWIFT_VAR,
    SWIFT_WHILE,
};

const char* swiftTypeToString(enum swift_types type);

const char* swiftKeywordToString(enum swift_keywords keyword);

bool isSwiftType(const char* id);

bool isSwiftKeyword(const char* id);

bool isSwiftSpecialKeyword(const char* id);


#endif // _SWIFT_TYPES_H