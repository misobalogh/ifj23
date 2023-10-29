/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "dynamic_string.h"

#include <string.h>
#include <assert.h>
#include <malloc.h>
#include <stdio.h>


bool stringInit(String* string, const char* cStr) {
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

bool stringConcat(String* string, const String* const other) {
  if (string->size + string->size > string->capacity
    && !stringResize(string, (string->size + string->size + 1) * STRING_GROWTH)) {
      return false;
  }

  strcat(string->data, other->data);
  return true;
}

bool stringConcatCStr(String* string, const char* cStr) {
  size_t len = strlen(cStr);

  if (string->size + len <= string->capacity - 1
    && !stringResize(string, (string->size + len + 1) * STRING_GROWTH)) {
      return false;
  }

  strcat(string->data, cStr);
  return true;
}

void stringFree(String* string) {
  free(string->data);
  string->data = NULL;
  string->size = 0;
  string->capacity = 0;
}

const char* stringCStr(String* string) {
  if (string->size <= string->capacity - 1) {
    stringResize(string, string->capacity * STRING_GROWTH);
  }

  string->data[string->size] = '\0';
  return string->data;
}

bool stringConcatChar(String* string, char c) {
  if (string->size + 1 <= string->capacity - 1
      && !stringResize(string, (string->capacity + 1) * STRING_GROWTH)) {
    return false;
  }

  string->data[string->size++] = c;
  return true;
}

