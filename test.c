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

static void test_macro_helpers(void) {
  assert(WHAT_POWER_OF_TWO(1ull) == 0);
  assert(WHAT_POWER_OF_TWO(2ull) == 1);
  assert(WHAT_POWER_OF_TWO(8ull) == 3);
  assert(WHAT_POWER_OF_TWO(1024ull) == 10);
  assert(WHAT_POWER_OF_TWO(3ull) == -1);

  CHAR_BUFFER(buffer, buffer_len, 8);
  assert(buffer_len == 8);
  memset(buffer, 'x', buffer_len);
  buffer[buffer_len - 1] = '\0';
  assert(buffer[0] == 'x');
  assert(buffer[buffer_len - 1] == '\0');

  int numbers[] = {1, 2, 3, 4};
  assert(ARRAY_ELEMENTS(numbers) == 4);
  assert(ARRAY_SIZE_CHARS(numbers) == sizeof(numbers));

  assert(MIN(3, 5) == 3);
  assert(MAX(3, 5) == 5);
  assert(IS_MULTIPLE_OF(12, 3));
  assert(!IS_MULTIPLE_OF(12, 5));
}

int main(void) {
  init_random();
  test_string_helpers();
  test_math_helpers();
  test_macro_helpers();
  return 0;
}
