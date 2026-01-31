#include "base64.h"
#include "stdjacob.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

static const char ENCODE_TABLE[] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

#define BASE64_PAD '='
// Base64 alphabet offsets: A-Z=0-25, a-z=26-51, 0-9=52-61, +=62, /=63
#define B64_UPPER_START   0   // A-Z starts at index 0
#define B64_LOWER_START   26  // a-z starts at index 26 (after 26 uppercase)
#define B64_DIGIT_START   52  // 0-9 starts at index 52 (after 26+26 letters)
#define B64_PLUS_VALUE    62
#define B64_SLASH_VALUE   63
#define B64_INVALID       255

static uchar base64_upper_to_6_bits(uchar c) {
    return (uchar)(c - 'A' + B64_UPPER_START);
}

static uchar base64_lower_to_6_bits(uchar c) {
    return (uchar)(c - 'a' + B64_LOWER_START);
}

static uchar base64_digit_to_6_bits(uchar c) {
    return (uchar)(c - '0' + B64_DIGIT_START);
}

// Decode character to 6-bit value (fails assertion for invalid)
static uchar base64_to_6_bits(uchar c) {
    if (isupper(c)) return base64_upper_to_6_bits(c);
    if (islower(c)) return base64_lower_to_6_bits(c);
    if (isdigit(c)) return base64_digit_to_6_bits(c);
    if (c == '+') return B64_PLUS_VALUE;
    if (c == '/') return B64_SLASH_VALUE;
    
    // Should not happen if validated first, or caller handles validation
    // But per instructions, we should fail assertion instead of returning invalid
    // However, base64_is_valid calls this indirectly via is_valid_char which might rely on B64_INVALID return
    // Let's check is_valid_char usage.
    // Original code: return B64_INVALID;
    // Requested: "instead of returning invalid, we should fail an assertion or soemthing."
    
    // If this function is used for decoding known valid base64, assertion is fine.
    // If it's used for validation, we need a way to check without crashing.
    // Let's split validation logic from decoding logic.
    
    // But wait, is_valid_char uses this.
    // Let's change is_valid_char to not use this function, or make a separate validator.
    
    // Actually, let's keep B64_INVALID for validation purposes but assert in the decoding path?
    // Or just make a `base64_char_value_or_die`?
    
    // Let's stick to the prompt: "instead of returning invalid, we should fail an assertion or soemthing."
    // This implies we assume input is valid when calling this for value extraction.
    // I will rename this to `base64_char_value` and add `base64_is_valid_char` separately.
    
    fprintf(stderr, "Invalid base64 character: %c (%d)\n", c, c);
    abort();
    return 0; // Unreachable
}

static bool base64_is_valid_char(char c) {
    return isupper(c) || islower(c) || isdigit(c) || c == '+' || c == '/' || c == BASE64_PAD;
}

size_t base64_encode_size(size_t input_len) {
    return ((input_len + 2) / 3) * 4 + 1;  // +1 for null terminator
}

size_t base64_decode_size(size_t input_len) {
    return (input_len / 4) * 3;
}

static int encode_triplet(const uchar* in, int len, char* out) {
    uint triplet = 0;
    
    triplet |= (len > 0) ? (in[0] << 16) : 0;
    triplet |= (len > 1) ? (in[1] << 8)  : 0;
    triplet |= (len > 2) ? in[2]         : 0;
    
    out[0] = ENCODE_TABLE[(triplet >> 18) & 0x3F];
    out[1] = ENCODE_TABLE[(triplet >> 12) & 0x3F];
    out[2] = (len > 1) ? ENCODE_TABLE[(triplet >> 6) & 0x3F] : BASE64_PAD;
    out[3] = (len > 2) ? ENCODE_TABLE[triplet & 0x3F] : BASE64_PAD;
    
    return 4;
}

char* binary_to_base64(const uchar* data, size_t input_len) {
    if (!data && input_len > 0) return NULL;
    
    size_t output_len = base64_encode_size(input_len);
    char* output = (char*)malloc(output_len);
    if (!output) return NULL;
    
    size_t i = 0;
    size_t j = 0;
    
    while (i < input_len) {
        int remaining = (int)(input_len - i);
        if (remaining > 3) remaining = 3;
        j += encode_triplet(data + i, remaining, output + j);
        i += 3;
    }
    
    output[j] = '\0';
    return output;
}

static int decode_quartet(const char* in, uchar* out) {
    uchar a = base64_to_6_bits((uchar)in[0]);
    uchar b = base64_to_6_bits((uchar)in[1]);
    uchar c = (in[2] == BASE64_PAD) ? 0 : base64_to_6_bits((uchar)in[2]);
    uchar d = (in[3] == BASE64_PAD) ? 0 : base64_to_6_bits((uchar)in[3]);
    
    out[0] = (uchar)((a << 2) | (b >> 4));
    
    if (in[2] == BASE64_PAD) return 1;
    out[1] = (uchar)((b << 4) | (c >> 2));
    
    if (in[3] == BASE64_PAD) return 2;
    out[2] = (uchar)((c << 6) | d);
    
    return 3;
}

uchar* base64_to_binary(const char* input, size_t* output_len) {
    if (!input || !output_len) return NULL;
    
    size_t input_len = strlen(input);
    if (input_len == 0) {
        *output_len = 0;
        return (uchar*)calloc_or_die(1, 1);
    }
    
    // Input must be multiple of 4
    if (!IS_MULTIPLE_OF(input_len, 4)) return NULL;
    
    // Validate first
    if (!base64_is_valid(input)) return NULL;

    size_t max_output = base64_decode_size(input_len);
    uchar* output = (uchar*)malloc(max_output);
    if (!output) return NULL;
    
    size_t i = 0;
    size_t j = 0;
    
    while (i < input_len) {
        j += decode_quartet(input + i, output + j);
        i += 4;
    }
    
    *output_len = j;
    return output;
}

int base64_is_valid(const char* input) {
    if (!input) return 0;
    
    size_t len = strlen(input);
    if (len == 0) return 1;  // Empty string is valid
    if (!IS_MULTIPLE_OF(len, 4)) return 0;
    
    for (size_t i = 0; i < len; i++) {
        if (!base64_is_valid_char(input[i])) return 0;
        // Padding can only appear at end
        if (input[i] == BASE64_PAD && i < len - 2) return 0;
    }
    
    return 1;
}
