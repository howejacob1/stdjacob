#define _POSIX_C_SOURCE 200809L // For strtok_r, strdup
#include "wer.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper to free token array
static void free_wer_tokens(char** tokens, uint count) {
    FORTO(i, count) {
        free(tokens[i]);
    }
    free(tokens);
}

#define WER_INITIAL_TOKEN_CAPACITY 16

// Tokenize string into array of words
// Returns array of strings, sets *count to number of tokens
static char** to_wer_tokens(const char* text, uint* count) {
    *count = 0;
    // Make a copy to modify
    char* s = strdup_or_die(text);

    // Normalize
    str_to_lower(s);
    remove_str_punctuation(s);

    // First pass: count tokens? No, just realloc.
    uint capacity = WER_INITIAL_TOKEN_CAPACITY;
    char** tokens = (char**)malloc_or_die(capacity * sizeof(char*));
    
    char* saveptr;
    char* token = strtok_r(s, " \t\n\r", &saveptr);
    while (token) {
        if (*count >= capacity) {
            capacity *= 2;
            tokens = (char**)realloc_or_die(tokens, capacity * sizeof(char*));
        }
        tokens[(*count)++] = strdup_or_die(token);
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    free(s);
    return tokens;
}

double wer(const char* str_to_analyze, const char* what_str_should_be) {
    uint n_hyp = 0;
    char** hyp_tokens = to_wer_tokens(str_to_analyze, &n_hyp);
    
    uint n_ref = 0;
    char** ref_tokens = to_wer_tokens(what_str_should_be, &n_ref);
    
    if (n_ref == 0) {
        free_wer_tokens(hyp_tokens, n_hyp);
        free_wer_tokens(ref_tokens, n_ref);
        return n_hyp > 0 ? 1.0f : 0.0f;
    }
    
    // DP Table: (n_ref + 1) x (n_hyp + 1)
    // d[i][j] where i is ref index (0..n_ref), j is hyp index (0..n_hyp)
    
    uint rows = n_ref + 1;
    uint cols = n_hyp + 1;
    
    // Allocate flat array
    uint* d = (uint*)malloc_or_die((size_t)(rows * cols) * sizeof(uint));
    
    // Access macro: d[i][j] -> d[i * cols + j]
    #define D(i, j) d[(i) * cols + (j)]
    
    // Initialize
    FORTO(i, rows) D(i, 0) = i; // Deletions
    FORTO(j, cols) D(0, j) = j; // Insertions
    
    for (uint i = 1; i <= n_ref; i++) {
        for (uint j = 1; j <= n_hyp; j++) {
            // ref_tokens[i-1] vs hyp_tokens[j-1]
            if (streq(ref_tokens[i-1], hyp_tokens[j-1])) {
                D(i, j) = D(i-1, j-1);
            } else {
                uint sub = D(i-1, j-1) + 1;
                uint ins = D(i, j-1) + 1;
                uint del = D(i-1, j) + 1;
                // min3 takes ints, but we have uints. 
                // Since these are small counts, casting to int is safe for min3.
                // Or we should update min3 to take uints? 
                // stdjacob.h defines min3(int, int, int).
                // Let's cast to int for min3, then assign back to uint.
                D(i, j) = (uint)min3((int)sub, (int)ins, (int)del);
            }
        }
    }
    
    double result = (double)D(n_ref, n_hyp) / (double)n_ref;
    
    free(d);
    free_wer_tokens(hyp_tokens, n_hyp);
    free_wer_tokens(ref_tokens, n_ref);
    
    #undef D
    return result;
}
