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

    printf("Hash: %d\n", hash("isSwift", 100));
    hashTable* htab = hashTableInit();

    if (hashTableInsert(htab, "isSwift", 1111)) { 
        for(int i = 0; i < 101; i++) {
            if (htab->table[i].key != NULL) {
                printf("%s\n", htab->table[i].key);
            }
        }
    }


    // char* buffer = NULL;
    // buffer = load_input(stdin);
    // printf("%s\n", buffer);
    // free(buffer);
    return 0;
}
