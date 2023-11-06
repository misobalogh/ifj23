#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../synt_analysis.h"
#include "colors.h"

void test_expr1() {
    CU_ASSERT_TRUE(precedenceParser());
}

void test_expr2() {
    CU_ASSERT_TRUE(precedenceParser());

}

void test_expr3() {
    CU_ASSERT_TRUE(precedenceParser());

}

void test_expr4() {
    CU_ASSERT_TRUE(precedenceParser());

}

void test_expr5() {
    CU_ASSERT_TRUE(precedenceParser());
}

void test_expr6() {
    CU_ASSERT_TRUE(precedenceParser());
}

void test_expr7() {
    CU_ASSERT_FALSE(precedenceParser());
}


int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite(YELLOW"EXPRESSIONS - SHOULD BE TRUE"RESET, NULL, NULL);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr1);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr2);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr3);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr4);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr5);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_expr6);
    CU_pSuite suite2 = CU_add_suite(YELLOW"EXPRESSIONS - SHOULD BE FALSE"RESET, NULL, NULL);
    CU_add_test(suite2, BLUE"test_stack_init"RESET, test_expr7);
    CU_basic_set_mode(CU_BRM_SILENT);
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
