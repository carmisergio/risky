#include "unity.h"

// Funcitons under test
extern uint64_t fib(uint64_t n);

void setUp(void) {}

void tearDown(void) {}

void test_fib_0() { TEST_ASSERT_EQUAL_UINT64(0, fib(0)); }
void test_fib_1() { TEST_ASSERT_EQUAL_UINT64(1, fib(1)); }
void test_fib_2() { TEST_ASSERT_EQUAL_UINT64(1, fib(2)); }
void test_fib_3() { TEST_ASSERT_EQUAL_UINT64(2, fib(3)); }
void test_fib_4() { TEST_ASSERT_EQUAL_UINT64(3, fib(4)); }
void test_fib_10() { TEST_ASSERT_EQUAL_UINT64(55, fib(10)); }

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_fib_0);
    RUN_TEST(test_fib_1);
    RUN_TEST(test_fib_2);
    RUN_TEST(test_fib_3);
    RUN_TEST(test_fib_4);
    RUN_TEST(test_fib_10);
    return UNITY_END();
}
