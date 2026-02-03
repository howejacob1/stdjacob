#define _GNU_SOURCE
#define _XOPEN_SOURCE 500
#include "utils.h"
#include <pwd.h>
#include <stdarg.h>
#include <libgen.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/resource.h>

#if IS_WINDOWS()
  #include <windows.h>
  #include <stdio.h>  // for _getmaxstdio
#else
  #ifndef P_tmpdir
    #define P_tmpdir "/tmp"
  #endif
#endif

#if defined(__linux__)
  #include <sched.h>
#endif

void enable_emojis(void) {
  setlocale(LC_ALL, "en_US.utf8");
}

int random_number_between_inclusive(int start, int end) {
  int lowest = MIN(start, end);
  int highest = MAX(start, end);
  int range = abs(highest - lowest) + 1;
  int modulo_result = rand() % range;
  return modulo_result + lowest;
}

void init_random(void) {
  srand((unsigned int)clock());
}

bool is_divisible_by(int thing, int by) {
  return !(thing % by);
}

int min3(int a, int b, int c) {
  int m = a;
  if (b < m) m = b;
  if (c < m) m = c;
  return m;
}

int max3(int a, int b, int c) {
  int m = a;
  if (b > m) m = b;
  if (c > m) m = c;
  return m;
}

int clamp_int(int val, int min, int max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

float clamp_float(float val, float min, float max) {
  if (val < min) return min;
  if (val > max) return max;
  return val;
}

bool streq(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;
}

bool streq_maybe(const char* str1, const char* str2) {
  if (!str1 || !str2) return false;
  return streq(str1, str2);
}

bool streq_case_insensitive(const char* str1, const char* str2) {
  if (str1 == str2) return true;
  if (!str1 || !str2) return false;
  while (*str1 && *str2) {
    if (tolower((unsigned char)*str1) != tolower((unsigned char)*str2)) {
      return false;
    }
    str1++;
    str2++;
  }
  return *str1 == *str2;  // Both must be '\0'
}

bool is_str_in_str(const char* haystack, const char* needle) {
  return strstr(haystack, needle) != NULL;
}

bool is_char_in_str(const char* haystack, char needle) {
  return strchr(haystack, needle) != NULL;
}

bool find_str_in_str(const char* haystack, const char* needle, uint* index) {
  const char* p = strstr(haystack, needle);
  if (p == NULL) return false;
  *index = (uint)(p - haystack);
  return true;
}

bool find_char_in_str(const char* haystack, char needle, uint* index) {
  const char* p = strchr(haystack, needle);
  if (p == NULL) return false;
  *index = (uint)(p - haystack);
  return true;
}

bool is_emptyish_str(const char* s) {
  if (!s) return true;
  while (*s) {
    if (!isspace((unsigned char)*s)) return false;
    s++;
  }
  return true;
}

void str_to_lower(char* str) {
  uint index = 0;
  char cur;
  do {
    cur = str[index];
    str[index] = (char)tolower((unsigned char)cur);
    index++;
  } while (cur);
}

void remove_str_punctuation(char* str) {
  uint src_index = 0;
  uint dest_index = 0;
  char cur;
  do {
    cur = str[src_index];
    // Keep alphanumeric and spaces
    if (isalnum((unsigned char)cur) || isspace((unsigned char)cur) || cur == '\0') {
      if (dest_index != src_index) {
        str[dest_index] = cur;
      }
      dest_index++;
    }
    src_index++;
  } while (cur);
  // Ensure null termination if loop finished
  if (dest_index > 0 && str[dest_index-1] != '\0') {
      str[dest_index] = '\0';
  }
}

void str_to_upper(char* str) {
  uint index = 0;
  char cur;
  do {
    cur = str[index];
    str[index] = (char)toupper((unsigned char)cur);
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

bool is_stdin_closed(void) {
  return feof(stdin) != 0;
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
  return (uint)(strlen(str)+1);
}

void* malloc_or_die(size_t num_chars) {
  void* result = malloc(num_chars);
  if (result == NULL) {
    fprintf(stderr, "Failed to malloc.\n");
    exit(ENOMEM);
  }
  return result;
}

void* calloc_or_die(size_t num_members, size_t size) {
  void* result = calloc(num_members, size);
  if (result == NULL) {
    fprintf(stderr, "Failed to calloc.\n");
    exit(ENOMEM);
  }
  return result;
}

void* realloc_or_die(void* ptr, size_t num_bytes) {
  void* result = realloc(ptr, num_bytes);
  if (result == NULL) {
    fprintf(stderr, "Failed to realloc.\n");
    exit(ENOMEM);
  }
  return result;
}

char* strdup_or_die(const char* str) {
  DIE_IF_NULL(str);
  char* result = strdup(str);
  if (result == NULL) {
    fprintf(stderr, "Failed to strdup.\n");
    exit(ENOMEM);
  }
  return result;
}

int sys_page_size(void) {
#if IS_WINDOWS()
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return (int)si.dwPageSize;
#else
  return (int)sysconf(_SC_PAGE_SIZE);
#endif
}

bool is_page_size_correct(int page_size) {
  int cur_page_size = sys_page_size();
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
  return (uint)(pointer_as_uint % num_bytes_to_align_to);
}

bool is_misaligned(void* ptr, uint num_bytes_to_align_to) {
  return misalignment_amount(ptr, num_bytes_to_align_to) != 0;
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

void gen_iso_timestamp(char* buffer, size_t max_len) {
  time_t now = time(NULL);
  struct tm* tm_info = gmtime(&now);
  strftime(buffer, max_len, "%Y-%m-%dT%H:%M:%SZ", tm_info);
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

bool ends_with_char(const char* s, char c) {
  size_t len = strlen(s);
  return len > 0 && s[len - 1] == c;
}

bool begins_with(const char* s, const char* prefix) {
  size_t prelen = strlen(prefix);
  return strncmp(s, prefix, prelen) == 0;
}

bool begins_with_char(const char* s, char c) {
  return s[0] == c;
}

bool is_dir_dots(const char* s) {
  DIE_IF_NULL(s);
  return streq(s, ".") || streq(s, "..");
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

/* === File I/O helpers === */

ssize_t read_definitely(int fd, void* buf, size_t count) {
  size_t bytes_read = 0;
  char* buffer = (char*)buf;
  
  while (bytes_read < count) {
    ssize_t n = read(fd, buffer + bytes_read, count - bytes_read);
    if (n < 0) {
      return -1;  // Error
    }
    if (n == 0) {
      break;  // EOF
    }
    bytes_read += (size_t)n;
  }
  
  return (ssize_t)bytes_read;
}

ssize_t read_definitely_or_die(int fd, void* buf, size_t count) {
  ssize_t result = read_definitely(fd, buf, count);
  if (result < 0) {
    die("read() failed");
  }
  if ((size_t)result < count) {
    die("read() returned insufficient bytes");
  }
  return result;
}

size_t fread_definitely(void* buf, size_t size, size_t nmemb, FILE* stream) {
  size_t total = size * nmemb;
  size_t bytes_read = 0;
  char* buffer = (char*)buf;

  while (bytes_read < total) {
    size_t n = fread(buffer + bytes_read, 1, total - bytes_read, stream);
    if (n == 0) {
      break;  // EOF or error
    }
    bytes_read += n;
  }

  return bytes_read / size;  // Return number of elements, like fread
}

void* slurp_file(const char* path, size_t* size) {
  FILE* f = fopen(path, "rb");
  if (!f) return NULL;

  fseek(f, 0, SEEK_END);
  long file_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  if (file_size <= 0) {
    fclose(f);
    return NULL;
  }

  void* data = malloc((size_t)file_size);
  if (!data) {
    fclose(f);
    return NULL;
  }

  size_t bytes_read = fread(data, 1, (size_t)file_size, f);
  fclose(f);

  if (bytes_read != (size_t)file_size) {
    free(data);
    return NULL;
  }

  if (size) *size = (size_t)file_size;
  return data;
}

int count_str_occurrences_in_file(const char* path, const char* needle) {
  if (!needle || !needle[0]) return 0;

  size_t file_size;
  char* data = slurp_file(path, &file_size);
  if (!data) return -1;

  int count = 0;
  size_t needle_len = strlen(needle);
  char* p = data;
  char* end = data + file_size;

  while (p + needle_len <= end) {
    char* found = memmem(p, (size_t)(end - p), needle, needle_len);
    if (!found) break;
    count++;
    p = found + 1;  // Allow overlapping matches
  }

  free(data);
  return count;
}

int count_char_occurrences_in_file(const char* path, char c) {
  size_t file_size;
  char* data = slurp_file(path, &file_size);
  if (!data) return -1;

  int count = 0;
  for (size_t i = 0; i < file_size; i++) {
    if (data[i] == c) count++;
  }

  free(data);
  return count;
}

bool str_to_port(const char* str, port_t* out) {
  char* end;
  long val = strtol(str, &end, BASE_10);
  if (end == str || !is_empty_str(end) || !is_valid_port(val)) {
    return false;
  }
  *out = (port_t)val;
  return true;
}

/* === User/Privilege management === */

bool are_we_root(void) {
  return getuid() == 0;
}

void die(const char* message) {
  fprintf(stderr, "%s\n", message);
  exit(1);
}

int printferr(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int result = vfprintf(stderr, fmt, args);
  va_end(args);
  return result;
}

void kill_our_process_group(void) {
  kill(0, SIGTERM);
}

void renice_maybe(int inc) {
#if !IS_WINDOWS()
    errno = 0;
    int ret = nice(inc);
    (void)ret;
#else
    (void)inc;
#endif
}

void renice(int inc) {
#if !IS_WINDOWS()
    errno = 0;
    int ret = nice(inc);
    if (ret == -1 && errno != 0) {
        perror("nice");
        exit(1);
    }
#else
    (void)inc;
    fprintf(stderr, "renice not supported on Windows\n");
    exit(1);
#endif
}

int get_niceness(void) {
#if !IS_WINDOWS()
    errno = 0;
    int prio = getpriority(PRIO_PROCESS, 0);
    if (prio == -1 && errno != 0) {
        perror("getpriority");
        exit(1);
    }
    return prio;
#else
    return 0;
#endif
}

double ns_to_sec(long nanoseconds) {
  return (double)nanoseconds / (double)1e9f;
}

double elapsed_sec(struct timespec* start, struct timespec* end) {
  return (double)(end->tv_sec - start->tv_sec) + ns_to_sec(end->tv_nsec - start->tv_nsec);
}

double get_monotonic_time_sec(void) {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (double)ts.tv_sec + (double)ts.tv_nsec / (double)1e9f;
}

bool become_user(const char* username) {
  struct passwd* pw = getpwnam(username);
  if (!pw) {
    fprintf(stderr, "Error: user '%s' not found\n", username);
    return false;
  }

  if (setgid(pw->pw_gid) != 0) {
    perror("setgid");
    return false;
  }

  if (setuid(pw->pw_uid) != 0) {
    perror("setuid");
    return false;
  }

  return true;
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

/* === Media file detection === */

static const char* media_extensions[] = {
  EXT_WAV, EXT_MP3, EXT_MP4, EXT_M4A, EXT_FLAC, EXT_OGG, EXT_OPUS,
  EXT_WEBM, EXT_MKV, EXT_AVI, EXT_MOV, EXT_WMV, EXT_AAC, EXT_WMA,
  EXT_AIFF, EXT_AIF, EXT_RIFF
};

bool is_pathname_media(const char* pathname) {
  if (!pathname) return false;
  
  // Find last dot in pathname
  const char* dot_and_ext = strrchr(pathname, '.'); // a.b.c returns dot = ".c" 
  bool was_dot_found = (dot_and_ext != NULL) && (dot_and_ext != pathname);
  if (!was_dot_found) return false;
  
  const char* ext = dot_and_ext + 1;
  if (*ext == '\0') return false;
  
  // Case-insensitive comparison against known extensions
  for (size_t i = 0; i < ARRAY_ELEMENTS(media_extensions); i++) {
    if (streq_case_insensitive(ext, media_extensions[i])) {
      return true;
    }
  }
  return false;
}

/* === Executable path utilities === */

void where_is_exe(char* buffer, size_t max_len) {
  ssize_t len = readlink("/proc/self/exe", buffer, max_len - 1);
  if (len < 0) {
    die("Failed to read /proc/self/exe");
  }
  buffer[len] = '\0';
}

void where_is_exe_dir(char* buffer, size_t max_len) {
  char exe_path[PATH_MAX];
  ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
  if (len < 0) {
    die("Failed to read /proc/self/exe");
  }
  exe_path[len] = '\0';
  char* dir = dirname(exe_path);
  strncpy(buffer, dir, max_len - 1);
  buffer[max_len - 1] = '\0';
}

void concat_paths(char* dest, size_t max_size, const char* starting, const char* ending) {
  DIE_IF_NULL(dest);
  DIE_IF_NULL(starting);
  DIE_IF_NULL(ending);
  
  bool starting_ends_with_slash = ends_with_char(starting, '/');
  bool ending_starts_with_slash = begins_with_char(ending, '/');
  
  if (starting_ends_with_slash && ending_starts_with_slash) {
    ending++;
  }
  
  bool need_slash = !starting_ends_with_slash && !ending_starts_with_slash;
  const char* sep = need_slash ? "/" : "";
  
  snprintf(dest, max_size, "%s%s%s", starting, sep, ending);
}

bool is_valid_directory(const char* path) {
  if (!path) return false;
  struct stat st;
  return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
}

bool file_exists(const char* path) {
  return access(path, F_OK) == 0;
}

// Fill buffer with cryptographically random bytes
static void fill_random_bytes(uchar* buf, size_t len) {
#if IS_WINDOWS()
  // RtlGenRandom (SystemFunction036) - available on all Windows versions
  extern BOOLEAN NTAPI SystemFunction036(PVOID, ULONG);
  SystemFunction036(buf, (ULONG)len);
#else
  FILE* f = fopen("/dev/urandom", "rb");
  if (f) {
    fread_definitely(buf, 1, len, f);
    fclose(f);
  }
#endif
}

void generate_uuid4(char* buf, size_t size) {
  if (size < UUID_STR_LEN) return;

  uchar bytes[16];
  fill_random_bytes(bytes, 16);

  // Set version (4) and variant bits
  bytes[6] = (bytes[6] & 0x0F) | 0x40;
  bytes[8] = (bytes[8] & 0x3F) | 0x80;

  // UUID format: 8-4-4-4-12 (indices where dashes go: after 4, 6, 8, 10)
  static const int dash_after[] = {4, 6, 8, 10};
  int dash_idx = 0;
  int pos = 0;

  for (int i = 0; i < 16; i++) {
    pos += sprintf(buf + pos, "%02x", bytes[i]);
    if (dash_idx < 4 && i + 1 == dash_after[dash_idx]) {
      buf[pos++] = '-';
      dash_idx++;
    }
  }
  buf[pos] = '\0';
}

int num_cpus(void) {
#if defined(__linux__)
  // Linux: use sched_getaffinity (respects cgroups/containers)
  cpu_set_t set;
  CPU_ZERO(&set);
  sched_getaffinity(0, sizeof(set), &set);
  return CPU_COUNT(&set);
#elif IS_WINDOWS()
  // Windows: use GetSystemInfo
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  return (int)si.dwNumberOfProcessors;
#else
  // POSIX fallback (macOS, BSD, etc.)
  return (int)sysconf(_SC_NPROCESSORS_ONLN);
#endif
}

int max_fds(void) {
#if IS_WINDOWS()
  // Windows: return stdio limit (can be increased with _setmaxstdio)
  return _getmaxstdio();
#else
  // POSIX: use getrlimit to get soft limit
  struct rlimit rl;
  getrlimit(RLIMIT_NOFILE, &rl);
  return (int)rl.rlim_cur;
#endif
}

bool set_open_file_limit(uint64_t limit) {
#if IS_WINDOWS()
  return false;
#else
  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) != 0) return false;
  
  rl.rlim_cur = (rlim_t)limit;
  if (limit > rl.rlim_max) rl.rlim_max = (rlim_t)limit; // Try to raise hard limit too if we can (root)
  
  return setrlimit(RLIMIT_NOFILE, &rl) == 0;
#endif
}

bool set_open_file_limit_to_max(void) {
    return set_open_file_limit(get_open_file_limit_limit());
}

uint64_t get_open_file_limit(void) {
#if IS_WINDOWS()
  return (uint64_t)_getmaxstdio();
#else
  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) != 0) return 0;
  return (uint64_t)rl.rlim_cur;
#endif
}

uint64_t get_open_file_limit_limit(void) {
#if IS_WINDOWS()
  return 8192; // Typical max for _setmaxstdio on Windows
#else
  struct rlimit rl;
  if (getrlimit(RLIMIT_NOFILE, &rl) != 0) return 0;
  return (uint64_t)rl.rlim_max;
#endif
}

bool could_set_open_file_limit_to(uint64_t limit) {
#if IS_WINDOWS()
  return limit <= 8192;
#else
  return limit <= get_open_file_limit_limit();
#endif
}

uint find_max_uint(const uint* uint_array, uint uint_array_size) {
  if (uint_array == NULL || uint_array_size == 0) {
    return 0;
  }
  
  uint max_value = uint_array[0];
  FORTO(i, uint_array_size) {
    if (uint_array[i] > max_value) {
      max_value = uint_array[i];
    }
  }
  
  return max_value;
}

int bool_to_exit_code(bool success) {
    return success ? EXIT_SUCCESS : EXIT_FAILURE;
}

#if !IS_WINDOWS()
#include <semaphore.h>

void create_sem_thread_local(sem_t* sem) {
  sem_init(sem, SEM_THREAD_LOCAL, 0);
}

void create_sem_process_shared(sem_t* sem) {
  sem_init(sem, SEM_PROCESS_SHARED, 0);
}

#endif

#ifdef _OPENMP
#include <omp.h>
#endif

void set_num_omp_threads(uint n) {
#ifdef _OPENMP
    omp_set_num_threads((int)n);
#else
    // If OpenMP is not enabled at compile time, try setting environment variable
    // just in case it's dynamically loaded or used by dependencies
    char buf[32];
    snprintf(buf, sizeof(buf), "%u", n);
    #if IS_WINDOWS()
        SetEnvironmentVariable("OMP_NUM_THREADS", buf);
    #else
        setenv("OMP_NUM_THREADS", buf, 1);
    #endif
#endif
}
