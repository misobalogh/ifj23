/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name:
* Description:
* Author:
* Faculty: FIT VUT
* Date:

* Comments:

***************************************************************/

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "macros.h"

// Prime number for size of hash table
#define HASH_TABLE_SIZE 101

/**
 * @brief struct for hash table item
 * 
 * @param key string key
 * @param data integer data
 */
typedef struct htableItem {
    char* key;
    int data;
} hashTableItem;

/**
 * @brief struct for hash table
 * 
 * @param size size of hash table
 * @param count number of items in hash table
 * @param table array of hash table items
*/
typedef struct hashTable {
    int size;
    int count;
    hashTableItem* table;
} hashTable;


uint32_t hash(const char* key, int length);

hashTable* hashTableInit();

bool hashTableInsert(hashTable* htab, const char* key, int data);

hashTableItem* htableSearch(hashTable* htab, const char* key);

void hashTableDelete(hashTable* htab, const char* key);

void hashTableClear(hashTable* htab);

void hashTableFree(hashTable* htab);







#endif // HASH_TABLE_H