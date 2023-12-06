/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: symtable.c
* Description: implementation of hash table for table of symbols
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 5.10.2023

* Comments:

***************************************************************/

#include "symtable.h"


static uint32_t hash(const char* key, size_t length, size_t size);
static bool symtableResize(symtable** tab);


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
symtable* symtableInit(size_t capacity) {
    symtable* tab = (symtable*)malloc(sizeof(symtable));
    CHECK_MEMORY_ALLOC(tab);

    tab->size = capacity;
    tab->itemCount = 0;
    tab->table = malloc(sizeof(symtableItem) * tab->size);
    CHECK_MEMORY_ALLOC(tab->table);

    // Initialize all items in hash table to NULL and data to 0
    for (int i = 0; i < tab->size; i++) {
        tab->table[i].key = NULL;
        tab->table[i].data.paramCount = 0;
        tab->table[i].data.params = NULL;
        tab->table[i].data.dataType = (Type){ 'u', false };
        tab->table[i].data.flags = 0u;
        tab->table[i].data.symbolType = symbol_FN;
        /* memset(&tab->table[i].data, 0, sizeof(SymbolData)); */
        /* tab->table[i].data = 0; */
    }

    return tab;
}


/**
 * @brief Insert an item into the hash table.
 *
 * Insert an item into the hash table on the position given by the hash function. If the position is already occupied,
 * linear probing is used to find an empty slot.
 * When the hash table reaches SYMTABLE_MAX_LOAD capacity, it is automatically resized to double the size.
 * If the key already exists in the hash table, the data is updated.
 * If memory allocation for item fails, exits with INTERNAL_ERROR.
 *
 * @param tab pointer to hash table
 * @param key key to insert
 * @param data data to insert
 *
 * @return 0 if insert fails, 1 if item was already in table and was updated, 2 if item was inserted succesfully
 */
int symtableInsert(symtable* tab, const char* key, SymbolData data) {
    if (tab == NULL) {
        fprintf(stderr, "Error - symtableInsert: invalid pointer, tab is NULL\n");
        return 0;
    }

    // Resize hash table when it reaches SYMTABLE_MAX_LOAD capacity
    if (tab->itemCount >= (tab->size * SYMTABLE_MAX_LOAD)) {
        if (!symtableResize(&tab)) {
            fprintf(stderr, "Error - symtableResize\n");
            return 0;
        }
    }

    uint32_t hashValue = hash(key, strlen(key), tab->size); // Hash the key to get the index of the slot where the item should be inserted

    while (tab->table[hashValue].key != NULL) { // Find an empty slot
        if (strcmp(tab->table[hashValue].key, key) == 0) { // Check if key already exists
            tab->table[hashValue].data = data;  // Update data of existing item
            return 1;
        }
        hashValue++; // Linear probing to find an empty slot
        hashValue %= tab->size;
    }

    // Insert item
    tab->table[hashValue].key = (char*)malloc(sizeof(char) * (strlen(key) + 1));
    CHECK_MEMORY_ALLOC(tab->table[hashValue].key);
    strcpy(tab->table[hashValue].key, key);

    tab->table[hashValue].data = data;
    if (data.paramCount > 0) {
        tab->table[hashValue].data.params = malloc(sizeof(Param) * data.paramCount);
        CHECK_MEMORY_ALLOC(tab->table[hashValue].data.params);
    }
    else {
        tab->table[hashValue].data.params = NULL;
    }

    for (unsigned i = 0; i < data.paramCount; i++) {
        Param* tableParam = &tab->table[hashValue].data.params[i];
        Param* dataParam = &data.params[i];

        *tableParam = *dataParam;
        stringInit(&tableParam->name, stringCStr(&dataParam->name));
        stringInit(&tableParam->label, stringCStr(&dataParam->label));
    }

    /* tab->table[hashValue].type = (char*)malloc(sizeof(char) * (strlen(type) + 1)); */
    /* CHECK_MEMORY_ALLOC(tab->table[hashValue].type); */
    /* strcpy(tab->table[hashValue].type, type); */

    /* tab->table[hashValue].data = data; */
    tab->itemCount++;

    return 2;
}


