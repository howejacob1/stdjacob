#define _POSIX_C_SOURCE 199309L
#include "stdjacob.h"
#include <math.h>

// Float comparison helper - use epsilon for approximate equality
static bool float_eq(float a, float b) {
  return fabsf(a - b) < 1e-6f;
}

static bool double_eq(double a, double b) {
  return fabs(a - b) < (double)1e-9f;
}

// ============================================================================
// String helpers
// ============================================================================

static void test_streq(void) {
  assert(streq("abc", "abc"));
  assert(!streq("abc", "ab"));
  assert(!streq("abc", "abcd"));
  assert(streq("", ""));
}

static void test_streq_case_insensitive(void) {
  assert(streq_case_insensitive("AbC", "aBc"));
  assert(streq_case_insensitive("HELLO", "hello"));
  assert(!streq_case_insensitive("abc", "abcd"));
}

static void test_is_emptyish_str(void) {
  assert(is_emptyish_str("   \t\n"));
  assert(is_emptyish_str(""));
  assert(is_emptyish_str(NULL));
  assert(!is_emptyish_str("x"));
  assert(!is_emptyish_str("  x  "));
}

static void test_is_empty_str(void) {
  assert(is_empty_str(""));
  assert(!is_empty_str("a"));
  assert(!is_empty_str(" "));
}

static void test_strneq(void) {
  assert(strneq("hello world", "hello", 5));
  assert(!strneq("hello", "world", 5));
  assert(strneq("abc", "abcdef", 3));
}

static void test_strneq_lit(void) {
  assert(strneq_lit("hello world", "hello"));
  assert(!strneq_lit("world", "hello"));
  assert(strneq_lit("prefix_suffix", "prefix"));
}

static void test_str_to_upper(void) {
  char buf[] = "hello World 123";
  str_to_upper(buf);
  assert(streq(buf, "HELLO WORLD 123"));
}

static void test_trim_whitespace(void) {
  char buf1[] = "  hello  ";
  trim_whitespace(buf1);
  assert(streq(buf1, "hello"));

  char buf2[] = "no_trim";
  trim_whitespace(buf2);
  assert(streq(buf2, "no_trim"));

  char buf3[] = "   ";
  trim_whitespace(buf3);
  assert(streq(buf3, ""));
}

static void test_strsize(void) {
  assert(strsize("hello") == 6);  // includes null terminator
  assert(strsize("") == 1);
}

static void test_begins_with(void) {
  assert(begins_with("hello world", "hello"));
  assert(!begins_with("hello world", "world"));
  assert(begins_with("test", "test"));
  assert(begins_with("test", ""));
  assert(!begins_with("", "test"));
}

static void test_begins_with_char(void) {
  assert(begins_with_char("hello", 'h'));
  assert(!begins_with_char("hello", 'e'));
  assert(!begins_with_char("", 'x'));
}

static void test_ends_with(void) {
  assert(ends_with("hello.txt", ".txt"));
  assert(!ends_with("hello.txt", ".wav"));
  assert(ends_with("test", "test"));
  assert(ends_with("test", ""));
  assert(!ends_with("", "test"));
}

static void test_ends_with_char(void) {
  assert(ends_with_char("hello/", '/'));
  assert(!ends_with_char("hello", '/'));
  assert(!ends_with_char("", '/'));
}

static void test_is_dir_dots(void) {
  assert(is_dir_dots("."));
  assert(is_dir_dots(".."));
  assert(!is_dir_dots("..."));
  assert(!is_dir_dots("file"));
}

// ============================================================================
// Math helpers
// ============================================================================

static void test_is_divisible_by(void) {
  assert(is_divisible_by(10, 5));
  assert(is_divisible_by(10, 2));
  assert(!is_divisible_by(10, 3));
  assert(!is_divisible_by(10, 6));
}

static void test_clamp_int(void) {
  // Value within range - return unchanged
  assert(clamp_int(5, 0, 10) == 5);
  assert(clamp_int(0, 0, 10) == 0);
  assert(clamp_int(10, 0, 10) == 10);
  
  // Value below min - clamp to min
  assert(clamp_int(-5, 0, 10) == 0);
  assert(clamp_int(-100, -50, 50) == -50);
  
  // Value above max - clamp to max
  assert(clamp_int(15, 0, 10) == 10);
  assert(clamp_int(100, -50, 50) == 50);
  
  // Edge case: min == max
  assert(clamp_int(5, 7, 7) == 7);
  assert(clamp_int(7, 7, 7) == 7);
  assert(clamp_int(9, 7, 7) == 7);
  
  // Negative ranges
  assert(clamp_int(-25, -30, -20) == -25);
  assert(clamp_int(-35, -30, -20) == -30);
  assert(clamp_int(-15, -30, -20) == -20);
}

