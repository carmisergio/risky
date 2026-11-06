#include "unity.h"

#include "crypto.h"

void setUp(void) {}

void tearDown(void) {}

void test_hash_1() {
    uint8_t data[] = {0x28, 0xAF, 0xF4, 0xBB, 0x2D, 0x00, 0xFF, 0x91};
    uint32_t hash = hash_sdbm(data, 8);
    TEST_ASSERT_EQUAL_UINT32(0xD0C4D6B3, hash);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_hash_1);
    return UNITY_END();
}
