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
#include "hash_table.h"


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
    hashTable* htab = hashTableInit(HASH_TABLE_SIZE); // init

    for (int i = 0; i < 124; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        int retValue = hashTableInsert(htab, key, i);
        if (retValue == 0) {    // insert, automatic resize
            printf("ERROR\n");
            free(key);
            hashTableFree(htab);
            exit(1);
        }
        // if (retValue == 1) {
        //     hashTableResize(&htab);
        //     hashTableInsert(htab, key, i);
        // }
        free(key);
    }    

    hashTableItem* item;
    for (int i = 0; i < 202; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        if ((item = hashTableSearch(htab, key))) {
            printf("Key: %s, Data: %d\n", item->key, item->data);
        }
        free(key);
    }
    hashTableFree(htab); // free
}

int main() {
    // workingWithSwiftKeywords();

    workingWithHashTable();

    // workingWithLoadInput();
    

    return 0;
}
