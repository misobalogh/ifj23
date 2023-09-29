/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: macros.h
* Description: macros for the whole project
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/
#ifndef INPUT_LOADER_H
#define INPUT_LOADER_H

/**
 * @brief Loads input from stdin.
 *
 * Loads input from stdin and returns pointer to it.
 *
 * @return Pointer to input
 */
char* load_input(FILE* input_file);

#endif // INPUT_LOADER_H