/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: 
* Description: 
* Author: 
* Faculty: FIT VUT
* Date: 

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
    for(int i = 0; i < htab->size; i++) {
        htab->table[i].key = NULL;
        htab->table[i].data = 1;
    }

    return htab;
}

bool hashTableInsert(hashTable* htab, const char* key, int data) {
    if (htab == NULL) {
        return false;
    }

    // Check if hash table is full
    if(htab->itemCount == htab->size) {
        return false;
    }

    // Hash the key
    uint32_t hashValue = hash(key, strlen(key), htab->size);

    // Find an empty slot
    while(htab->table[hashValue].key != NULL) {
        // Check if key already exists
        if(strcmp(htab->table[hashValue].key, key) == 0) {
            return false;
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

    return true;
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
    while(htab->table[hashValue].key != NULL) {
        if(strcmp(htab->table[hashValue].key, key) == 0) {
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
    hashTableItem *itemToDelete = hashTableSearch(htab, key);
    itemToDelete->key = "TOMBSTONE"; // TOOD: maybe add flag to hashTableItem struct to indicate if item is deleted instead?
    itemToDelete->data = 0;
}


bool copyHashTable(hashTable* dest, hashTable* src) {
    if (dest == NULL || src == NULL) {
        printf("dest or src is NULL\n");
        return false;
    }

    if (dest->size < src->size) {
        printf("dest size is smaller than src size\n");
        return false;
    }

    for (int i = 0; i < src->size; i++) {
        // char *item = src->table[i].key;
        if (src->table[i].key != NULL) {
            dest->table[i].key = (char*)malloc(sizeof(char) * (strlen(src->table[i].key) + 1));
            CHECK_MEMORY_ALLOC(dest->table[i].key);
            strcpy(dest->table[i].key, src->table[i].key);
            dest->table[i].data = src->table[i].data;
        }
    }

    return true;
}


bool hashTableResize(hashTable* htab) {
    // Create a new hash table with double the size
    size_t newCapacity = htab->size * 2;
    hashTable* newHtab = hashTableInit(newCapacity);

    // Copy all items from the old table to the new table
    if(!copyHashTable(newHtab, htab)) {
        printf("Error - copyHashTable\n"); 
        return false;
    }

    // Delete the old table
    hashTableFree(htab);
    // Set the new table as the hash table
    htab = newHtab;

    return true;
}

/**
 * @brief Clear and free hash table items
 * 
 * @param htab pointer to hash table
 */
void hashTableClear(hashTable* htab) {
    for(int i = 0; i < htab->size; i++) {
        if(htab->table[i].key != NULL) {
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