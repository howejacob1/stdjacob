#define _XOPEN_SOURCE 500
#include "stdjacob.h"

#if IS_WINDOWS()
  #include <windows.h>
#else
  #ifndef P_tmpdir
    #define P_tmpdir "/tmp"
  #endif
#endif

void enable_emojis() {
  setlocale(LC_ALL, "en_US.utf8");
}

int random_number_between_inclusive(int start, int end) {
  int lowest = MIN(start, end);
  int highest = MAX(start, end);
  int range = abs(highest - lowest) + 1;
  int modulo_result = rand() % range;
  return modulo_result + lowest;
}

void init_random() {
  srand(clock());
}

bool is_divisible_by(int thing, int by) {
  return !(thing % by);
}

bool streq(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

void str_to_upper(char* str) {
  uint index = 0;
  char cur;
  do {
    cur = str[index];
    str[index] = toupper(cur);
    index++;
  } while (cur);
}

void trim_whitespace(char* str) {
  uint src_index = 0;
  uint dest_index = 0;
  char cur;
  do {
    cur = str[src_index];    
    if (!isspace(cur)) {
      if (dest_index != src_index) {
        str[dest_index] = str[src_index];
      }
      dest_index++;
    }
    src_index++;
  } while (cur);
}

void set_array_to_zero(void* array, uint num_bytes) {
  memset(array, 0, num_bytes);
}

bool is_stdin_closed() {
  return (bool)feof(stdin);
}

bool is_empty_str(const char* str) {
  return strlen(str) == 0;
}  

bool ask_yn(const char* message, const char* error_message, bool default_is_yes) {
  const bool invalid_input = true; // for compiler warning, technically infinite loop 
  char input[YN_BUFFER_SIZE];
  char* fgets_result;
  do {
    printf("%s", message);
    fgets_result = fgets(input, YN_BUFFER_SIZE, stdin);
    if ((fgets_result == NULL) || is_stdin_closed()) {
      return false;
    }
    str_to_upper(input);
    trim_whitespace(input);
    if (is_empty_str(input)) {
      return default_is_yes;
    }      
    if (streq(input, "YES") || streq(input, "Y")) {
      return true;
    }
    if (streq(input, "NO") || streq(input, "N")) {
      return false;
    }
    printf("%s", error_message);
  } while (invalid_input); // for compiler warning, technically infinite loop
  return false; // for compiler warning
}

bool is_str_in_list(const char* str, const char* str_list[],
                    const uint num_strs) {
  const char* cur_str;
  FORTO(index, num_strs) {
    cur_str = str_list[index];
    if (streq(cur_str, str)) {
      return true;
    }
  }
  return false;
}

char* malloc_chars(uint num_chars) {
  return malloc(num_chars*sizeof(char));
}

void** malloc_voidstars(uint num_voidstars){
  return malloc(num_voidstars * sizeof(void*));
}

uint strsize(const char* str) {
  return strlen(str)+1;
}

void* malloc_or_die(size_t num_chars) {
  void* result = malloc(num_chars);
  if (result == NULL) {
    fprintf(stderr, "Failed to malloc.\n");
    exit(ENOMEM);
  }
  return result;
}

int sys_page_size() {
#if IS_WINDOWS()
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return si.dwPageSize;
#else
  return sysconf(_SC_PAGE_SIZE);
#endif
}

bool is_page_size_correct(int page_size) {
  uint cur_page_size = sys_page_size();
  if (page_size != cur_page_size) {
    return false;    
  }
  return true;
}

void die_if_bad_page_size(int page_size) {
  if (!is_page_size_correct(page_size)) {
    exit(EINVAL);
  }    
}

uint misalignment_amount(void* ptr, uint num_bytes_to_align_to) {
  uintptr_t pointer_as_uint = (uintptr_t)ptr;
  return pointer_as_uint % num_bytes_to_align_to;
}

bool is_misaligned(void* ptr, uint num_bytes_to_align_to) {
  return (bool)(misalignment_amount(ptr, num_bytes_to_align_to));
}

bool is_power_of_two(uintptr_t thing) {
  // Got this approach from offline--
  // A power of two like 2^32 only contains one bit.
  // AFAIK, most CPUs contain a bit counting instruction, but C does not.
  // Therefore, the following trick is applied:
  // Say you have 2^3, or 8.
  // 0000 1000
  // you subtract 1,
  // 0000 0111
  // then ANDing both will always return all zeroes if a power of two.
  if (thing == 0) {
    return false;
  }
  return !(thing & (thing - 1));
}

uint max_misalignment(uint num_bytes_to_align_to) {
  assert(is_power_of_two(num_bytes_to_align_to));
  return num_bytes_to_align_to - 1;
}

int gen_tmp_filename(char* buffer, size_t buffer_size) {
  assert(buffer != NULL);
  assert(buffer_size >= TMP_FILENAME_MAX);
  
#if IS_WINDOWS()
  // Windows implementation using GetTempFileName
  char temp_path[MAX_PATH];
  DWORD result = GetTempPath(MAX_PATH, temp_path);
  if (result == 0 || result > MAX_PATH) {
    return -1;
  }
  
  if (GetTempFileName(temp_path, "tmp", 0, buffer) == 0) {
    return -1;
  }
  return 0;
#else
  // Unix/Linux/macOS implementation using mkstemp
  int fd;
  
  // P_tmpdir is always defined on POSIX systems
  snprintf(buffer, buffer_size, P_tmpdir "/XXXXXX");
  
  fd = mkstemp(buffer);
  if (fd == -1) {
    return -1;
  }
  
  close(fd);
  return 0;
#endif
}

/* === Compressed file I/O === */

bool ends_with(const char* s, const char* suffix) {
  size_t slen = strlen(s);
  size_t suflen = strlen(suffix);
  return slen >= suflen && strcmp(s + slen - suflen, suffix) == 0;
}

bool is_xz(const char* path)  { return ends_with(path, ".xz"); }
bool is_zst(const char* path) { return ends_with(path, ".zst"); }
bool is_gz(const char* path)  { return ends_with(path, ".gz"); }
bool is_compressed(const char* path) { return is_xz(path) || is_zst(path) || is_gz(path); }

static FILE* open_pipe(const char* cmd, const char* path) {
  char buf[4096];
  snprintf(buf, sizeof(buf), "%s '%s'", cmd, path);
  return popen(buf, "r");
}

FILE* open_xz(const char* path)  { return open_pipe("xz -dc", path); }
FILE* open_zst(const char* path) { return open_pipe("zstd -dc", path); }
FILE* open_gz(const char* path)  { return open_pipe("gzip -dc", path); }

FILE* open_compressed(const char* path) {
  if (is_xz(path))  return open_xz(path);
  if (is_zst(path)) return open_zst(path);
  if (is_gz(path))  return open_gz(path);
  return fopen(path, "r");
}

void close_compressed(FILE* f) {
  pclose(f);
}

/* === Bit counting fallbacks (when no builtins available) === */

#if !(__STDC_VERSION__ >= 202311L) && !defined(__GNUC__) && !defined(__clang__)

int bit_popcount(unsigned x) {
  int count = 0;
  while (x) {
    count += x & 1;
    x >>= 1;
  }
  return count;
}

int bit_clz(unsigned x) {
  if (x == 0) return sizeof(unsigned) * 8;
  int count = 0;
  unsigned mask = 1u << (sizeof(unsigned) * 8 - 1);
  while (!(x & mask)) {
    count++;
    mask >>= 1;
  }
  return count;
}

int bit_ctz(unsigned x) {
  if (x == 0) return sizeof(unsigned) * 8;
  int count = 0;
  while (!(x & 1)) {
    count++;
    x >>= 1;
  }
  return count;
}

#endif
