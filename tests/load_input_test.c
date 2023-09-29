/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: load_input_test.c
* Description: unit tests for load_input.c
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023

* Comments:

***************************************************************/

#include <CUnit/Basic.h>
#include "../load_input.h" 
#include "colors.h"
#include <stdlib.h>
#include <stdio.h>
#include "../error_codes.h"

FILE* mock_input_file = NULL;
FILE* input_file = NULL;

int init_suite(void) {
    // Open a temporary file for mocking getchar
    mock_input_file = tmpfile();
    if (mock_input_file == NULL) {
        return -1;
    }
    return 0;
}

int clean_suite(void) {
    fclose(mock_input_file);
    return 0;
}

// Test case for load_input function
void test_load_input(void) {
    init_suite();
    // Mock input data by writing to the temporary file
    const char* test_input = "Hello, World!\n";
    fputs(test_input, mock_input_file);
    // Rewind the file pointer to the beginning of the file
    rewind(mock_input_file);
    // Redirect getchar to read from the temporary file
    freopen(NULL, "r", mock_input_file);

    char* result = load_input(mock_input_file);

    CU_ASSERT_STRING_EQUAL(result, test_input);

    free(result);
    clean_suite();
}

// Test case for empty input
void test_load_input_empty(void) {
    init_suite();
    rewind(mock_input_file);

    char* result = load_input(mock_input_file);

    CU_ASSERT_STRING_EQUAL(result, "");

    free(result);
    clean_suite();
}


int main() {
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // Create a test suite
    CU_pSuite suite = CU_add_suite("load_input_OK", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (CU_add_test(suite, "test_load_input", test_load_input) == NULL ||
        CU_add_test(suite, "test_load_input_empty", test_load_input_empty) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);

    // Run the tests
    CU_basic_run_tests();
    if (CU_get_number_of_failures() == 0) {
        printf(GREEN "All tests passed\n" RESET);
    }
    else {
        printf(RED "%d Tests failed\n" RESET, CU_get_number_of_failures());
    }

    CU_cleanup_registry();

    return CU_get_error();
}
