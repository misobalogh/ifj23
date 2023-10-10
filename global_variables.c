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
#include <string.h>
#include <stdarg.h>


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











bool token_isFunc(char* token) {
    return !strcmp(token, swiftKeywordToString(SWIFT_FUNC));
}


static char* parseOneFuncParameter(const char* type,const char* name, const char* id) {
    size_t parsedParamLen = strlen(type) + strlen(name) + strlen(id) + 4; 
    
    char* parsedParam = (char*)malloc(parsedParamLen);
    CHECK_MEMORY_ALLOC(parsedParam);

    snprintf(parsedParam, parsedParamLen, "%c;%s;%s;", type[0], name, id);

    return parsedParam;
}

//TODO:
char* transformFuncParams(int numberOfTokens, ...) {
    va_list tokens;
    size_t length = 0;

    va_start(tokens, numberOfTokens);

    for (int i = 0; i < numberOfTokens / 3; i++) {
        const char* type = va_arg(tokens, const char*);
        const char* name = va_arg(tokens, const char*);
        const char* id   = va_arg(tokens, const char*);
        printf("%s, %s, %s\n", type, name, id);
        char *parsed = parseOneFuncParameter(type, name, id);
        printf("\nParsed arg: %s\n",parsed );
        length += strlen(parsed);
        free(parsed);
    }
    
    va_end(tokens);

    va_start(tokens, numberOfTokens);
    char *transformedParams = malloc(sizeof(char) * (length + 3));
    transformedParams[0] = 'f';
    transformedParams[1] = ';';
    transformedParams[2] = '\0';

    for (int i = 0; i < numberOfTokens / 3; i++) {
            const char* type = va_arg(tokens, const char*);
            const char* name = va_arg(tokens, const char*);
            const char* id   = va_arg(tokens, const char*);
            char *parsed = parseOneFuncParameter(type, name, id);
            transformedParams = strcat(transformedParams, parsed);
            free(parsed);
        }
    va_end(tokens);
    transformedParams[length + 1] = '\0';
    printf("Transformed params: %s\n", transformedParams);

    printf("Length of all params %ld\n", length);
    return transformedParams;
}

char *nextToken() {
    return ")";
}

char* parseFuncParams() {
    const char *name = nextToken();

    int numberOfTokens = 0;
    while (nextToken() != ")") {
        numberOfTokens++;
    }
    numberOfTokens++;

    //char* transformedParams = transformFuncParams(numberOfTokens,);
    // symtableInsert(global_table, name, transformedParams, numberOfTokens);
}

