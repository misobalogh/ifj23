#ifndef GENERATOR_H
#define GENERATOR_H

#include "stdbool.h"
#include "stdio.h"

extern FILE* outputFile;

void setOutputFile(FILE* file);
bool genFunction(const char* idname);

#endif
