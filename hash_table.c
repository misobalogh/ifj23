/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: hash_table.c
* Description: implementation of hash table for table of symbols
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 5.10.2023

* Comments:

***************************************************************/

#include "hash_table.h"

/**
 * @brief Hashing function for strings.
 *
 * The following hashing function was inspired by
 * "Crafting Interpreters" book by Robert Nystrom.
 * Book URL: https://craftinginterpreters.com/
 *
 * This hashing function is using the FNV-1a hash algorithm which is
 * designed to be fast while maintaining a low collision rate.
 *
 * @param key The string to hash.
 * @param length The length of the string.
 *
 * @return The hash value of the string.
*/
uint32_t hash(const char* key, size_t length, size_t size) {
    // Initialize the hash value to a constant prime number.
    uint32_t hash = 2166136261u;

    // Iterate through each character in the string.
    for (size_t character = 0; character < length; character++) {
        // XOR the current character with the current hash value.
        // This mixes the bits of the character into the hash.
        hash ^= (uint8_t)key[character];

        // Multiply the hash by another prime number to further mix the bits.
        // This helps prevent hash collisions and improves the distribution of hash values.
        hash *= 16777619;
    }
    // Return the final hash value that fits in the hash table.
    return hash % size;
}


/**
 * @brief initialize hash table
 *
 * Initialize hash table with size HASH_TABLE_SIZE, if memory allocation fails
 * exits with INTERNAL_ERROR. All items in hash table are set to NULL and data are set to 0.
 *
 * @return pointer to initialized hash table
*/
hashTable* hashTableInit(size_t capacity) {
    hashTable* htab = (hashTable*)malloc(sizeof(hashTable));
    CHECK_MEMORY_ALLOC(htab);

    htab->size = capacity;
    htab->itemCount = 0;
    htab->table = (hashTableItem*)malloc(sizeof(hashTableItem) * htab->size);
    CHECK_MEMORY_ALLOC(htab->table);

    // Initialize all items in hash table to NULL and data to 0
    for (int i = 0; i < htab->size; i++) {
        htab->table[i].key = NULL;
        htab->table[i].data = 0;
    }

    return htab;
}


/**
 * @brief Insert an item into the hash table.
 *
 * Insert an item into the hash table on the position given by the hash function. If the position is already occupied,
 * linear probing is used to find an empty slot.
 * When the hash table reaches HASH_TABLE_MAX_LOAD capacity, it is automatically resized to double the size.
 * If the key already exists in the hash table, the data is updated.
 * If memory allocation for item fails, exits with INTERNAL_ERROR.
 *
 */
int hashTableInsert(hashTable* htab, const char* key, int data) {
    if (htab == NULL) {
        return 0;
    }

    // Resize hash table if it's full
    if (htab->itemCount >= (htab->size * HASH_TABLE_MAX_LOAD)) {
        if (!hashTableResize(&htab)) {
            fprintf(stderr, "Error - hashTableResize\n");
            return 0;
        }
        // printf("resized\n");
        // printf("size: %d\n", htab->size);
        // hashTableItem* item;
        // for (int i = 0; i < 202; i++) {
        //     char* key = (char*)malloc(sizeof(char) * 10);
        //     sprintf(key, "key%d", i);
        //     if ((item = hashTableSearch(htab, key))) {
        //         printf("Key: %s, Data: %d\n", item->key, item->data);
        //     }
        //     free(key);
        // }
    }

    // Check if hash table is full
    // if (htab->itemCount == htab->size) {
    //     return 1;
    // }

    // Hash the 
    uint32_t hashValue = hash(key, strlen(key), htab->size);
    // Find an empty slot
    while (htab->table[hashValue].key != NULL) {
        // Check if key already exists
        if (strcmp(htab->table[hashValue].key, key) == 0) {
            // Update data of existing item
            htab->table[hashValue].data = data;
            return 1;
        }

        // Linear probing to find an empty slot
        hashValue++;
        hashValue %= htab->size;
    }

    // Insert item
    htab->table[hashValue].key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
    CHECK_MEMORY_ALLOC(htab->table[hashValue].key);
    strcpy(htab->table[hashValue].key, key);
    htab->table[hashValue].data = data;
    htab->itemCount++;

    return 2;
}


/**
 * @brief Look up an item in the hash table by key.
 *
 * @param htab pointer to hash table
 * @param key key to search for
 *
 * @return pointer to hash table item if found, NULL otherwise
*/
hashTableItem* hashTableSearch(hashTable* htab, const char* key) {
    // Hash the key, so we know where to look for it in the hash table
    uint32_t hashValue = hash(key, strlen(key), htab->size);

    // Search for the item while we don't find an empty slot
    while (htab->table[hashValue].key != NULL) {
        if (strcmp(htab->table[hashValue].key, key) == 0) {
            return &htab->table[hashValue];
        }

        // Linear probing 
        hashValue++;
        hashValue %= htab->size;
    }

    // Item not found
    return NULL;
}

void hashTableDeleteItem(hashTable* htab, const char* key) {
    hashTableItem* itemToDelete = hashTableSearch(htab, key);
    itemToDelete->key = "TOMBSTONE"; // TOOD: maybe add flag to hashTableItem struct to indicate if item is deleted instead?
    itemToDelete->data = 0;
}


bool copyHashTable(hashTable* dest, hashTable* src) {
    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Error - copyHashTable: dest or src is NULL\n");
        return false;
    }

    if (dest->size < src->size) {
        fprintf(stderr, "Error - copyHashTable: dest capacity is smaller than src capacity\n");
        return false;
    }

    for (int i = 0; i < src->size; i++) {
        if (src->table[i].key != NULL) {
            hashTableInsert(dest, src->table[i].key, src->table[i].data);
        }
    }

    return true;
}



bool hashTableResize(hashTable** htab) {
    // Create a new hash table with double the size
    size_t newCapacity = (*htab)->size * 2;
    hashTable* newHtab = hashTableInit(newCapacity);

    // Copy all items from the old table to the new table
    if (!copyHashTable(newHtab, *htab)) {
        hashTableFree(newHtab);
        return false;
    }

    // Delete the old table
    hashTableFree(*htab);
    // Set the new table as the hash table
    *htab = newHtab;

    return true;
}

/**
 * @brief Clear and free hash table items
 *
 * @param htab pointer to hash table
 */
void hashTableClear(hashTable* htab) {
    for (int i = 0; i < htab->size; i++) {
        if (htab->table[i].key != NULL) {
            free(htab->table[i].key);
            htab->table[i].key = NULL;
            htab->table[i].data = 0;
        }
    }
    free(htab->table);
}

/**
 * @brief Deallocate hash table sources
 *
 * @param htab pointer to hash table
*/
void hashTableFree(hashTable* htab) {
    hashTableClear(htab);
    free(htab);
}