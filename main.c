/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: main.c
* Description: main program
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "macros.h"
#include "error_codes.h"
#include "load_input.h"
#include "swift_keywords.h"
#include "symtable.h"
#include "global_variables.h"
#include "check_function.h"
#include "synt_prec_stack.h"
#include "token_types.h"
#include "synt_prec_table.h"
#include "synt_analysis.h"



void cleanup(); 

void workingWithLoadInput() {
    char* buffer = NULL;
    buffer = load_input(stdin);
    printf("%s\n", buffer);
    free(buffer);
}

void workingWithSwiftKeywords() {
    const char* str = swiftTypeToString(2);
    if (str == NULL) {
        printf("NULL\n");
        exit(INTERNAL_ERROR);
    }
    else {
        printf("%s\n", str);
    }

    if (isSwiftSpecialKeyword(str)) {
        printf("Is special keyword\n");
        if (isSwiftType(str)) {
            printf("Also it is type\n");
        }
        else {
            printf("Only keyword\n");
        }
    }
    else {
        printf("Not special keyword\n");
    }
}

void workingWithHashTable() {
    int prime = 101;
    symtable* tab = symtableInit(prime); // init

    printf("Old item count: %d\n", tab->itemCount);
    printf("Old size: %d\n", tab->size);

    symtableItem* item = NULL;
    for (int i = 0; i < 258; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        CHECK_MEMORY_ALLOC(key);
        sprintf(key, "key%d", i);
        if (!symtableInsert(tab, key, "String", i)) { // insert with automatic resize
            fprintf(stderr, "Error - symtableInsert\n");
            symtableFree(tab); 
            return;
        }
        free(key);
    }

    for (int i = 0; i < tab->size; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        CHECK_MEMORY_ALLOC(key);
        sprintf(key, "key%d", i);
        item = symtableSearch(tab, key);
        if (item != NULL) {
            // printf("Key: %s, Data: %d\n", item->key, item->data);
        }

        free(key);
    }

    printf("New item count: %d\n", tab->itemCount);
    printf("New size: %d\n", tab->size);
    symtableFree(tab); // free
}

void workingWithParamsToString(){
    char* str = funcParamsToString();
    LOG("%s", str);
    int numParams = 0;
    char** params = getParamsFromString(str, &numParams);

    for (int i = 0; i < numParams; i++) {
        LOG("Param: %s", params[i]);
        free(params[i]);
    }

    free(str);
    free(params);
}

void workingWithStack(){
    stack* s = malloc(sizeof(stack));
    CHECK_MEMORY_ALLOC(s);
    stackInit(s);

    stackPush(s, token_ID);
    stackPush(s, token_PLUS);
    stackPush(s, token_MUL);
    stackPush(s, token_PARENTHESES_L);
    stackPush(s, token_PARENTHESES_R);

    if (stackFirst(s)) {
        LOG("%s", TOKEN_TYPE_NAME(stackFirst(s)->type));
    }
    if (stackSecond(s)) {
        LOG("%s", TOKEN_TYPE_NAME(stackSecond(s)->type));
    }
    if (stackThird(s)) {
        LOG("%s", TOKEN_TYPE_NAME(stackThird(s)->type));
    }

    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPop(s);
    stackPop(s);

    stackPush(s, token_NONTERMINAL);
    stackPush(s, token_NONTERMINAL);
    stackPush(s, token_NONTERMINAL);
    stackPush(s, token_NONTERMINAL);

    if(stackTopTerminal(s))
        LOG("Terminal %s", TOKEN_TYPE_NAME(stackTopTerminal(s)->type));
    stackFreeItems(s);
    free(s);
}

int main() {
    // workingWithSwiftKeywords();

    // workingWithHashTable();

    // workingWithLoadInput();

    // workingWithParamsToString();
    
    // workingWithStack();


    if(precedenceParser() == false) {
        LOG("MAIN: syntax Error: code %d\n", SYNTAX_ANALYSIS_ERR);
        return SYNTAX_ANALYSIS_ERR;
    }
    else {
        LOG("MAIN: Expression syntax is correct: code %d", 0);
    }
    return 0;
}
