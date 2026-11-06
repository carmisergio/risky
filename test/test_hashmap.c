#include "results.h"
#include "unity.h"

#include <hashmap.h>
#include <stdint.h>

uint8_t test_hashmap_2[] = {
    33,  0,   0,   0,   0,   0,   0,   0,   40,  0,   0,   0,   5,   0,   0,
    0,   20,  0,   0,   0,   0,   0,   0,   0,   45,  0,   0,   0,   5,   0,
    0,   0,   22,  0,   0,   0,   0,   0,   0,   0,   50,  0,   0,   0,   5,
    0,   0,   0,   24,  0,   0,   0,   0,   0,   0,   0,   55,  0,   0,   0,
    5,   0,   0,   0,   26,  0,   0,   0,   0,   0,   0,   0,   60,  0,   0,
    0,   5,   0,   0,   0,   28,  0,   0,   0,   0,   0,   0,   0,   65,  0,
    0,   0,   5,   0,   0,   0,   30,  0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   4,   0,
    0,   0,   2,   0,   0,   0,   0,   0,   0,   0,   8,   0,   0,   0,   4,
    0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   0,   12,  0,   0,   0,
    4,   0,   0,   0,   6,   0,   0,   0,   0,   0,   0,   0,   16,  0,   0,
    0,   4,   0,   0,   0,   8,   0,   0,   0,   0,   0,   0,   0,   20,  0,
    0,   0,   4,   0,   0,   0,   10,  0,   0,   0,   0,   0,   0,   0,   24,
    0,   0,   0,   4,   0,   0,   0,   12,  0,   0,   0,   0,   0,   0,   0,
    28,  0,   0,   0,   4,   0,   0,   0,   14,  0,   0,   0,   0,   0,   0,
    0,   32,  0,   0,   0,   4,   0,   0,   0,   16,  0,   0,   0,   0,   0,
    0,   0,   36,  0,   0,   0,   4,   0,   0,   0,   18,  0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   107, 101, 121, 48,
    107, 101, 121, 49,  107, 101, 121, 50,  107, 101, 121, 51,  107, 101, 121,
    52,  107, 101, 121, 53,  107, 101, 121, 54,  107, 101, 121, 55,  107, 101,
    121, 56,  107, 101, 121, 57,  107, 101, 121, 49,  48,  107, 101, 121, 49,
    49,  107, 101, 121, 49,  50,  107, 101, 121, 49,  51,  107, 101, 121, 49,
    52,  107, 101, 121, 49,  53};

uint8_t test_bidir_map_2[] = {
    16,  0,   0,   0,   33,  0,   0,   0,   40,  0,   0,   0,   0,   0,   0,
    0,   5,   0,   0,   0,   10,  0,   0,   0,   45,  0,   0,   0,   0,   0,
    0,   0,   5,   0,   0,   0,   11,  0,   0,   0,   50,  0,   0,   0,   0,
    0,   0,   0,   5,   0,   0,   0,   12,  0,   0,   0,   55,  0,   0,   0,
    0,   0,   0,   0,   5,   0,   0,   0,   13,  0,   0,   0,   60,  0,   0,
    0,   0,   0,   0,   0,   5,   0,   0,   0,   14,  0,   0,   0,   65,  0,
    0,   0,   0,   0,   0,   0,   5,   0,   0,   0,   15,  0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   4,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,
    0,   0,   4,   0,   0,   0,   1,   0,   0,   0,   8,   0,   0,   0,   0,
    0,   0,   0,   4,   0,   0,   0,   2,   0,   0,   0,   12,  0,   0,   0,
    0,   0,   0,   0,   4,   0,   0,   0,   3,   0,   0,   0,   16,  0,   0,
    0,   0,   0,   0,   0,   4,   0,   0,   0,   4,   0,   0,   0,   20,  0,
    0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,   24,
    0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   6,   0,   0,   0,
    28,  0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   7,   0,   0,
    0,   32,  0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   8,   0,
    0,   0,   36,  0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   9,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   15,  0,   0,   0,
    16,  0,   0,   0,   17,  0,   0,   0,   18,  0,   0,   0,   19,  0,   0,
    0,   20,  0,   0,   0,   21,  0,   0,   0,   22,  0,   0,   0,   23,  0,
    0,   0,   24,  0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   2,
    0,   0,   0,   3,   0,   0,   0,   4,   0,   0,   0,   5,   0,   0,   0,
    107, 101, 121, 48,  107, 101, 121, 49,  107, 101, 121, 50,  107, 101, 121,
    51,  107, 101, 121, 52,  107, 101, 121, 53,  107, 101, 121, 54,  107, 101,
    121, 55,  107, 101, 121, 56,  107, 101, 121, 57,  107, 101, 121, 49,  48,
    107, 101, 121, 49,  49,  107, 101, 121, 49,  50,  107, 101, 121, 49,  51,
    107, 101, 121, 49,  52,  107, 101, 121, 49,  53};