static void test_clamp_float(void) {
  // Value within range - return unchanged
  assert(float_eq(clamp_float(0.5f, 0.0f, 1.0f), 0.5f));
  assert(float_eq(clamp_float(0.0f, 0.0f, 1.0f), 0.0f));
  assert(float_eq(clamp_float(1.0f, 0.0f, 1.0f), 1.0f));
  
  // Value below min - clamp to min
  assert(float_eq(clamp_float(-0.5f, 0.0f, 1.0f), 0.0f));
  assert(float_eq(clamp_float(-100.0f, -50.0f, 50.0f), -50.0f));
  
  // Value above max - clamp to max
  assert(float_eq(clamp_float(1.5f, 0.0f, 1.0f), 1.0f));
  assert(float_eq(clamp_float(100.0f, -50.0f, 50.0f), 50.0f));
  
  // Edge case: min == max
  assert(float_eq(clamp_float(0.5f, 0.7f, 0.7f), 0.7f));
  assert(float_eq(clamp_float(0.7f, 0.7f, 0.7f), 0.7f));
  assert(float_eq(clamp_float(0.9f, 0.7f, 0.7f), 0.7f));
  
  // Negative ranges
  assert(float_eq(clamp_float(-2.5f, -3.0f, -2.0f), -2.5f));
  assert(float_eq(clamp_float(-3.5f, -3.0f, -2.0f), -3.0f));
  assert(float_eq(clamp_float(-1.5f, -3.0f, -2.0f), -2.0f));
}

static void test_is_power_of_two(void) {
  assert(is_power_of_two(1));
  assert(is_power_of_two(2));
  assert(is_power_of_two(1024));
  assert(!is_power_of_two(0));
  assert(!is_power_of_two(1023));
  assert(!is_power_of_two(3));
}

static void test_random_number_between_inclusive(void) {
  for (int i = 0; i < 100; i++) {
    int r = random_number_between_inclusive(5, 10);
    assert(r >= 5 && r <= 10);
  }
  // Edge case: same min and max
  assert(random_number_between_inclusive(7, 7) == 7);
}

// ============================================================================
// Macro helpers
// ============================================================================

static void test_what_power_of_two(void) {
  assert(WHAT_POWER_OF_TWO(1ull) == 0);
  assert(WHAT_POWER_OF_TWO(2ull) == 1);
  assert(WHAT_POWER_OF_TWO(4ull) == 2);
  assert(WHAT_POWER_OF_TWO(8ull) == 3);
  assert(WHAT_POWER_OF_TWO(1024ull) == 10);
  assert(WHAT_POWER_OF_TWO(9223372036854775808ull) == 63);
  assert(WHAT_POWER_OF_TWO(3ull) == -1);
  assert(WHAT_POWER_OF_TWO(0ull) == -1);
}

static void test_char_buffer(void) {
  CHAR_BUFFER(buffer, buffer_len, 16);
  assert(buffer_len == 16);
  memset(buffer, 'x', buffer_len);
  buffer[buffer_len - 1] = '\0';
  assert(buffer[0] == 'x');
  assert(buffer[buffer_len - 1] == '\0');
}

static void test_array_macros(void) {
  int numbers[] = {1, 2, 3, 4, 5};
  assert(ARRAY_ELEMENTS(numbers) == 5);
  assert(ARRAY_SIZE_CHARS(numbers) == sizeof(int) * 5);

  char str[] = "hello";
  assert(ARRAY_ELEMENTS(str) == 6);  // includes null terminator
}

static void test_min_max(void) {
  assert(MIN(3, 5) == 3);
  assert(MIN(5, 3) == 3);
  assert(MIN(-1, 1) == -1);
  assert(MAX(3, 5) == 5);
  assert(MAX(5, 3) == 5);
  assert(MAX(-1, 1) == 1);
}

static void test_is_multiple_of(void) {
  assert(IS_MULTIPLE_OF(12, 3));
  assert(IS_MULTIPLE_OF(12, 4));
  assert(IS_MULTIPLE_OF(12, 6));
  assert(!IS_MULTIPLE_OF(12, 5));
  assert(!IS_MULTIPLE_OF(12, 7));
  assert(IS_MULTIPLE_OF(0, 5));
}

