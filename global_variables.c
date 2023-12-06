/********************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23  *
* File Name: global_variables.c                                     *
* Description: Table of symbols                                     *
* Author:  Michal Balogh xbalog06, Michal Cenek xcenek04            *
* Faculty: FIT VUT                                                  *
* Date:    10.10.2023                                               *
*                                                                   *
* Comments: operator precedence is in inverse order as in table,    *
*           so the higher the number, the higher the precedence     *
*                                                                   *
********************************************************************/

#include "global_variables.h"



/**
 * @brief Global table of symbols
*/
symtable* global_table;
SymtableStack* global_symtableStack;

/**
 * @brief Initialize global table of symbols
 */
void global_initSymtable(void) {
  global_table = symtableInit(SYMTABLE_SIZE);
}

void global_freeSymtable(void) {
  symtableFree(global_table);
}

void global_initSymtableStack(void) {
  global_symtableStack = malloc(sizeof(SymtableStack));
  if (global_symtableStack == NULL) {
    exit(99);
  }
  symtableStackInit(global_symtableStack);
}

void global_freeSymtableStack(void) {
  SymtableStackItem* current = global_symtableStack->first;

  while (current != NULL) {
    SymtableStackItem* next = current->next;
    free(current);
    current = next;
  }

  free(global_symtableStack);
  global_symtableStack = NULL;
}

const char* errorToString(error_codes err) {
  switch (err) {
  case SUCCESS:
    return "Program is syntactically and semantically correct";
  case LEX_ANALYSIS_ERR:
    return "Lexical error";
  case SYNTAX_ANALYSIS_ERR:
    return "Syntax error";
  case UNDEFINED_FN:
    return "Semantic error (undefined function or variable redefinition)";
  case WRONG_PARAM:
    return "Semantic error (function call)";
  case UNDEFINED_VAR:
    return "Semantic error (use of undefined or unitialized variable)";
  case RETURN_EXPRESSION_ERR:
    return "Semantic error (missing or excess expression in return statement)";
  case TYPE_COMPATIBILITY_ERR:
    return "Semantic error (type compatibility)";
  case TYPE_INFERENCE_ERR:
    return "Semantic error (type cannot be infered)";
  case SEMANTIC_ERR:
    return "Semantci error";
  case INTERNAL_ERROR:
    return "Internal error";
  }

  return "Undefined error code";
}


/**
 * generate instruction for interpret from global variable
 */
void global_generateInstruction();

symtableItem* global_symbolSearch(const char* key, unsigned* out_id) {
  symtableItem* result = symtableStackSearch(global_symtableStack, key, out_id);
  if (result != NULL) {
    return result;
  }

  if (out_id != NULL) {
      *out_id = 0;
  }

  return symtableSearch(global_table, key);
}

void global_insertTop(const char* key, SymbolData data) {
  if (global_symtableStack == NULL || global_table == NULL) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  symtable* table = global_table;
  if (global_symtableStack->first != NULL) {
    table = global_symtableStack->first->table;
  }

  symtableInsert(table, key, data);
}

symtableItem* global_searchTop(const char* key) {
  if (global_symtableStack == NULL || global_table == NULL) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  symtable* table = global_table;
  if (global_symtableStack->first != NULL) {
    table = global_symtableStack->first->table;
  }

  return symtableSearch(table, key);
}

bool global_isLocal(const char* key) {
    return symtableStackIsLocal(global_symtableStack, key);
}