void setUp(void) {}

void tearDown(void) {}

void test_simple_validity_1() {
    uint64_t max = sizeof(test_hashmap_2) / sizeof(uint8_t);
    uint64_t res = simple_bytes_u64_hashmap_validate(test_hashmap_2, max);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_OK, res);
}

void test_simple_validity_2() {
    uint64_t max = sizeof(test_hashmap_2) / sizeof(uint8_t);
    uint64_t res = simple_bytes_u64_hashmap_validate(test_hashmap_2, max - 1);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_MALFORMED, res);
}

void test_simple_lookup_1() {
    result res =
        simple_bytes_u64_hashmap_lookup(test_hashmap_2, (uint8_t *)"key12", 5);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_OK, res.status);
    TEST_ASSERT_EQUAL_UINT64(24, res.value);
}

void test_simple_lookup_2() {
    result res = simple_bytes_u64_hashmap_lookup(
        test_hashmap_2, (uint8_t *)"thiskeydoesnotexist", 19);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_NOT_FOUND, res.status);
}

void test_bidir_validity_1() {
    uint64_t max = sizeof(test_bidir_map_2) / sizeof(uint8_t);
    uint64_t res = bidir_bytes_u32_map_validate(test_bidir_map_2, max);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_OK, res);
}

void test_bidir_validity_2() {
    uint64_t max = sizeof(test_bidir_map_2) / sizeof(uint8_t);
    uint64_t res = bidir_bytes_u32_map_validate(test_bidir_map_2, max - 1);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_MALFORMED, res);
}

void test_bidir_lookup_1() {
    result res =
        bidir_bytes_u32_map_lookup(test_bidir_map_2, (uint8_t *)"key12", 5);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_OK, res.status);
    TEST_ASSERT_EQUAL_UINT64(12, res.value);
}

void test_bidir_lookup_2() {
    result res = bidir_bytes_u32_map_lookup(
        test_bidir_map_2, (uint8_t *)"thiskeydoesnotexist", 19);

    TEST_ASSERT_EQUAL_UINT64(HASHMAP_NOT_FOUND, res.status);
}

void test_bidir_revlookup_1() {
    bytestr_result res = bidir_bytes_u32_map_revlookup(test_bidir_map_2, 5);
    TEST_ASSERT_NOT_NULL(res.ptr);
    TEST_ASSERT_EQUAL_UINT64(4, res.len);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((uint8_t *)"key5", res.ptr, 4);
}

void test_bidir_revlookup_2() {
    bytestr_result res = bidir_bytes_u32_map_revlookup(test_bidir_map_2, 100);
    TEST_ASSERT_NULL(res.ptr);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_simple_validity_1);
    RUN_TEST(test_simple_validity_2);
    RUN_TEST(test_simple_lookup_1);
    RUN_TEST(test_simple_lookup_2);
    RUN_TEST(test_bidir_validity_1);
    RUN_TEST(test_bidir_validity_2);
    RUN_TEST(test_bidir_lookup_1);
    RUN_TEST(test_bidir_lookup_2);
    RUN_TEST(test_bidir_revlookup_1);
    RUN_TEST(test_bidir_revlookup_2);
    return UNITY_END();
}