static void test_forto(void) {
  uint sum = 0;
  FORTO(i, 5) {
    sum += i;
  }
  assert(sum == 0 + 1 + 2 + 3 + 4);
}

static void test_bits_in(void) {
  assert(BITS_IN(char) == 8);
  assert(BITS_IN(uint16_t) == 16);
  assert(BITS_IN(uint32_t) == 32);
  assert(BITS_IN(uint64_t) == 64);
}

static void test_zero_struct(void) {
  typedef struct { int a; int b; char c[10]; } test_struct_t;
  test_struct_t s = { .a = 42, .b = 99 };
  s.c[0] = 'x';
  ZERO_STRUCT(test_struct_t, s);
  assert(s.a == 0);
  assert(s.b == 0);
  assert(s.c[0] == '\0');
}

static void test_is_valid_port(void) {
  assert(is_valid_port(1));
  assert(is_valid_port(80));
  assert(is_valid_port(443));
  assert(is_valid_port(65535));
  assert(!is_valid_port(0));
  assert(!is_valid_port(-1));
  assert(!is_valid_port(65536));
}

static void test_str_to_port(void) {
  port_t port;
  
  // Valid ports
  assert(str_to_port("80", &port) && port == 80);
  assert(str_to_port("443", &port) && port == 443);
  assert(str_to_port("8080", &port) && port == 8080);
  assert(str_to_port("1", &port) && port == 1);
  assert(str_to_port("65535", &port) && port == 65535);
  
  // Invalid ports
  assert(!str_to_port("0", &port));
  assert(!str_to_port("-1", &port));
  assert(!str_to_port("65536", &port));
  assert(!str_to_port("abc", &port));
  assert(!str_to_port("80abc", &port));
  assert(!str_to_port("", &port));
  assert(!str_to_port("  ", &port));
}

static void test_bool_to_str(void) {
  assert(streq(BOOL_TO_STR(true), "true"));
  assert(streq(BOOL_TO_STR(false), "false"));
  assert(streq(BOOL_TO_STR(1 == 1), "true"));
  assert(streq(BOOL_TO_STR(1 == 2), "false"));
}

// ============================================================================
// Bit manipulation
// ============================================================================

static void test_bit_get_set_clear_toggle(void) {
  unsigned int x = 0;

  // BIT_SET
  x = BIT_SET(x, 0);
  assert(x == 1);
  x = BIT_SET(x, 3);
  assert(x == 9);  // 0b1001

  // BIT_GET
  assert(BIT_GET(x, 0) == 1);
  assert(BIT_GET(x, 1) == 0);
  assert(BIT_GET(x, 3) == 1);

  // BIT_CLEAR
  x = BIT_CLEAR(x, 0);
  assert(x == 8);  // 0b1000

  // BIT_TOGGLE
  x = BIT_TOGGLE(x, 3);
  assert(x == 0);
  x = BIT_TOGGLE(x, 5);
  assert(x == 32);
}

static void test_bit_n_macros(void) {
  unsigned char byte = 0xB4;  // 0b10110100
  assert(BIT_7(byte) == 1);
  assert(BIT_6(byte) == 0);
  assert(BIT_5(byte) == 1);
  assert(BIT_4(byte) == 1);
  assert(BIT_3(byte) == 0);
  assert(BIT_2(byte) == 1);
  assert(BIT_1(byte) == 0);
  assert(BIT_0(byte) == 0);
}

static void test_bit_bool_macros(void) {
  unsigned char byte = 0x81;  // 0b10000001
  assert(BIT_7_BOOL(byte) == true);
  assert(BIT_6_BOOL(byte) == false);
  assert(BIT_0_BOOL(byte) == true);
  assert(BIT_1_BOOL(byte) == false);
}

static void test_bit_counting(void) {
  // BIT_POPCOUNT - count set bits
  assert(BIT_POPCOUNT(0) == 0);
  assert(BIT_POPCOUNT(1) == 1);
  assert(BIT_POPCOUNT(0x0F) == 4);     // 0b1111
  assert(BIT_POPCOUNT(0xAA) == 4);     // 0b10101010

  // BIT_CLZ - count leading zeros (undefined for 0)
  assert(BIT_CLZ(1) == 31);
  assert(BIT_CLZ(0x80000000) == 0);
  assert(BIT_CLZ(0x40000000) == 1);

  // BIT_CTZ - count trailing zeros (undefined for 0)
  assert(BIT_CTZ(1) == 0);
  assert(BIT_CTZ(2) == 1);
  assert(BIT_CTZ(8) == 3);
  assert(BIT_CTZ(0x80000000) == 31);
}

