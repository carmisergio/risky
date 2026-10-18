#include "unity.h"

// Funcitons under test
extern uint64_t utf8_to_unicode(uint8_t *src, uint64_t src_len, uint32_t *dst, uint64_t dst_len);

void setUp(void) {}

void tearDown(void) {}

void test_ut8_to_utf32_0() {
    uint64_t s_len = 10;
    uint8_t stringa[] = {0x43, 0x61, 0x66, 0x66, 0xC3, 0xA8, 0x20, 0xE2, 0x98, 0x95};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x0043, 0x0061, 0x0066, 0x0066, 0x00E8, 0x0020, 0x2615};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 7);
}

void test_ut8_to_utf32_1() {
    uint64_t s_len = 5;
    uint8_t stringa[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x0048, 0x0065, 0x006C, 0x006C, 0x006F};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 5);
}

void test_ut8_to_utf32_2() {
    uint64_t s_len = 8;
    uint8_t stringa[] = {0xC3, 0xA0, 0xC3, 0xA8, 0xC3, 0xAC, 0xC3, 0xB2};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x00E0, 0x00E8, 0x00EC, 0x00F2};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 4);
}

void test_ut8_to_utf32_3() {
    uint64_t s_len = 6;
    uint8_t stringa[] = {0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x65E5, 0x672C};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 2);
}

void test_ut8_to_utf32_4() {
    uint64_t s_len = 8;
    uint8_t stringa[] = {0xF0, 0x9F, 0x9A, 0x80, 0xF0, 0x9F, 0x8C, 0x8D};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x0001F680, 0x0001F30D};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 2);
}

void test_ut8_to_utf32_5() {
    uint64_t s_len = 10;
    uint8_t stringa[] = {0x24, 0xC2, 0xA2, 0xE2, 0x82, 0xAC, 0xF0, 0x90, 0x8D, 0x88};
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {0x0024, 0x00A2, 0x20AC, 0x00010348};    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 4);
}


void test_ut8_to_utf32_6() {
    uint64_t s_len = 50;
    uint8_t stringa[] = {
        0x7E,
        0xCE, 0xBB,
        0xD0, 0x96,
        0xDF, 0xBF,
        0xE0, 0xA0, 0x80,
        0xE2, 0x89, 0x88,
        0xED, 0x9F, 0xBF,
        0xEE, 0x80, 0x80,
        0xEF, 0xBF, 0xBD,
        0xF0, 0x90, 0x80, 0x80,
        0xF0, 0x9F, 0xA6, 0x80,
        0xF0, 0x9F, 0x92, 0xA9,
        0xF3, 0xA0, 0x81, 0x81,
        0xF4, 0x8F, 0xBF, 0xBD
    };
    uint32_t dst[100];
    uint64_t dst_len = 100;

    uint32_t dst_len_off = utf8_to_unicode(stringa, s_len, dst, dst_len);
    if (dst_len_off < 0) TEST_FAIL();
    uint32_t array_exp[] = {
        0x0000007E,
        0x000003BB,
        0x00000416,
        0x000007FF,
        0x00000800,
        0x00002248,
        0x0000D7FF,
        0x0000E000,
        0x0000FFFD,
        0x00010000,
        0x0001F980,
        0x0001F4A9,
        0x000E0041,
        0x0010FFFD
    };    
    TEST_ASSERT_EQUAL_UINT32_ARRAY(array_exp, dst, 14);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ut8_to_utf32_0);
    RUN_TEST(test_ut8_to_utf32_1);
    RUN_TEST(test_ut8_to_utf32_2);
    RUN_TEST(test_ut8_to_utf32_4);
    RUN_TEST(test_ut8_to_utf32_5);
    RUN_TEST(test_ut8_to_utf32_6);
    return UNITY_END();
}
