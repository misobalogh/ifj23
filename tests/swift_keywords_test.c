#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdbool.h> 
#include "colors.h"

#include "../swift_keywords.h"

void testIsSwiftType() {
    CU_ASSERT_TRUE(isSwiftType("Int"));
    CU_ASSERT_TRUE(isSwiftType("Double"));
    CU_ASSERT_TRUE(isSwiftType("String"));
    CU_ASSERT_FALSE(isSwiftType("Array"));
    CU_ASSERT_FALSE(isSwiftType("if"));
    CU_ASSERT_FALSE(isSwiftType("MDAASD"));
    CU_ASSERT_FALSE(isSwiftType("Var"));
    CU_ASSERT_FALSE(isSwiftType("var"));
    CU_ASSERT_FALSE(isSwiftType("return"));
    CU_ASSERT_FALSE(isSwiftType("0"));
    CU_ASSERT_FALSE(isSwiftType(""));
}

void testIsSwiftKeyword() {
    CU_ASSERT_TRUE(isSwiftKeyword("else"));
    CU_ASSERT_TRUE(isSwiftKeyword("if"));
    CU_ASSERT_TRUE(isSwiftKeyword("func"));
    CU_ASSERT_TRUE(isSwiftKeyword("let"));
    CU_ASSERT_TRUE(isSwiftKeyword("nil"));
    CU_ASSERT_TRUE(isSwiftKeyword("return"));
    CU_ASSERT_TRUE(isSwiftKeyword("var"));
    CU_ASSERT_TRUE(isSwiftKeyword("while"));
    CU_ASSERT_FALSE(isSwiftKeyword("Int"));
    CU_ASSERT_FALSE(isSwiftKeyword("Double"));
    CU_ASSERT_FALSE(isSwiftKeyword("String"));
    CU_ASSERT_FALSE(isSwiftKeyword("Array"));
    CU_ASSERT_FALSE(isSwiftKeyword("variable"));
    CU_ASSERT_FALSE(isSwiftKeyword("var1"));
    CU_ASSERT_FALSE(isSwiftKeyword(""));
}

void testTypeToString() {
    CU_ASSERT_STRING_EQUAL(swiftTypeToString(SWIFT_INT), "Int");
    CU_ASSERT_STRING_EQUAL(swiftTypeToString(SWIFT_DOUBLE), "Double");
    CU_ASSERT_STRING_EQUAL(swiftTypeToString(SWIFT_STRING), "String");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_INT), "Double");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_INT), "String");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_DOUBLE), "Int");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_DOUBLE), "String");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_STRING), "Int");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_STRING), "Double");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_INT), "Int ");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_INT), " Int");
    CU_ASSERT_STRING_NOT_EQUAL(swiftTypeToString(SWIFT_INT), "int");
}

void testKeywordToString() {
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_ELSE), "else");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_IF), "if");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_FUNC), "func");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_LET), "let");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_NIL), "nil");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_RETURN), "return");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_VAR), "var");
    CU_ASSERT_STRING_EQUAL(swiftKeywordToString(SWIFT_WHILE), "while");
    CU_ASSERT_STRING_NOT_EQUAL(swiftKeywordToString(SWIFT_ELSE), "if");
    CU_ASSERT_STRING_NOT_EQUAL(swiftKeywordToString(SWIFT_ELSE), "func");
    CU_ASSERT_STRING_NOT_EQUAL(swiftKeywordToString(SWIFT_ELSE), "let");
}

int main() {
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }

    CU_pSuite suite = CU_add_suite(YELLOW "Swift Keywords Check Suite" RESET, NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite, BLUE "Test isSwiftType" RESET, testIsSwiftType) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite, BLUE "Test isSwiftKeyword" RESET, testIsSwiftKeyword) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_pSuite suite2 = CU_add_suite(YELLOW "Swift Keywords To String Suite" RESET, NULL, NULL);
    if (suite2 == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, BLUE "Test type to string" RESET, testTypeToString) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (CU_add_test(suite2, BLUE "Test keyword to string" RESET, testKeywordToString) == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }

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
