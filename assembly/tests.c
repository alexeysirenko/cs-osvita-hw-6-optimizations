#include "vendor/unity.h"

extern int pangram(char *phrase);

void setUp(void) {
}

void tearDown(void) {
}

void test_pangram_empty(void) { TEST_ASSERT_FALSE(pangram("")); }
void test_pangram_alpha(void) {
  TEST_ASSERT_TRUE(pangram("abcdefghijklmnopqrstuvwxyz"));
}
void test_pangram_punctuated(void) {
  TEST_ASSERT_TRUE(pangram("abc, def! ghi... jkl25; mnopqrstuvwxyz"));
}
void test_pangram_incomplete(void) {
  TEST_ASSERT_FALSE(pangram("abcdefghijklmnopqrstuvwxy"));
}
void test_pangram_mixed_case(void) {
  TEST_ASSERT_TRUE(pangram("ABCdefGHIjklMNOpqrSTUvwxYZ"));
}
void test_pangram_phrase(void) {
  TEST_ASSERT_TRUE(pangram("the quick brown fox jumps over teh lazy dog"));
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_pangram_empty);
    RUN_TEST(test_pangram_alpha);
    RUN_TEST(test_pangram_punctuated);
    RUN_TEST(test_pangram_incomplete);
    RUN_TEST(test_pangram_mixed_case);
    RUN_TEST(test_pangram_phrase);

    return UNITY_END();
}
