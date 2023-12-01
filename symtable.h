/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name:    symtable.h
* Description:  Header file for hash table for storing identifiers
* Author:       Michal Balogh, xbalog06
* Faculty:      FIT VUT
* Date:         10.10.2023

* Comments:

***************************************************************/

#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dynamic_string.h"
#include "expr.h"
#include "macros.h"

#define symbol_flag_VARIADIC 1
#define symbol_flag_BUILTIN 2
#define symbol_flag_INITIALIZED 4

/* Prime number for size of hash table */
#define SYMTABLE_SIZE 257   

/* Max load factor for hash table */
#define SYMTABLE_MAX_LOAD 0.75

typedef struct Param {
  String label;
  String name;
  Type type;
  int intVal;
  float floatVal;
  bool isConst;
} Param;

typedef enum SymbolType { symbol_VAR, symbol_LET, symbol_FN } SymbolType;

typedef struct SymbolData {
  Type dataType;
  Param* params;
  unsigned paramCount;
  unsigned flags;
  SymbolType symbolType;
} SymbolData;

/**
 * @brief struct for hash table item
 * 
 * @param key name of variable
 * @param type  type of variable, for functions it is first letter of return type,

 * @param data value of variable, for functions it is number of parameters
 */
typedef struct symtableItem {
    char* key;
    SymbolData data;
} symtableItem;


/**
 * @brief struct for hash table
 * 
 * @param size size of hash table
 * @param itemCount number of items in hash table
 * @param table array of hash table items
*/
typedef struct symtable {
    int size;
    int itemCount;
    symtableItem *table;
} symtable;


symtable* symtableInit(size_t capacity);

int symtableInsert(symtable* tab, const char* key, SymbolData data);

symtableItem* symtableSearch(symtable* tab, const char* key);

bool copySymtableItems(symtable* dest, symtable* src);

void symtableDeleteItem(symtable* tab, const char* key);

void symtableClear(symtable* tab);

void symtableFree(symtable* tab);


#endif // SYMTABLE_H
