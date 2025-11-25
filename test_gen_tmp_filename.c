#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "stdjacob.h"

int main(void) {
    char buffer1[TMP_FILENAME_MAX];
    char buffer2[TMP_FILENAME_MAX];
    struct stat st;
    int result;
    int tests_passed = 0;
    int tests_failed = 0;
    
    printf("Testing gen_tmp_filename()...\n\n");
    
    // Test 1: Basic functionality
    printf("Test 1: gen_tmp_filename() returns success... ");
    result = gen_tmp_filename(buffer1, sizeof(buffer1));
    if (result == 0 && strlen(buffer1) > 0) {
        printf("PASS (generated: %s)\n", buffer1);
        tests_passed++;
    } else {
        printf("FAIL (result=%d, buffer='%s')\n", result, buffer1);
        tests_failed++;
    }
    
    // Test 2: File exists
    printf("Test 2: Generated file exists... ");
    if (stat(buffer1, &st) == 0) {
        printf("PASS\n");
        tests_passed++;
    } else {
        printf("FAIL (file not found)\n");
        tests_failed++;
    }
    
    // Test 3: Multiple calls generate different filenames
    printf("Test 3: Multiple calls generate unique names... ");
    result = gen_tmp_filename(buffer2, sizeof(buffer2));
    if (result == 0 && strcmp(buffer1, buffer2) != 0) {
        printf("PASS\n");
        printf("  First:  %s\n", buffer1);
        printf("  Second: %s\n", buffer2);
        tests_passed++;
    } else {
        printf("FAIL (names are the same or error)\n");
        tests_failed++;
    }
    
    // Test 4: Second file also exists
    printf("Test 4: Second file exists... ");
    if (stat(buffer2, &st) == 0) {
        printf("PASS\n");
        tests_passed++;
    } else {
        printf("FAIL (file not found)\n");
        tests_failed++;
    }
    
    // Cleanup
    printf("\nCleaning up temporary files... ");
    unlink(buffer1);
    unlink(buffer2);
    printf("done\n");
    
    // Summary
    printf("\n========================================\n");
    printf("Tests passed: %d/%d\n", tests_passed, tests_passed + tests_failed);
    printf("Tests failed: %d/%d\n", tests_failed, tests_passed + tests_failed);
    printf("========================================\n");
    
    return (tests_failed == 0) ? 0 : 1;
}

