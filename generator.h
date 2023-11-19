/*
 * Implementace překladače imperativního jazyka IFJ23
 * Michal Cenek xcenek04
 */


#ifndef GENERATOR_H
#define GENERATOR_H

#include "stdbool.h"
#include "stdio.h"

void setOutputFile(FILE* file);
bool genFunction(const char* idname);

#endif
