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
#define HASH_TABLE_MAX_LOAD 0.75

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
 * @param itemCount number of items in hash table
 * @param table array of hash table items
*/
typedef struct hashTable {
    int size;
    int itemCount;
    hashTableItem *table;
} hashTable;


uint32_t hash(const char* key, size_t length, size_t size);

hashTable* hashTableInit(size_t capacity);

int hashTableInsert(hashTable* htab, const char* key, int data);

hashTableItem* hashTableSearch(hashTable* htab, const char* key);

bool copyHashTable(hashTable* dest, hashTable* src);

bool hashTableResize(hashTable** htab);

void hashTableDeleteItem(hashTable* htab, const char* key);

void hashTableClear(hashTable* htab);

void hashTableFree(hashTable* htab);


#endif // HASH_TABLE_H