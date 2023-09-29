/***************************************************************
* Project Name: Implementace překladače imperativního jazyka IFJ23
* File Name: test.c
* Description: Example file of unit test
* Author: MICHAL BALOGH xbalog06
* Faculty: FIT VUT
* Date: 29.09.2023
*
* Comments: Example file that shows how to write unit tests
*           using CUnit library.
*
* Example usage:
* ```
* make
* make runtests - start all unit tests
* ```
***************************************************************/

#include <CUnit/Basic.h>
#include "../sum.h" 
#include "colors.h"

// test case 1
void test_add_POS_INT() {
    CU_ASSERT(sum(2, 3) == 5);
    CU_ASSERT(sum(1, 0) == 1);
}

// test case 2
void test_add_NEG_INT() {
    CU_ASSERT(sum(-2, -3) == -5);
    CU_ASSERT(sum(-2, -1) == -3);
}

// test case 3
void wrong_arg() {
    CU_ASSERT(sum(NULL, -1) == -3);
}


int main() {
    CU_initialize_registry();

    // Create a test suite
    CU_pSuite suite = CU_add_suite("Test Sum", NULL, NULL);

    // Add test cases to the suite
    CU_add_test(suite, "2 positive integers", test_add_POS_INT);
    CU_add_test(suite, "2 negative integers", test_add_NEG_INT);

    // Create second suite
    CU_pSuite suite2 = CU_add_suite("Test Sum2", NULL, NULL);

    // Add test to the second suite
    CU_add_test(suite2, "Wrong arg", wrong_arg);

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
