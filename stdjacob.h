#ifndef STDJACOB_H
#define STDJACOB_H

#ifdef __cplusplus
extern "C" {
#endif

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
#include <limits.h>

// P_tmpdir is defined in stdio.h on POSIX systems
// On Windows, we use GetTempPath instead

// Platform detection - evaluates to true on Windows, false otherwise
#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
  #define IS_WINDOWS() true
#else
  #define IS_WINDOWS() false
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
  constexpr uint len_name = (len); \
  char buffer_name[len_name];

// returns elements in array
#define ARRAY_ELEMENTS(array) (sizeof(array)/sizeof(array[0]))
#define ARRAY_SIZE_CHARS(array) (sizeof(array))
// obviously double eval'd but.... 
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// Clamp value to range [min, max]
int clamp_int(int val, int min, int max);
float clamp_float(float val, float min, float max);
#define IS_MULTIPLE_OF(x, y) (((x) % (y)) == 0)

// Numeric base for strtol/strtoul
#define BASE_10 10
#define BASE_16 16
#define BASE_2  2

// Semaphore pshared argument
#define SEM_THREAD_LOCAL   0
#define SEM_PROCESS_SHARED 1

// Semaphore helpers (POSIX only)
#if !IS_WINDOWS()
#include <semaphore.h>
void create_sem_thread_local(sem_t* sem);
void create_sem_process_shared(sem_t* sem);
#endif

// Port number validation (TCP/UDP ports are 16-bit: 1-65535)
#define MAX_PORT_NUMBER 65535
#define is_valid_port(port) ((port) > 0 && (port) <= MAX_PORT_NUMBER)
typedef uint16_t port_t;
bool str_to_port(const char* str, port_t* out);

#define FORTO(var, to) for (uint var = 0; var < to; var++)

#define BITS_IN(type) (sizeof(type) * CHAR_BIT)

#define ZERO_STRUCT(type, var) do { \
    memset(&(var), 0, sizeof(type)); \
} while(0)

void enable_emojis(void);
int random_number_between_inclusive(int, int);
void init_random(void);
bool is_divisible_by(int, int);
bool streq(const char* str1, const char* str2);
bool streq_case_insensitive(const char* str1, const char* str2);
bool is_str_in_str(const char* haystack, const char* needle);
bool is_char_in_str(const char* haystack, char needle);
bool find_str_in_str(const char* haystack, const char* needle, uint* index);
bool find_char_in_str(const char* haystack, char needle, uint* index);

// Compare first n characters (like strncmp but returns bool)
#define strneq(s1, s2, n) (strncmp((s1), (s2), (n)) == 0)

// Compare string against a literal - uses sizeof to get length at compile time
// Note: only works with string literals, not char* variables
#define strneq_lit(s, lit) (strncmp((s), (lit), sizeof(lit) - 1) == 0)

// Check if string is empty (null-terminator at start)
#define is_empty_str(s) ((s)[0] == '\0')

// Check if string is NULL, empty, or whitespace-only
bool is_emptyish_str(const char* s);

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

