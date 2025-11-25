#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include <stdint.h>
#include <assert.h>

// Platform detection macro
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
  #define IS_WINDOWS() 1
#else
  #define IS_WINDOWS() 0
#endif

// Macro to determine what power of two a value is
  #define WHAT_POWER_OF_TWO(x)                                              \
  ((x) == 1ull ? 0 :                                                        \
  ((x) == 2ull ? 1 :                                                        \
  ((x) == 4ull ? 2 :                                                        \
  ((x) == 8ull ? 3 :                                                        \
  ((x) == 16ull ? 4 :                                                       \
  ((x) == 32ull ? 5 :                                                       \
  ((x) == 64ull ? 6 :                                                       \
  ((x) == 128ull ? 7 :                                                      \
  ((x) == 256ull ? 8 :                                                      \
  ((x) == 512ull ? 9 :                                                      \
  ((x) == 1024ull ? 10 :                                                    \
  ((x) == 2048ull ? 11 :                                                    \
  ((x) == 4096ull ? 12 :                                                    \
  ((x) == 8192ull ? 13 :                                                    \
  ((x) == 16384ull ? 14 :                                                   \
  ((x) == 32768ull ? 15 :                                                   \
  ((x) == 65536ull ? 16 :                                                   \
  ((x) == 131072ull ? 17 :                                                  \
  ((x) == 262144ull ? 18 :                                                  \
  ((x) == 524288ull ? 19 :                                                  \
  ((x) == 1048576ull ? 20 :                                                 \
  ((x) == 2097152ull ? 21 :                                                 \
  ((x) == 4194304ull ? 22 :                                                 \
  ((x) == 8388608ull ? 23 :                                                 \
  ((x) == 16777216ull ? 24 :                                                \
  ((x) == 33554432ull ? 25 :                                                \
  ((x) == 67108864ull ? 26 :                                                \
  ((x) == 134217728ull ? 27 :                                               \
  ((x) == 268435456ull ? 28 :                                               \
  ((x) == 536870912ull ? 29 :                                               \
  ((x) == 1073741824ull ? 30 :                                              \
  ((x) == 2147483648ull ? 31 :                                              \
  ((x) == 4294967296ull ? 32 :                                              \
  ((x) == 8589934592ull ? 33 :                                              \
  ((x) == 17179869184ull ? 34 :                                             \
  ((x) == 34359738368ull ? 35 :                                             \
  ((x) == 68719476736ull ? 36 :                                             \
  ((x) == 137438953472ull ? 37 :                                            \
  ((x) == 274877906944ull ? 38 :                                            \
  ((x) == 549755813888ull ? 39 :                                            \
  ((x) == 1099511627776ull ? 40 :                                           \
  ((x) == 2199023255552ull ? 41 :                                           \
  ((x) == 4398046511104ull ? 42 :                                           \
  ((x) == 8796093022208ull ? 43 :                                           \
  ((x) == 17592186044416ull ? 44 :                                          \
  ((x) == 35184372088832ull ? 45 :                                          \
  ((x) == 70368744177664ull ? 46 :                                          \
  ((x) == 140737488355328ull ? 47 :                                         \
  ((x) == 281474976710656ull ? 48 :                                         \
  ((x) == 562949953421312ull ? 49 :                                         \
  ((x) == 1125899906842624ull ? 50 :                                        \
  ((x) == 2251799813685248ull ? 51 :                                        \
  ((x) == 4503599627370496ull ? 52 :                                        \
  ((x) == 9007199254740992ull ? 53 :                                        \
  ((x) == 18014398509481984ull ? 54 :                                       \
  ((x) == 36028797018963968ull ? 55 :                                       \
  ((x) == 72057594037927936ull ? 56 :                                       \
  ((x) == 144115188075855872ull ? 57 :                                      \
  ((x) == 288230376151711744ull ? 58 :                                      \
  ((x) == 576460752303423488ull ? 59 :                                      \
  ((x) == 1152921504606846976ull ? 60 :                                     \
  ((x) == 2305843009213693952ull ? 61 :                                     \
  ((x) == 4611686018427387904ull ? 62 :                                     \
  ((x) == 9223372036854775808ull ? 63 : -1))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))

typedef unsigned char uchar;
typedef unsigned int uint; 
#define SCANF_SUCCESS 1

#define CHAR_BUFFER(buffer_name, len_name, len) \
  const uint len_name = len; \
  char buffer_name[len_name];

// returns elements in array
#define ARRAY_ELEMENTS(array) (sizeof(array)/sizeof(array[0]))
#define ARRAY_SIZE_CHARS(array) (sizeof(array))
// obviously double eval'd but.... 
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define FORTO(var, to) for (uint var = 0; var < to; var++)

void enable_emojis();
int random_number_between_inclusive(int, int);
void init_random();
bool is_divisible_by(int, int);
bool streq(const char* str1, const char* str2);
void str_to_upper(char* str);
void trim_whitespace(char* str);

void set_array_to_zero(void* array, uint num_bytes);

#define DIE_IF_ERROR(thing)                             \
  do {                                                  \
    if ((thing) == EXIT_FAILURE) {                      \
      fprintf(stderr, "Encountered error, dying");      \
      exit(1);                                          \
    }                                                   \
  } while (0);

bool is_stdin_closed();
#define YN_BUFFER_SIZE 255
bool ask_yn(const char* message, const char* error_message, bool default_is_yes);

bool is_str_in_list(const char* str, const char* str_list[], uint num_strs);

#define SORT_CODE_LESSER -1
#define SORT_CODE_EQUAL 0
#define SORT_CODE_GREATER 1
char* malloc_chars(uint num_chars);
void** malloc_voidstars(uint num_voidstars);
uint strsize(const char* str);

#define MALLOC(num, type) (malloc((num) * sizeof(type)))
#define REALLOC(result, num, type) (realloc(result, (num) * sizeof(type)))
void* malloc_or_die(size_t num_chars);

// generate a temporary variable name in a macro-- works because
// macros occur on "one line" via the backslash
// we have two #defines because ## doesn't do macroexpansion
#define TEMP_GENERATE_NAME_UNEXPANDED(prefix, line) prefix ## line
#define TEMP_GENERATE_NAME(prefix, line) TEMP_GENERATE_NAME_UNEXPANDED(prefix, line)
#define TEMP(prefix) TEMP_GENERATE_NAME(prefix, __LINE__)

int sys_page_size();
bool is_page_size_correct(int page_size);
void die_if_bad_page_size(int page_size);
uint misalignment_amount(void* ptr, uint num_bytes_to_align_to);
bool is_misaligned(void* ptr, uint num_bytes_to_align_to);
bool is_power_of_two(uintptr_t thing);
uint max_misalignment(uint num_bytes_to_align_to);
#define BITS_PER_CHAR 8

#define BOOL_TO_STR(thing) ((thing) ? "true" : "false")

// Temporary filename generation
#define TMP_FILENAME_MAX 256
int gen_tmp_filename(char* buffer, size_t buffer_size);

#endif
