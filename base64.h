#ifndef BASE64_H
#define BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

// Calculate the size needed for base64 encoding
size_t base64_encode_size(size_t input_len);

// Calculate the maximum size needed for base64 decoding
size_t base64_decode_size(size_t input_len);

// Encode binary data to base64 string
// Returns allocated string (caller must free), or NULL on error
char* binary_to_base64(const uchar* data, size_t input_len);

// Decode base64 string to binary data
// Returns allocated buffer (caller must free), or NULL on error
// Sets *output_len to the actual decoded length
uchar* base64_to_binary(const char* input, size_t* output_len);

// Check if a string is valid base64
int base64_is_valid(const char* input);

#ifdef __cplusplus
}
#endif

#endif // BASE64_H
