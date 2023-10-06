/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: macros.h
* Description: macros for the whole project
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "macros.h"
#include "load_input.h"
#include "error_codes.h"

#define BUFF_SIZE 1024

char* load_input(FILE* input_file) {
    if(input_file == NULL) {
        fprintf(stderr, "ERR_CODE: %d, input file is NULL\n", INTERNAL_ERROR);
        exit(INTERNAL_ERROR);
    };

    char* buffer = NULL;
    size_t buffer_size = BUFF_SIZE;
    size_t lenght = 0;
    int c;   

    buffer = (char*) malloc(buffer_size * sizeof(char));
    CHECK_MEMORY_ALLOC(buffer);

    while((c=fgetc(input_file)) != EOF) {
        if(lenght+1 >= buffer_size) {
            buffer_size *= 2;
            buffer = (char*) realloc(buffer, buffer_size * sizeof(char));
            CHECK_MEMORY_ALLOC(buffer);
        }
        buffer[lenght++] = (char)c;
    }

    buffer[lenght] = '\0';

    return buffer;
}