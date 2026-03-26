#ifndef WER_H
#define WER_H

#ifdef __cplusplus
extern "C" {
#endif

// Calculate Word Error Rate (WER)
// Returns a double between 0.0 and 1.0 (or higher)
// str_to_analyze: The hypothesis string (what was transcribed)
// what_str_should_be: The reference string (ground truth)
double wer(const char* str_to_analyze, const char* what_str_should_be);
float calculate_wer(const char* str_to_analyze, const char* what_str_should_be);

#ifdef __cplusplus
}
#endif

#endif // WER_H