/**
 * @brief Look up an item in the hash table by key.
 *
 * @param tab pointer to hash table
 * @param key key to search for
 *
 * @return pointer to hash table item if found, NULL otherwise
*/
symtableItem* symtableSearch(symtable* tab, const char* key) {
    // Hash the key, so we know where to look initialy for it in the hash table
    uint32_t hashValue = hash(key, strlen(key), tab->size);

    // Search for the item while we dont find an empty slot
    while (tab->table[hashValue].key != NULL) {
        if (strcmp(tab->table[hashValue].key, key) == 0) {
            return &tab->table[hashValue];
        }
        hashValue++; // Linear probing
        hashValue %= tab->size;
    }

    // Item not found
    return NULL;
}

/**
 * @brief Delete an item from the hash table by key.
 *
 * @param tab pointer to hash table
 * @param key key to delete
 */
 // TOOD: add flag to symtableItem struct to indicate if item is deleted instead of freeing the key


/**
  * @brief Copy hash table items from src to dest
  *
  * @return true if copy was successful, false if dest capacity is smaller than src capacity or if dest or src is NULL
  */
bool copySymtableItems(symtable* dest, symtable* src) {
    if (dest == NULL || src == NULL) {
        fprintf(stderr, "Error - copySymtableItems: invalid pointer, dest or src is NULL\n");
        return false;
    }

    if (dest->size < src->size) {
        fprintf(stderr, "Error - copySymtableItems: dest capacity is smaller than src capacity\n");
        return false;
    }

    for (int i = 0; i < src->size; i++) {
        if (src->table[i].key != NULL) {
            symtableInsert(dest, src->table[i].key, src->table[i].data);
        }
    }

    return true;
}

/**
 * @brief Resize hash table to double the size.
 * Resizing is done by creating a new hash table with double the size and copying all items from the old
 * table to the new table and then deleting the old table. Function is called automatically when the hash table
 * reaches SYMTABLE_MAX_LOAD capacity. This function is not meant to be called manually, therefore it is static.
 *
 * @param tab pointer to hash table
 *
 * @return true if resize was successful, false if memory allocation fails
 */
static bool symtableResize(symtable** tab) {
    // Create a new hash table with double the size
    size_t newCapacity = (*tab)->size * 2;
    symtable* newHtab = symtableInit(newCapacity);

    // Copy all items from the old table to the new table
    if (!copySymtableItems(newHtab, *tab)) {
        symtableFree(newHtab);
        return false;
    }

    // Delete the old table
    for (int i = 0; i < (*tab)->size; i++) {
        if ((*tab)->table[i].key != NULL) {
            free((*tab)->table[i].key);
        }
    }
    free((*tab)->table);

    // Set the new table as the hash table
    (*tab)->size = newHtab->size;
    (*tab)->itemCount = newHtab->itemCount;
    (*tab)->table = newHtab->table;

    // Free the newHtab structure but not its table (it's now owned by *tab)
    free(newHtab);

    return true;
}

/**
 * @brief Clear and free hash table items
 *
 * @param tab pointer to hash table
 */
void symtableClear(symtable* tab) {
    for (int i = 0; i < tab->size; i++) {
        if (tab->table[i].key != NULL) {
            free(tab->table[i].key);
            tab->table[i].key = NULL;

            for (unsigned j = 0; j < tab->table[i].data.paramCount; j++) {
                Param* param = &tab->table[i].data.params[j];
                stringFree(&param->name);
                stringFree(&param->label);
            }

            free(tab->table[i].data.params);
            tab->table[i].data.params = NULL;
        }
    }
    free(tab->table);
}

/**
 * @brief Deallocate hash table sources
 *
 * @param tab pointer to hash table
*/
void symtableFree(symtable* tab) {
    symtableClear(tab);
    if (tab != NULL) {
        free(tab);
    }
}
