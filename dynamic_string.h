/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */

#ifndef DYNAMIC_STRING_H
#define DYNAMIC_STRING_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// How much should string capacity increase
#define STRING_GROWTH 2

/**
 * @brief Dynamic string that grows and allows safe concatonation
 */
typedef struct String {
  char* data;
  size_t size;
  size_t capacity;
} String;

/**
 * @brief Initilize string. Should be called before calling any other methods
 *
 * @param string String to initilize
 * @param cStr inital value
 */
void stringInit(String* string, const char* cStr);

/**
 * @brief Concatonate two strings. String will be resized of necesarry.
 *
 * @param a String that will be modified
 * @param b String to concatonate to a. Will not be modified
 */
void stringConcat(String* a, const String* const b);

/**
 * @brief Free all string allocated memory.
 *
 * @param string String to free
 */
void stringFree(String* string);

/**
 * @brief Increase capacity of string
 *
 * @param string String to resize
 * @param size New capacity. If size < string.capacity this fuction has no effect.
 */
void stringResize(String* string, size_t size);

/**
 * @brief Get null terminated c-style string
 *
 * @param string String that should be converted to c-style string
 */
const char* stringCStr(String* string);

/**
 * @brief Concatonate String and null-terminated array of chars.
 * String will be resized if necesarry.
 *
 * @param string String to concatonate, will be modified
 * @param Null-terminated array of chars to concatonate to string
 */
void stringConcatCStr(String* string, const char* cStr);

/**
 * @brief Concatonate char to String. String will be resized if necesarry.
 *
 * @param string String that will be modified
 * @param c Char to concatonate to string
 */
void stringConcatChar(String* string, char c);

void stringClear(String* string);

void stringSet(String* string, const char* cStr);

void stringSetS(String* string, String* other);

bool stringEq(String* string, String* other);

#endif
