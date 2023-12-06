/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#include "dynamic_string.h"
#include "macros.h"

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>


void stringInit(String* string, const char* cStr) {
  size_t len = strlen(cStr);
  string->capacity = (len + 1) * STRING_GROWTH;
  string->size = len;

  string->data = malloc(string->capacity);
  CHECK_MEMORY_ALLOC(string->data);

  strncpy(string->data, cStr, string->capacity);
}

void stringResize(String* string, size_t size) {
  if (size <= string->capacity) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  char* ptr = realloc(string->data, size);

  if (ptr == NULL) {
    EXIT_WITH_MESSAGE(INTERNAL_ERROR);
  }

  string->data = ptr;
  string->capacity = size;
}

void stringConcat(String* string, const String* const other) {
  if (string->size + other->size + 1 >= string->capacity) {
    stringResize(string, (string->size + other->size + 1) * STRING_GROWTH);
  }

  strncpy(string->data + string->size, other->data, other->size);
  string->size += other->size;
}

void stringConcatCStr(String* string, const char* cStr) {
  size_t len = strlen(cStr);

  if (string->size + len + 1 >= string->capacity) {
    stringResize(string, (string->size + len + 1) * STRING_GROWTH);
  }

  strncpy(string->data + string->size, cStr, len);
  string->size += len;
}

void stringFree(String* string) {
  free(string->data);
  string->data = NULL;
  string->size = 0;
  string->capacity = 0;
}

const char* stringCStr(String* string) {
  if (string->size + 1 >= string->capacity) {
    stringResize(string, string->capacity * STRING_GROWTH);
  }

  string->data[string->size] = '\0';
  return string->data;
}

void stringConcatChar(String* string, char c) {
  if (string->size + 1 >= string->capacity) {
      stringResize(string, (string->capacity + 1) * STRING_GROWTH);
  }

  string->data[string->size++] = c;
}

void stringClear(String* string) {
  string->size = 0;
}

void stringSet(String* string, const char* cStr) {
  stringClear(string);
  stringConcatCStr(string, cStr);
}

void stringSetS(String* string, String* other) {
  stringClear(string);
  stringConcat(string, other);
}

bool stringEq(String* string, String* other) {
  return strcmp(stringCStr(string), stringCStr(other)) == 0;
}