// ============================================================================
// Memory helpers
// ============================================================================

static void test_malloc_or_die(void) {
  void* p = malloc_or_die(100);
  assert(p != NULL);
  free(p);
}

static void test_calloc_or_die(void) {
  int* arr = calloc_or_die(10, sizeof(int));
  assert(arr != NULL);
  // calloc should zero-initialize
  for (int i = 0; i < 10; i++) {
    assert(arr[i] == 0);
  }
  free(arr);
}

static void test_realloc_or_die(void) {
  void* p = malloc_or_die(10);
  p = realloc_or_die(p, 100);
  assert(p != NULL);
  free(p);
}

static void test_malloc_macros(void) {
  int* arr = MALLOC(5, int);
  assert(arr != NULL);
  arr[0] = 42;
  arr = REALLOC(arr, 10, int);
  assert(arr != NULL);
  assert(arr[0] == 42);
  free(arr);
}

static void test_malloc_chars(void) {
  char* str = malloc_chars(10);
  assert(str != NULL);
  strcpy(str, "hello");
  assert(streq(str, "hello"));
  free(str);
}

static void test_malloc_voidstars(void) {
  void** ptrs = malloc_voidstars(5);
  assert(ptrs != NULL);
  int x = 42;
  ptrs[0] = &x;
  assert(*(int*)ptrs[0] == 42);
  free(ptrs);
}

static void test_set_array_to_zero(void) {
  int arr[] = {1, 2, 3, 4, 5};
  set_array_to_zero(arr, sizeof(arr));
  for (int i = 0; i < 5; i++) {
    assert(arr[i] == 0);
  }
}

// ============================================================================
// Alignment helpers
// ============================================================================

static void test_misalignment(void) {
  // Test misalignment_amount
  char buffer[64];
  uintptr_t addr = (uintptr_t)buffer;

  // For 16-byte alignment
  uint misalign = misalignment_amount(buffer, 16);
  assert(misalign == (addr % 16));

  // Test is_misaligned
  void* aligned = (void*)(addr + (16 - (addr % 16)) % 16);
  assert(!is_misaligned(aligned, 16));

  // Test max_misalignment
  assert(max_misalignment(16) == 15);
  assert(max_misalignment(4) == 3);
  assert(max_misalignment(1) == 0);
}

// ============================================================================
// Time helpers
// ============================================================================

static void test_ns_to_sec(void) {
  assert(double_eq(ns_to_sec(1000000000L), (double)1.0f));
  assert(double_eq(ns_to_sec(500000000L), (double)0.5f));
  assert(double_eq(ns_to_sec(0L), (double)0.0f));
}

static void test_elapsed_sec(void) {
  struct timespec start = { .tv_sec = 10, .tv_nsec = 500000000 };
  struct timespec end = { .tv_sec = 12, .tv_nsec = 700000000 };
  double elapsed = elapsed_sec(&start, &end);
  // Should be 2.2 seconds
  assert(elapsed > (double)2.19f && elapsed < (double)2.21f);
}

static void test_get_monotonic_time_sec(void) {
  double t1 = get_monotonic_time_sec();
  assert(t1 > 0);  // Should be positive (system uptime in seconds)
  
  // Small busy wait
  for (volatile int i = 0; i < 1000000; i++);
  
  double t2 = get_monotonic_time_sec();
  assert(t2 >= t1);  // Monotonic: should never go backwards
  
  // Sleep for a bit and check time advances
  struct timespec sleep_time = { .tv_sec = 0, .tv_nsec = 10000000 };  // 10ms
  nanosleep(&sleep_time, NULL);
  double t3 = get_monotonic_time_sec();
  assert(t3 > t2);  // Should have advanced
  assert((t3 - t2) >= (double)0.009f);  // At least ~9ms should have passed
  assert((t3 - t2) < (double)1.0f);  // But not more than 1 second
}

// ============================================================================
// Path helpers
// ============================================================================

static void test_concat_paths(void) {
  char result[256];

  concat_paths(result, sizeof(result), "/home/user", "file.txt");
  assert(streq(result, "/home/user/file.txt"));

  concat_paths(result, sizeof(result), "/home/user/", "file.txt");
  assert(streq(result, "/home/user/file.txt"));

  concat_paths(result, sizeof(result), "/home/user", "/abs/path");
  // Implementation may vary - just check it doesn't crash
  assert(strlen(result) > 0);
}

