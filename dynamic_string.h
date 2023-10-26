#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdbool.h>
#include <stdint.h>

#define STRING_GROWTH 2

typedef struct String {
  char* data;
  size_t size;
  size_t capacity;
} String;

bool stringInit(String* string, char* cStr);
bool stringConcat(String* a, String* b);
void stringFree(String* string);
bool stringResize(String* string, size_t size);

#endif
