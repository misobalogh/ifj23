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

int main() {
    // Working with swift_keywords
    const char* str = swiftTypeToString(2);
    if (str == NULL) {
        printf("NULL\n");
        return 1;
    }
    else {
        printf("%s\n", str);
    }
    
    if (isSwiftSpecialKeyword(str)) {
        printf("Is special keyword\n");
        if(isSwiftType(str)) {
            printf("Also it is type\n");
        }
        else {
            printf("Only keyword\n");
        }
    }
    else {
        printf("Not special keyword\n");   
    }

    printf("Hash: %d\n", hash("isSwift", 100, 101));

    
    // Working with hash table
    hashTable* htab = hashTableInit(HASH_TABLE_SIZE);

    for (int i = 0; i < 101; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        if(hashTableInsert(htab, key, i) == false) {
            printf("ERROR\n");
            free(key);
            return 1;
        }
        free(key);
    }

    for (int i = 0; i < 101; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        hashTableItem* item = hashTableSearch(htab, key);
        // printf("Key: %s, Data: %d\n", item->key, item->data);
        free(key);
    }

    hashTable *htab2 = hashTableInit(HASH_TABLE_SIZE);

    if (copyHashTable(htab2, htab) == false) {
        printf("1 ERROR\n");
        return 1;
    }

    for (int i = 0; i < 101; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        hashTableItem* item = hashTableSearch(htab2, key);
        // printf("Key: %s, Data: %d\n", item->key, item->data);
        free(key);
    }    

    hashTableResize(htab2);
    
    printf("okej\n");
    if (hashTableInsert(htab2, "key101", 101)) {
        printf("ERROR\n");
        return 1;
    }
    if (hashTableInsert(htab2, "key102", 102)) {
        printf("ERROR\n");
        return 1;
    }
    

    printf("Data 1: %d\n" , hashTableSearch(htab2, "key101")->data);
    printf("Data 2: %d\n" , hashTableSearch(htab2, "key102")->data);

    printf("\n\n\n");
    for (int i = 0; i < 202; i++) {
        char* key = (char*)malloc(sizeof(char) * 10);
        sprintf(key, "key%d", i);
        if(hashTableInsert(htab2, key, i) == false) {
            printf("ERROR\n");
            free(key);
            return 1;
        }
        printf("Key: %s, Data: %d\n", key, i);
        free(key);
    }

    // Working with load_input
    // char* buffer = NULL;
    // buffer = load_input(stdin);
    // printf("%s\n", buffer);
    // free(buffer);

    // hashTableFree(htab);
    // hashTableFree(htab2);
    return 0;
}