static void test_is_valid_directory(void) {
  assert(is_valid_directory("/tmp"));
  assert(is_valid_directory("/"));
  assert(!is_valid_directory("/nonexistent_dir_12345"));
}

static void test_file_exists(void) {
  assert(file_exists("/etc/passwd"));
  assert(file_exists("/tmp"));
  assert(!file_exists("/nonexistent_file_12345"));
}

static void test_fread_definitely(void) {
  // Read from /dev/urandom - should always give us exactly what we ask for
  FILE* f = fopen("/dev/urandom", "rb");
  assert(f != NULL);
  
  unsigned char buf[32];
  size_t n = fread_definitely(buf, 1, 32, f);
  assert(n == 32);
  
  fclose(f);
}

static void test_count_str_occurrences_in_file(void) {
  // Create a temp file with known content
  char tmp_path[TMP_FILENAME_MAX];
  gen_tmp_filename(tmp_path, sizeof(tmp_path));
  
  FILE* f = fopen(tmp_path, "w");
  assert(f != NULL);
  fprintf(f, "hello world hello there hello\n");
  fprintf(f, "goodbye world\n");
  fclose(f);
  
  // Test counting
  assert(count_str_occurrences_in_file(tmp_path, "hello") == 3);
  assert(count_str_occurrences_in_file(tmp_path, "world") == 2);
  assert(count_str_occurrences_in_file(tmp_path, "goodbye") == 1);
  assert(count_str_occurrences_in_file(tmp_path, "notfound") == 0);
  assert(count_str_occurrences_in_file(tmp_path, "\n") == 2);
  
  // Test overlapping: "aa" in "aaa" should find 2
  FILE* f2 = fopen(tmp_path, "w");
  fprintf(f2, "aaa");
  fclose(f2);
  assert(count_str_occurrences_in_file(tmp_path, "aa") == 2);
  
  // Test nonexistent file
  assert(count_str_occurrences_in_file("/nonexistent_file_12345", "x") == -1);
  
  // Test empty needle
  assert(count_str_occurrences_in_file(tmp_path, "") == 0);
  
  // Cleanup
  remove(tmp_path);
}

static void test_count_char_occurrences_in_file(void) {
  char tmp_path[TMP_FILENAME_MAX];
  gen_tmp_filename(tmp_path, sizeof(tmp_path));
  
  FILE* f = fopen(tmp_path, "w");
  assert(f != NULL);
  fprintf(f, "hello world\n");
  fclose(f);
  
  assert(count_char_occurrences_in_file(tmp_path, 'l') == 3);
  assert(count_char_occurrences_in_file(tmp_path, 'o') == 2);
  assert(count_char_occurrences_in_file(tmp_path, 'h') == 1);
  assert(count_char_occurrences_in_file(tmp_path, '\n') == 1);
  assert(count_char_occurrences_in_file(tmp_path, 'z') == 0);
  assert(count_char_occurrences_in_file("/nonexistent_file_12345", 'x') == -1);
  
  remove(tmp_path);
}

static void test_generate_uuid4(void) {
  char uuid[UUID_STR_LEN];
  generate_uuid4(uuid, sizeof(uuid));
  
  // Check length (36 chars: 8-4-4-4-12)
  assert(strlen(uuid) == 36);
  
  // Check dashes in right places
  assert(uuid[8] == '-');
  assert(uuid[13] == '-');
  assert(uuid[18] == '-');
  assert(uuid[23] == '-');
  
  // Check version nibble (should be 4)
  assert(uuid[14] == '4');
  
  // Check variant nibble (should be 8, 9, a, or b)
  char v = uuid[19];
  assert(v == '8' || v == '9' || v == 'a' || v == 'b');
  
  // Generate another and make sure they're different
  char uuid2[UUID_STR_LEN];
  generate_uuid4(uuid2, sizeof(uuid2));
  assert(!streq(uuid, uuid2));
}

// ============================================================================
// File extension helpers
// ============================================================================

static void test_compression_detection(void) {
  assert(is_xz("file.xz"));
  assert(is_xz("archive.tar.xz"));
  assert(!is_xz("file.gz"));

  assert(is_zst("file.zst"));
  assert(!is_zst("file.zstd"));  // only .zst, not .zstd
  assert(!is_zst("file.xz"));

  assert(is_gz("file.gz"));
  assert(is_gz("archive.tar.gz"));
  assert(!is_gz("file.xz"));

  assert(is_compressed("file.xz"));
  assert(is_compressed("file.zst"));
  assert(is_compressed("file.gz"));
  assert(!is_compressed("file.txt"));
}