#define DIE_IF_NULL(thing)                              \
  do {                                                  \
    if ((thing) == NULL) {                              \
      fprintf(stderr, "%s was null.\n", #thing);        \
      exit(1);                                          \
    }                                                   \
  } while (0)

bool is_stdin_closed(void);
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
void* calloc_or_die(size_t num_members, size_t size);
void* realloc_or_die(void* ptr, size_t num_bytes);

// generate a temporary variable name in a macro-- works because
// macros occur on "one line" via the backslash
// we have two #defines because ## doesn't do macroexpansion
#define TEMP_GENERATE_NAME_UNEXPANDED(prefix, line) prefix ## line
#define TEMP_GENERATE_NAME(prefix, line) TEMP_GENERATE_NAME_UNEXPANDED(prefix, line)
#define TEMP(prefix) TEMP_GENERATE_NAME(prefix, __LINE__)

int sys_page_size(void);
bool is_page_size_correct(int page_size);
void die_if_bad_page_size(int page_size);
uint misalignment_amount(void* ptr, uint num_bytes_to_align_to);
bool is_misaligned(void* ptr, uint num_bytes_to_align_to);
bool is_power_of_two(uintptr_t thing);
uint max_misalignment(uint num_bytes_to_align_to);
#define BITS_PER_CHAR 8

// Bit manipulation - basic ops (universal, no library needed)
#define BIT_GET(x, n)    (((x) >> (n)) & 1u)
#define BIT_SET(x, n)    ((x) | (1u << (n)))
#define BIT_CLEAR(x, n)  ((x) & ~(1u << (n)))
#define BIT_TOGGLE(x, n) ((x) ^ (1u << (n)))
#define BIT_7(x) BIT_GET(x, 7)
#define BIT_6(x) BIT_GET(x, 6)
#define BIT_5(x) BIT_GET(x, 5)
#define BIT_4(x) BIT_GET(x, 4)
#define BIT_3(x) BIT_GET(x, 3)
#define BIT_2(x) BIT_GET(x, 2)
#define BIT_1(x) BIT_GET(x, 1)
#define BIT_0(x) BIT_GET(x, 0)

// Boolean versions (return true/false)
#define BIT_GET_BOOL(x, n)   ((bool)(((x) >> (n)) & 1))
#define BIT_7_BOOL(x) BIT_GET_BOOL(x, 7)
#define BIT_6_BOOL(x) BIT_GET_BOOL(x, 6)
#define BIT_5_BOOL(x) BIT_GET_BOOL(x, 5)
#define BIT_4_BOOL(x) BIT_GET_BOOL(x, 4)
#define BIT_3_BOOL(x) BIT_GET_BOOL(x, 3)
#define BIT_2_BOOL(x) BIT_GET_BOOL(x, 2)
#define BIT_1_BOOL(x) BIT_GET_BOOL(x, 1)
#define BIT_0_BOOL(x) BIT_GET_BOOL(x, 0)

// Bit counting - compiler-agnostic with C23/GCC/Clang/MSVC support
#if __STDC_VERSION__ >= 202311L
  #include <stdbit.h>
  #define BIT_POPCOUNT(x)      stdc_count_ones(x)
  #define BIT_CLZ(x)           stdc_leading_zeros(x)
  #define BIT_CTZ(x)           stdc_trailing_zeros(x)
#elif defined(__GNUC__) || defined(__clang__)
  #define BIT_POPCOUNT(x)      __builtin_popcount(x)
  #define BIT_CLZ(x)           __builtin_clz(x)
  #define BIT_CTZ(x)           __builtin_ctz(x)
#else
  // Fallback: portable implementations (also covers MSVC)
  int bit_popcount(unsigned x);
  int bit_clz(unsigned x);
  int bit_ctz(unsigned x);
  #define BIT_POPCOUNT(x)      bit_popcount(x)
  #define BIT_CLZ(x)           bit_clz(x)
  #define BIT_CTZ(x)           bit_ctz(x)
#endif

#define BOOL_TO_STR(thing) ((thing) ? "true" : "false")

// Temporary filename generation
#define TMP_FILENAME_MAX 256
int gen_tmp_filename(char* buffer, size_t buffer_size);

// ISO 8601 timestamp generation (e.g., "2025-01-08T12:34:56Z")
void gen_iso_timestamp(char* buffer, size_t max_len);

// User/Privilege management
bool are_we_root(void);
bool become_user(const char* username);
void die(const char* message);
int printferr(const char* fmt, ...);

// Process management
void kill_our_process_group(void);

// Time utilities
double ns_to_sec(long nanoseconds);
double elapsed_sec(struct timespec* start, struct timespec* end);
double get_monotonic_time_sec(void);

// File I/O helpers
ssize_t read_definitely(int fd, void* buf, size_t count);
ssize_t read_definitely_or_die(int fd, void* buf, size_t count);
size_t fread_definitely(void* buf, size_t size, size_t nmemb, FILE* stream);
void* slurp_file(const char* path, size_t* size);
int count_str_occurrences_in_file(const char* path, const char* needle);
int count_char_occurrences_in_file(const char* path, char c);
#define file_go_to_beginning(f) fseek((f), 0, SEEK_SET)
#define file_go_to_end(f)       fseek((f), 0, SEEK_END)

// Compressed file I/O - opens files with automatic decompression
bool ends_with(const char* s, const char* suffix);
bool ends_with_char(const char* s, char c);
bool begins_with(const char* s, const char* prefix);
bool begins_with_char(const char* s, char c);
bool is_dir_dots(const char* s);
bool is_xz(const char* path);
bool is_zst(const char* path);
bool is_gz(const char* path);
bool is_compressed(const char* path);
FILE* open_xz(const char* path);
FILE* open_zst(const char* path);
FILE* open_gz(const char* path);
FILE* open_compressed(const char* path);
void close_compressed(FILE* f);

// Media file extensions (without leading period)
#define EXT_WAV   "wav"
#define EXT_MP3   "mp3"
#define EXT_MP4   "mp4"
#define EXT_M4A   "m4a"
#define EXT_FLAC  "flac"
#define EXT_OGG   "ogg"
#define EXT_OPUS  "opus"
#define EXT_WEBM  "webm"
#define EXT_MKV   "mkv"
#define EXT_AVI   "avi"
#define EXT_MOV   "mov"
#define EXT_WMV   "wmv"
#define EXT_AAC   "aac"
#define EXT_WMA   "wma"
#define EXT_AIFF  "aiff"
#define EXT_AIF   "aif"
#define EXT_RIFF  "riff"

// Media file detection
bool is_pathname_media(const char* pathname);

// Executable path utilities
void where_is_exe(char* buffer, size_t max_len);
void where_is_exe_dir(char* buffer, size_t max_len);

// UUID generation
#define UUID_STR_LEN 37  // 36 chars + null terminator
void generate_uuid4(char* buf, size_t size);

// Path manipulation
void concat_paths(char* dest, size_t max_size, const char* starting, const char* ending);
bool is_valid_directory(const char* path);

// File existence check
bool file_exists(const char* path);

// CPU count - returns number of CPUs available to this process
// On Linux: uses sched_getaffinity (respects cgroups/containers)
// On Windows: uses GetSystemInfo
// Fallback: uses sysconf(_SC_NPROCESSORS_ONLN)
int num_cpus(void);

// Max file descriptors - returns soft limit (ulimit -n equivalent)
// On POSIX: uses getrlimit(RLIMIT_NOFILE)
// On Windows: returns _getmaxstdio() (typically 512-8192)
int max_fds(void);

// Array helpers
// Find maximum value in unsigned integer array
// Returns the maximum value found, or 0 if array is empty
uint find_max_uint(const uint* uint_array, uint uint_array_size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
