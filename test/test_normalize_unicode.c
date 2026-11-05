#include "unity.h"

#include "normalize_unicode.h"

void setUp(void) {}

void tearDown(void) {}

void test_normalize_unicode_0(){
    uint32_t src1[] = { 0x68, 0x65, 0x6C, 0x6C, 0x6F };     /* h e l l o */
    uint64_t len1  = 5;
    uint32_t out1[len1 + 1];

    uint32_t out_exp[] = { 0x2581, 0x68, 0x65, 0x6C, 0x6C, 0x6F }; /* ▁hello */
    uint64_t out1_len = 6;


    uint64_t ris = normalize_unicode(src1, len1, out1);
    if (ris < 0) (TEST_FAIL());
    else {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(out_exp, out1, out1_len);
    }
}

void test_normalize_unicode_1(){
    /* leading, trailing and consecutive spaces: " a  b " */
    uint32_t src1[] = { 0x20, 0x61, 0x20, 0x20, 0x62, 0x20 };
    uint64_t len1  = 6;
    uint32_t out1[len1 + 1];

    uint32_t out_exp[] = { 0x2581, 0x2581, 0x61, 0x2581, 0x2581, 0x62, 0x2581 };
    uint64_t out1_len = 7;

    uint64_t ris = normalize_unicode(src1, len1, out1);
    if (ris < 0) (TEST_FAIL());
    else {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(out_exp, out1, out1_len);
    }
}

void test_normalize_unicode_2(){
    /* empty input: only the prefix is produced */
    uint32_t src2[] = { 0 };
    uint64_t len2  = 0;
    uint32_t out2[len2 + 1];

    uint32_t out_exp[] = { 0x2581 };
    uint64_t out2_len = 1;

    uint64_t ris = normalize_unicode(src2, len2, out2);
    if (ris < 0) (TEST_FAIL());
    else {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(out_exp, out2, out2_len);
    }
}

void test_normalize_unicode_3(){
    /* mixed scripts with a space between: "naïve 日本" */
    uint32_t src3[] = { 0x6E, 0x61, 0xEF, 0x76, 0x65, 0x20, 0x65E5, 0x672C };
    uint64_t len3  = 8;
    uint32_t out3[len3 + 1];

    uint32_t out_exp[] = { 0x2581, 0x6E, 0x61, 0xEF, 0x76, 0x65, 0x2581, 0x65E5, 0x672C };
    uint64_t out3_len = 9;

    uint64_t ris = normalize_unicode(src3, len3, out3);
    if (ris < 0) (TEST_FAIL());
    else {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(out_exp, out3, out3_len);
    }
}

void test_normalize_unicode_4(){
    /* input that is only spaces: "   " */
    uint32_t src4[] = { 0x20, 0x20, 0x20 };
    uint64_t len4  = 3;
    uint32_t out4[len4 + 1];

    uint32_t out_exp[] = { 0x2581, 0x2581, 0x2581, 0x2581 };
    uint64_t out4_len = 4;

    uint64_t ris = normalize_unicode(src4, len4, out4);
    if (ris < 0) (TEST_FAIL());
    else {
        TEST_ASSERT_EQUAL_UINT32_ARRAY(out_exp, out4, out4_len);
    }
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_normalize_unicode_0);
    RUN_TEST(test_normalize_unicode_1);
    RUN_TEST(test_normalize_unicode_2);
    RUN_TEST(test_normalize_unicode_3);
    RUN_TEST(test_normalize_unicode_4);
    return UNITY_END();
}