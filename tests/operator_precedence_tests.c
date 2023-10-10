#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h> 
#include "colors.h"

#include "../operator_precedence.h"



void test_PRECEDENCE_OP_NOT(void) {
    const char* operator = "!";
    CU_ASSERT_EQUAL(PRECEDENCE(operator), OP_NOT);
}

void test_PRECEDENCE_OP_MUL_DIV(void) {
    const char* operator = "*";
    CU_ASSERT_EQUAL(PRECEDENCE(operator), OP_MUL_DIV);
    const char* operator2 = "/";
    CU_ASSERT_EQUAL(PRECEDENCE(operator2), OP_MUL_DIV);
}

void test_PRECEDENCE_OP_PLUS_MINUS(void) {
    const char* operator = "+";
    CU_ASSERT_EQUAL(PRECEDENCE(operator), OP_PLUS_MINUS);
    const char* operator2 = "-";
    CU_ASSERT_EQUAL(PRECEDENCE(operator2), OP_PLUS_MINUS);
}

void test_PRECEDENCE_OP_REL(void) {
    const char* operator = "==";
    CU_ASSERT_EQUAL(PRECEDENCE(operator), OP_REL);
    const char* operator2 = "!=";
    CU_ASSERT_EQUAL(PRECEDENCE(operator2), OP_REL);
    const char* operator3 = "<";
    CU_ASSERT_EQUAL(PRECEDENCE(operator3), OP_REL);
    const char* operator4 = ">";
    CU_ASSERT_EQUAL(PRECEDENCE(operator4), OP_REL);
    const char* operator5 = "<=";
    CU_ASSERT_EQUAL(PRECEDENCE(operator5), OP_REL);
    const char* operator6 = ">=";
    CU_ASSERT_EQUAL(PRECEDENCE(operator6), OP_REL);
}

void test_PRECEDENCE_OP_CONCAT(void) {
    const char* operator = "??";
    CU_ASSERT_EQUAL(PRECEDENCE(operator), OP_CONCAT);
}

void test_operator_precedence(void) {
    const char* operator1 = "!";
    const char* operator2 = "*";
    CU_ASSERT_TRUE(PRECEDENCE(operator1) > PRECEDENCE(operator2));
    const char* operator3 = "/";
    const char* operator4 = "+";
    CU_ASSERT_TRUE(PRECEDENCE(operator3) > PRECEDENCE(operator4));
    const char* operator5 = "-";
    const char* operator6 = "==";
    CU_ASSERT_TRUE(PRECEDENCE(operator5) > PRECEDENCE(operator6));
    const char* operator7 = ">=";
    const char* operator8 = "??";
    CU_ASSERT_TRUE(PRECEDENCE(operator7) > PRECEDENCE(operator8));

    CU_ASSERT_EQUAL(PRECEDENCE(operator2), PRECEDENCE(operator3));
    CU_ASSERT_EQUAL(PRECEDENCE(operator4), PRECEDENCE(operator5));
    CU_ASSERT_EQUAL(PRECEDENCE(operator6), PRECEDENCE(operator7));
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite(YELLOW "Swift operator precedence" RESET, NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(suite, BLUE "Test PRECEDENCE macro for ! (OP_NOT)" RESET, test_PRECEDENCE_OP_NOT);
    CU_add_test(suite, BLUE "Test PRECEDENCE macro for * (OP_MUL_DIV)" RESET, test_PRECEDENCE_OP_MUL_DIV);
    CU_add_test(suite, BLUE "Test PRECEDENCE macro for + (OP_PLUS_MINUS)" RESET, test_PRECEDENCE_OP_PLUS_MINUS);
    CU_add_test(suite, BLUE "Test PRECEDENCE macro for == (OP_REL)" RESET, test_PRECEDENCE_OP_REL);
    CU_add_test(suite, BLUE "Test PRECEDENCE macro for ?? (OP_CONCAT)" RESET, test_PRECEDENCE_OP_CONCAT);
    CU_add_test(suite, BLUE "Test operator precedence" RESET, test_operator_precedence);
    CU_basic_set_mode(CU_BRM_VERBOSE);

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
