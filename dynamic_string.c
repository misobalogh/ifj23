/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "dynamic_string.h"

#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>


bool stringInit(String* string, char* cStr) {
  size_t len = strlen(cStr);
  string->capacity = (len + 1) * STRING_GROWTH;
  string->size = len;

  string->data = malloc(string->capacity);
  if (string->data == NULL) {
    return false;
  }

  strcpy(string->data, cStr);
  return true;
}

bool stringResize(String* string, size_t size) {
  if (size <= string->capacity) {
    return true;
  }

  char* ptr = realloc(string->data, size);

  if (ptr == NULL) {
    return false;
  }

  string->data = ptr;
  string->capacity = size;

  return true;
}

bool stringConcat(String* a, String* b) {
  size_t lenA = strlen(a->data);
  size_t lenB = strlen(b->data);

  if (lenA + lenB > a->capacity && !stringResize(a, (lenA + lenB) * STRING_GROWTH)) {
    return false;
  }

  strcat(a->data, b->data);
  return true;
}

void stringFree(String* string) {
  free(string->data);
  string->data = NULL;
  string->size = 0;
  string->capacity = 0;
}

