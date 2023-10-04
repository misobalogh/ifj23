/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: swift_types.h
* Description: data types for swift and key words
* Author:
* Faculty: FIT VUT
* Date:

* Comments:

***************************************************************/

#ifndef SWIFT_TYPES_H
#define SWIFT_TYPES_H

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

/**
 * @brief Array of strings for swift types
 */
const char* swift_type_names[] = {
    "Int",
    "Double",
    "String"
};

/**
 * @brief Array of strings for swift keywords
 */
const char* swift_keyword_names[] = {
    "else",
    "func",
    "if",
    "let",
    "nil",
    "return",
    "var",
    "while"
};

/**
 * @brief Converts enum swift_types to string
 *
 * @param type enum swift_types
 * @return const char* string or NULL if type is out of range
 */
const char* swiftTypeToString(enum swift_types type) {
    if (type >= sizeof(swift_type_names) / sizeof(swift_type_names[0]) || type < 0) { // check if type is in range of enum 
        return NULL;
    }
    return swift_type_names[type];
}


/**
 * @brief Converts enum swift_keywords to string
 *
 * @param keyword enum swift_keywords
 * @return const char* string or NULL if keyword is out of range
 */
const char* swiftKeywordToString(enum swift_keywords keyword) {
    if (keyword >= sizeof(swift_keyword_names) / sizeof(swift_keyword_names[0]) || keyword < 0) { // check if keyword is in range of enum
        return NULL;
    }
    return swift_keyword_names[keyword];
}


/**
 * @brief Checks whether given string is swift type
 *
 * @param id string to check
 *
 * @return true if string is swift type
*/
bool isSwiftType(const char* id) {
    for (size_t type = 0; type < sizeof(swift_type_names) / sizeof(swift_type_names[0]); type++) {
        if (strcmp(id, swift_type_names[type]) == 0) {
            return true;
        }
    }
    return false;
}


/**
 * @brief Checks whether given string is swift keyword
 *
 * @param id string to check
 *
 * @return true if string is swift keyword
*/
bool isSwiftKeyword(const char* id) {
    for (size_t keyword = 0; keyword < sizeof(swift_keyword_names) / sizeof(swift_keyword_names[0]); keyword++) {
        if (strcmp(id, swift_keyword_names[keyword]) == 0) {
            return true;
        }
    }
    return false;
}


/**
 * @brief Checks whether given string is swift keyword or type
 *
 * @param id string to check
 *
 * @return true if string is swift keyword or type
*/
bool isSwiftSpecialKeyword(const char* id) {
    if (isSwiftType(id) || isSwiftKeyword(id)) {
        return true;
    }
    return false;
}

#endif // SWIFT_TYPES_H