static void test_is_pathname_media(void) {
  assert(is_pathname_media("song.mp3"));
  assert(is_pathname_media("video.mp4"));
  assert(is_pathname_media("audio.wav"));
  assert(is_pathname_media("audio.flac"));
  assert(is_pathname_media("audio.ogg"));
  assert(is_pathname_media("audio.opus"));
  assert(is_pathname_media("video.webm"));
  assert(is_pathname_media("video.mkv"));
  assert(is_pathname_media("video.avi"));
  assert(is_pathname_media("video.mov"));
  assert(is_pathname_media("/path/to/AUDIO.WAV"));  // case insensitive
  assert(!is_pathname_media("document.txt"));
  assert(!is_pathname_media("image.png"));
}

// ============================================================================
// Constants
// ============================================================================

static void test_constants(void) {
  assert(SCANF_SUCCESS == 1);
  assert(BASE_10 == 10);
  assert(BASE_16 == 16);
  assert(BASE_2 == 2);
  assert(MAX_PORT_NUMBER == 65535);
  assert(BITS_PER_CHAR == 8);
  assert(SORT_CODE_LESSER == -1);
  assert(SORT_CODE_EQUAL == 0);
  assert(SORT_CODE_GREATER == 1);
}

// ============================================================================
// System helpers
// ============================================================================

static void test_num_cpus(void) {
  int n = num_cpus();
  assert(n >= 1);       // Must have at least 1 CPU
  assert(n <= 4096);    // Sanity check - unlikely to have more than 4096 CPUs
  printf("  num_cpus() = %d\n", n);
}

static void test_max_fds(void) {
  int n = max_fds();
  assert(n >= 64);      // Even minimal systems have at least 64 FDs
  printf("  max_fds() = %d\n", n);
}

static void test_semaphore_helpers(void) {
  sem_t sem;
  
  // Test thread-local semaphore
  create_sem_thread_local(&sem);
  // Post and wait should work
  sem_post(&sem);
  int ret = sem_trywait(&sem);
  assert(ret == 0);
  sem_destroy(&sem);
  
  // Test process-shared semaphore
  create_sem_process_shared(&sem);
  sem_post(&sem);
  ret = sem_trywait(&sem);
  assert(ret == 0);
  sem_destroy(&sem);
  
  printf("  semaphore helpers: ok\n");
}

// ============================================================================
// Main
// ============================================================================

int main(void) {
  init_random();

  // String helpers
  test_streq();
  test_streq_case_insensitive();
  test_is_emptyish_str();
  test_is_empty_str();
  test_strneq();
  test_strneq_lit();
  test_str_to_upper();
  test_trim_whitespace();
  test_strsize();
  test_begins_with();
  test_begins_with_char();
  test_ends_with();
  test_ends_with_char();
  test_is_dir_dots();

  // Math helpers
  test_is_divisible_by();
  test_clamp_int();
  test_clamp_float();
  test_is_power_of_two();
  test_random_number_between_inclusive();

  // Macro helpers
  test_what_power_of_two();
  test_char_buffer();
  test_array_macros();
  test_min_max();
  test_is_multiple_of();
  test_forto();
  test_bits_in();
  test_zero_struct();
  test_is_valid_port();
  test_str_to_port();
  test_bool_to_str();

  // Bit manipulation
  test_bit_get_set_clear_toggle();
  test_bit_n_macros();
  test_bit_bool_macros();
  test_bit_counting();

  // Memory helpers
  test_malloc_or_die();
  test_calloc_or_die();
  test_realloc_or_die();
  test_malloc_macros();
  test_malloc_chars();
  test_malloc_voidstars();
  test_set_array_to_zero();

  // Alignment helpers
  test_misalignment();

  // Time helpers
  test_ns_to_sec();
  test_elapsed_sec();
  test_get_monotonic_time_sec();

  // Path helpers
  test_concat_paths();
  test_is_valid_directory();
  test_file_exists();

  // File I/O helpers
  test_fread_definitely();
  test_count_str_occurrences_in_file();
  test_count_char_occurrences_in_file();

  // UUID generation
  test_generate_uuid4();

  // File extension helpers
  test_compression_detection();
  test_is_pathname_media();

  // Constants
  test_constants();

  // System helpers
  test_num_cpus();
  test_max_fds();
  test_semaphore_helpers();

  printf("All tests passed!\n");
  return 0;
}
