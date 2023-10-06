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


static uint32_t hash(const char* key, size_t length, size_t size);
static bool hashTableResize(hashTable** htab);


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
static uint32_t hash(const char* key, size_t length, size_t size) {
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
    htab->table = malloc(sizeof(hashTableItem) * htab->size);
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
 * @param htab pointer to hash table
 * @param key key to insert
 * @param data data to insert
 *
 * @return 0 if insert fails, 1 if item was already in table and was updated, 2 if item was inserted succesfully
 */
int hashTableInsert(hashTable* htab, const char* key, int data) {
    if (htab == NULL) {
        fprintf(stderr, "Error - hashTableInsert: invalid pointer, htab is NULL\n");
        return 0;
    }

    // Resize hash table when it reaches HASH_TABLE_MAX_LOAD capacity
    if (htab->itemCount >= (htab->size * HASH_TABLE_MAX_LOAD)) {
        if (!hashTableResize(&htab)) {
            fprintf(stderr, "Error - hashTableResize\n");
            return 0;
        }
    }

    uint32_t hashValue = hash(key, strlen(key), htab->size); // Hash the key to get the index of the slot where the item should be inserted

    while (htab->table[hashValue].key != NULL) { // Find an empty slot
        if (strcmp(htab->table[hashValue].key, key) == 0) { // Check if key already exists
            htab->table[hashValue].data = data;  // Update data of existing item
            return 1; 
        }        
        hashValue++; // Linear probing to find an empty slot
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
    // Hash the key, so we know where to look initialy for it in the hash table
    uint32_t hashValue = hash(key, strlen(key), htab->size);

    // Search for the item while we dont find an empty slot
    while (htab->table[hashValue].key != NULL) {
        if (strcmp(htab->table[hashValue].key, key) == 0) {
            return &htab->table[hashValue];
        }         
        hashValue++; // Linear probing
        hashValue %= htab->size;
    }

    // Item not found
    return NULL;
}

//TODO: implement hashTableDeleteItem
/**
 * @brief Delete an item from the hash table by key. 
 *
 * @param htab pointer to hash table
 * @param key key to delete
 */
// TOOD: add flag to hashTableItem struct to indicate if item is deleted instead of freeing the key


/**
 * @brief Copy hash table items from src to dest
 * 
 * @return true if copy was successful, false if dest capacity is smaller than src capacity or if dest or src is NULL
 */
bool copyHashTableItems(hashTable* dest, hashTable* src) {
    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Error - copyHashTableItems: invalid pointer, dest or src is NULL\n");
        return false;
    }

    if (dest->size < src->size) {
        fprintf(stderr, "Error - copyHashTableItems: dest capacity is smaller than src capacity\n");
        return false;
    }

    for (int i = 0; i < src->size; i++) {
        if (src->table[i].key != NULL) {
            hashTableInsert(dest, src->table[i].key, src->table[i].data);
        }
    }

    return true;
}

/**
 * @brief Resize hash table to double the size.
 * Resizing is done by creating a new hash table with double the size and copying all items from the old 
 * table to the new table and then deleting the old table. Function is called automatically when the hash table
 * reaches HASH_TABLE_MAX_LOAD capacity. This function is not meant to be called manually, therefore it is static.
 *  
 * @param htab pointer to hash table
 * 
 * @return true if resize was successful, false if memory allocation fails
 */
static bool hashTableResize(hashTable** htab) {
    // Create a new hash table with double the size
    size_t newCapacity = (*htab)->size * 2;
    hashTable* newHtab = hashTableInit(newCapacity);

    // Copy all items from the old table to the new table
    if (!copyHashTableItems(newHtab, *htab)) {
        hashTableFree(newHtab);
        return false;
    }

    // Delete the old table
    for (int i = 0; i < (*htab)->size; i++) {
        if ((*htab)->table[i].key != NULL) {
            free((*htab)->table[i].key);
        }
    }
    free((*htab)->table);

    // Set the new table as the hash table
    (*htab)->size = newHtab->size;
    (*htab)->itemCount = newHtab->itemCount;
    (*htab)->table = newHtab->table;

    // Free the newHtab structure but not its table (it's now owned by *htab)
    free(newHtab);

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
    if (htab != NULL) {
        free(htab);
    }
}