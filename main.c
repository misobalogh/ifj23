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

int main() {
    // workingWithSwiftKeywords();

    workingWithHashTable();

    // workingWithLoadInput();
    
    char* globalVar = "globalVar";
    global_addVar(globalVar, "String", 5);
    symtableItem* item = symtableSearch(global_table, globalVar);
    if (item != NULL) {
        printf("Key: %s, Data: %d\n", item->key, item->data);
    }
    char* globalVar2 = "f;S;S;_;x;S;with;y";
    global_addVar(globalVar2,"String", 5);
    item = symtableSearch(global_table, globalVar2);
    if (item != NULL) {
        printf("Key: %s, Data: %d\n", item->key, item->data);
    }
    char* globalVar3 = "globalVar";
    if(!global_addVar(globalVar3, "String", 5)) {
        printf("Double declaration\n");
    }

    cleanup();

    return 0;
}

void cleanup() {
    symtableFree(global_table);
}