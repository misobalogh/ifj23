#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../synt_prec_stack.h" 

void test_stack_init() {
    stack s;
    stackInit(&s);
    CU_ASSERT(s.size == 1);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_DOLLAR);
    CU_ASSERT(s.top->lower == NULL);
    stackFreeItems(&s);
}

void test_stack_push() {
    stack s;
    stackInit(&s);

    stackPush(&s, token_DIV);
    CU_ASSERT(s.size == 2);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_DIV);

    stackPush(&s, token_ID);
    CU_ASSERT(s.size == 3);
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
    CU_ASSERT(s.size == 2);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_MUL);

    stackPop(&s);
    CU_ASSERT(s.size == 1);
    CU_ASSERT(s.top != NULL);
    CU_ASSERT(s.top->type == token_DOLLAR);

    stackPop(&s); 
    CU_ASSERT(s.size == 0);
    CU_ASSERT(s.top == NULL);

    stackFreeItems(&s);
}


int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("Stack Test Suite", NULL, NULL);
    CU_add_test(suite, "test_stack_init", test_stack_init);
    CU_add_test(suite, "test_stack_push", test_stack_push);
    CU_add_test(suite, "test_stack_pop", test_stack_pop);

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();

    return CU_get_error();
}
