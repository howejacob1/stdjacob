#include "stdjacob.h"

static void test_string_helpers(void) {
  assert(streq("abc", "abc"));
  assert(!streq("abc", "ab"));
  assert(streq_case_insensitive("AbC", "aBc"));
  assert(is_emptyish_str("   \t\n"));
  assert(!is_emptyish_str("x"));
}

static void test_math_helpers(void) {
  assert(is_divisible_by(10, 5));
  assert(!is_divisible_by(10, 6));
  assert(is_power_of_two(1024));
  assert(!is_power_of_two(1023));
}

int main(void) {
  init_random();
  test_string_helpers();
  test_math_helpers();
  return 0;
}
