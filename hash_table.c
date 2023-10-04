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
uint32_t hash(const char* key, int length) {
    // Initialize the hash value to a constant prime number.
    uint32_t hash = 2166136261u;

    // Iterate through each character in the string.
    for (int i = 0; i < length; i++) {
        // XOR the current character with the current hash value.
        // This mixes the bits of the character into the hash.
        hash ^= (uint8_t)key[i];

        // Multiply the hash by another prime number to further mix the bits.
        // This helps prevent hash collisions and improves the distribution of hash values.
        hash *= 16777619;
    }

    return hash % HASH_TABLE_SIZE;
}


/**
 * @brief initialize hash table
 * 
 * Initialize hash table with size HASH_TABLE_SIZE, if memory allocation fails
 * exits with INTERNAL_ERROR. All items in hash table are set to NULL and data are set to 0.
 * 
 * @return pointer to initialized hash table
*/
hashTable* hashTableInit() {
    hashTable* htab = (hashTable*)malloc(sizeof(hashTable));
    CHECK_MEMORY_ALLOC(htab);

    htab->size = HASH_TABLE_SIZE;
    htab->count = 0;
    htab->table = (hashTableItem*)malloc(sizeof(hashTableItem) * htab->size);
    CHECK_MEMORY_ALLOC(htab->table);

    // Initialize all items in hash table to NULL and data to 0
    for(int i = 0; i < htab->size; i++) {
        htab->table[i].key = NULL;
        htab->table[i].data = 0;
    }

    return htab;
}

bool hashTableInsert(hashTable* htab, const char* key, int data) {
    // Check if hash table is full
    if(htab->count == htab->size) {
        return false;
    }

    // Hash the key
    uint32_t hashValue = hash(key, strlen(key));

    // Find an empty slot
    while(htab->table[hashValue].key != NULL) {
        // Check if key already exists
        if(strcmp(htab->table[hashValue].key, key) == 0) {
            return false;
        }

        // Linear probing
        hashValue++;
        hashValue %= htab->size;
    }

    // Insert item
    htab->table[hashValue].key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
    CHECK_MEMORY_ALLOC(htab->table[hashValue].key);
    strcpy(htab->table[hashValue].key, key);
    htab->table[hashValue].data = data;
    htab->count++;

    return true;
}