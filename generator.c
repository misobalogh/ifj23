#include "generator.h"
#include "global_variables.h"
#include "dynamic_string.h"


void setOutputFile(FILE* file) {
  outputFile = file;
}

bool genFunction(const char* idname) {
  fprintf(outputFile, "LABEL function__%s\n", idname);

  return true;
}
