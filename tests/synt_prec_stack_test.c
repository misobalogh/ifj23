#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../synt_prec_stack.h" 
#include "colors.h"

void test_stack_init() {
    stack s;
    stackInit(&s);
    CU_ASSERT(s.size == 0);
    CU_ASSERT(s.top == NULL);
    stackFreeItems(&s);
}

void test_stack_push() {
    stack s;
    stackInit(&s);

    stackPush(&s, token_DIV);
    CU_ASSERT(s.size == 1);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_DIV);

    stackPush(&s, token_ID);
    CU_ASSERT(s.size == 2);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_ID);

    stackFreeItems(&s);
}

void test_stack_pop() {
    stack s;
    stackInit(&s);
    stackPush(&s, token_MUL);
    stackPush(&s, token_EQ);

    stackPop(&s);
    CU_ASSERT(s.size == 1);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_MUL);

    stackPop(&s);
    CU_ASSERT(s.size == 0);
    CU_ASSERT(s.top == NULL);

    stackPop(&s);
    CU_ASSERT(s.size == 0);
    CU_ASSERT(s.top == NULL);

    stackFreeItems(&s);
}


int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite(YELLOW"Stack Test Suite"RESET, NULL, NULL);
    CU_add_test(suite, BLUE"test_stack_init"RESET, test_stack_init);
    CU_add_test(suite, BLUE"test_stack_push"RESET, test_stack_push);
    CU_add_test(suite, BLUE"test_stack_pop"RESET, test_stack_pop);

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
