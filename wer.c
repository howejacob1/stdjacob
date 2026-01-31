#define _POSIX_C_SOURCE 200809L // For strtok_r, strdup
#include "wer.h"
#include "stdjacob.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Helper to free token array
static void free_wer_tokens(char** tokens, int count) {
    FORTO(i, (uint)count) {
        free(tokens[i]);
    }
    free(tokens);
}

// Tokenize string into array of words
// Returns array of strings, sets *count to number of tokens
static char** to_wer_tokens(const char* text, int* count) {
    *count = 0;
    if (!text) return NULL;

    // Make a copy to modify
    char* s = strdup(text);
    if (!s) return NULL;

    // Normalize
    str_to_lower(s);
    remove_str_punctuation(s);

    // First pass: count tokens? No, just realloc.
    int capacity = 16;
    char** tokens = (char**)malloc_or_die(capacity * sizeof(char*));
    
    char* saveptr;
    char* token = strtok_r(s, " \t\n\r", &saveptr);
    while (token) {
        if (*count >= capacity) {
            capacity *= 2;
            tokens = (char**)realloc_or_die(tokens, capacity * sizeof(char*));
        }
        tokens[(*count)++] = strdup(token);
        token = strtok_r(NULL, " \t\n\r", &saveptr);
    }

    free(s);
    return tokens;
}

double wer(const char* str_to_analyze, const char* what_str_should_be) {
    int n_hyp = 0;
    char** hyp_tokens = to_wer_tokens(str_to_analyze, &n_hyp);
    
    int n_ref = 0;
    char** ref_tokens = to_wer_tokens(what_str_should_be, &n_ref);
    
    if (n_ref == 0) {
        free_wer_tokens(hyp_tokens, n_hyp);
        free_wer_tokens(ref_tokens, n_ref);
        return n_hyp > 0 ? 1.0 : 0.0;
    }
    
    // DP Table: (n_ref + 1) x (n_hyp + 1)
    // d[i][j] where i is ref index (0..n_ref), j is hyp index (0..n_hyp)
    
    int rows = n_ref + 1;
    int cols = n_hyp + 1;
    
    // Allocate flat array
    int* d = (int*)malloc_or_die((size_t)(rows * cols) * sizeof(int));
    
    // Access macro: d[i][j] -> d[i * cols + j]
    #define D(i, j) d[(i) * cols + (j)]
    
    // Initialize
    FORTO(i, (uint)rows) D(i, 0) = i; // Deletions
    FORTO(j, (uint)cols) D(0, j) = j; // Insertions
    
    for (int i = 1; i <= n_ref; i++) {
        for (int j = 1; j <= n_hyp; j++) {
            // ref_tokens[i-1] vs hyp_tokens[j-1]
            if (streq(ref_tokens[i-1], hyp_tokens[j-1])) {
                D(i, j) = D(i-1, j-1);
            } else {
                int sub = D(i-1, j-1) + 1;
                int ins = D(i, j-1) + 1;
                int del = D(i-1, j) + 1;
                D(i, j) = min3(sub, ins, del);
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
