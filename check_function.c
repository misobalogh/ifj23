/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: check_function.c
* Description: functions for checking function parameters
* Author: MICHAL BALOGH
* Faculty: FIT VUT
* Date: 10.10.2023

* Comments: 

***************************************************************/


#include "check_function.h"


/**
 * @brief Concatenates string and semicolon
 */
#define concatWithSemicolon(result, str1) \
    strcpy(result, str1);                   \
    strcat(result, ";");



/**
 *  @brief Mock of nextToken function
 * !TODO: REMOVE THIS FUNCTION
 */
char* nextToken(int position) {
    char* str[] = { "_", "x", "String", "var1", "y", "Int", "var2", "z", "Double", ")", "Int" };
    int numTokens = sizeof(str) / sizeof(str[0]);

    if (position >= 0 && position < numTokens) {
        return str[position];
    }
    else {
        return NULL;
    }
}



/**
 * @brief Checks if the token is a function
 *
 * @param token Token to check
 */
bool token_isFunc(char* token) {
    return !strcmp(token, swiftKeywordToString(/* Swift function keyword code */));
}

/**
 * @brief Checks if the token is a right parenthesis - check wheter it is end of the function parameters
 *
 * @param token Token to check
 */
static bool token_isRightParenthesis(const char* token) {
    return strncmp(token, ")", 1) == 0;
}


/**
 * @brief Concatenates two strings with a semicolon in between
 *
 * @param str1 First string
 * @param str2 Second string
 *
 * @return Concatenated string
 */
static char* concatenateWithDelimiter(const char* str1, const char* str2) {
    int totalLength = strlen(str1) + strlen(str2) + 2; // +2 for delimiter and null terminator
    char* result = (char*)malloc(totalLength);
    CHECK_MEMORY_ALLOC(result);
    concatWithSemicolon(result, str1);
    strcat(result, str2);
    return result;
}



/**
 * @brief Checks if the token is a left parenthesis
 */
char* funcParamsToString() {
    char* functionName =  /* nextToken() */ "NameOfFunction";
    // nextToken(); // Remove the left parenthesis

    int numberOfParams = 0;

    char* concatenatedParams = (char*)malloc(strlen(functionName) + 2);
    CHECK_MEMORY_ALLOC(concatenatedParams);
    strcpy(concatenatedParams, functionName);

    while (!token_isRightParenthesis(nextToken(numberOfParams))) {
        char* newConcatenatedParams = concatenateWithDelimiter(concatenatedParams, nextToken(numberOfParams));
        free(concatenatedParams); // Free the old concatenatedParams
        concatenatedParams = newConcatenatedParams;
        numberOfParams++;
    }

    // Divide by 3 because there are 3 tokens per parameter (name, id, type), -2 because of the first two tokens (name of function and left parenthesis)
    numberOfParams /= 3;

    char* newConcatenatedParams = concatenateWithDelimiter(concatenatedParams, nextToken(numberOfParams + 1)); // get the return type of the function
    free(concatenatedParams);
    concatenatedParams = newConcatenatedParams;

    LOG("Concatenated params: %s", concatenatedParams);
    LOG("Number of params: %d", numberOfParams);


    // free(concatenatedParams); 
    return concatenatedParams;
}


/**
 * @brief Gets the function parameters from the string by splitting it by semicolon
 *
 * @param string String to get the parameters from
 *
 * @return array of function parameters
 */
char** getParamsFromString(const char* string, int* numberOfParams) {
    char* copy = malloc(strlen(string) + 1);
    strcpy(copy, string);

    char* token = strtok(copy, ";");
    char** tokens = NULL;
    *numberOfParams = 0;

    while (token != NULL) {
        (*numberOfParams)++;
        tokens = (char**)realloc(tokens, sizeof(char*) * (*numberOfParams));
        tokens[(*numberOfParams) - 1] = malloc(strlen(token) + 1);
        strcpy(tokens[(*numberOfParams) - 1], token);
        token = strtok(NULL, ";");
    }

    free(copy);

    return tokens;
}


/**
 * @brief Parses the function parameters
 *
 * @return True if the function parameters were parsed successfully, false otherwise
 */
char* parseFuncParams() {
    return NULL;
}

