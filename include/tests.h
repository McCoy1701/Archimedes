#ifndef __TESTS_H__
#define __TESTS_H__

#include <stdio.h>
#include <time.h>

// Use high-resolution timing if available
#ifdef _POSIX_C_SOURCE
#include <sys/time.h>
#define USE_HIGH_RES_TIMER 1
#else
#define USE_HIGH_RES_TIMER 0
#endif

// Test color codes for better visibility
#define TEST_COLOR_RESET   "\033[0m"
#define TEST_COLOR_GREEN   "\033[32m"
#define TEST_COLOR_RED     "\033[31m"
#define TEST_COLOR_BLUE    "\033[34m"
#define TEST_COLOR_YELLOW  "\033[33m"
#define TEST_COLOR_BOLD_WHITE "\033[1;37m"

// Global test counters and timing (to be defined in each test file)
extern int total_tests;
extern int tests_passed;
extern int tests_failed;

// Global timing variables for suite-level timing
static double _test_suite_start_time;
static double _total_test_time = 0.0;

// High-resolution timer function
static double get_time_precise(void) {
#if USE_HIGH_RES_TIMER
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
#else
    return (double)clock() / CLOCKS_PER_SEC;
#endif
}

// Simple test framework macros
#define TEST_ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf(TEST_COLOR_RED "❌ FAIL: %s" TEST_COLOR_RESET "\n", message); \
            printf("   Line %d in %s\n", __LINE__, __FILE__); \
            return 0; \
        } else { \
            printf(TEST_COLOR_GREEN "✅ PASS: %s" TEST_COLOR_RESET "\n", message); \
        } \
    } while(0)

#define RUN_TEST(test_func) \
    do { \
        printf(TEST_COLOR_BLUE "\n🧪 Running %s...\n" TEST_COLOR_RESET, #test_func); \
        double start_time = get_time_precise(); \
        int test_result = test_func(); \
        double end_time = get_time_precise(); \
        double elapsed_time = end_time - start_time; \
        _total_test_time += elapsed_time; \
        printf(TEST_COLOR_BOLD_WHITE " (Took %.6f seconds)" TEST_COLOR_RESET "\n", elapsed_time); \
        if (test_result) { \
            printf(TEST_COLOR_GREEN "✅ %s PASSED" TEST_COLOR_RESET "\n", #test_func); \
            tests_passed++; \
        } else { \
            printf(TEST_COLOR_RED "❌ %s FAILED" TEST_COLOR_RESET "\n", #test_func); \
            tests_failed++; \
        } \
        total_tests++; \
    } while(0)

// Test suite header and footer macros
#define TEST_SUITE_START(suite_name) \
    _test_suite_start_time = get_time_precise(); \
    _total_test_time = 0.0; \
    printf(TEST_COLOR_YELLOW "🚀 Starting %s\n" TEST_COLOR_RESET, suite_name); \
    printf("==============================================\n");

#define TEST_SUITE_END() \
    double _suite_end_time = get_time_precise(); \
    double _total_suite_time = _suite_end_time - _test_suite_start_time; \
    printf(TEST_COLOR_YELLOW "\n📊 Test Results Summary\n" TEST_COLOR_RESET); \
    printf("======================\n"); \
    printf("Total Tests: %d\n", total_tests); \
    printf(TEST_COLOR_GREEN "✅ Passed: %d\n" TEST_COLOR_RESET, tests_passed); \
    printf(TEST_COLOR_RED "❌ Failed: %d\n" TEST_COLOR_RESET, tests_failed); \
    printf(TEST_COLOR_BOLD_WHITE "⏱️  Total Test Time: %.6f seconds\n" TEST_COLOR_RESET, _total_test_time); \
    printf(TEST_COLOR_BOLD_WHITE "🕒 Total Suite Time: %.6f seconds\n" TEST_COLOR_RESET, _total_suite_time); \
    printf(TEST_COLOR_BOLD_WHITE "📊 Framework Overhead: %.6f seconds\n" TEST_COLOR_RESET, _total_suite_time - _total_test_time); \
    if (tests_failed == 0) { \
        printf(TEST_COLOR_GREEN "\n🎉 All tests passed! Great job!\n" TEST_COLOR_RESET); \
        return 0; \
    } else { \
        printf(TEST_COLOR_RED "\n💥 Some tests failed. Check the output above.\n" TEST_COLOR_RESET); \
        return 1; \
    }

// Quiet version for when running multiple test files
#define TEST_SUITE_END_QUIET() \
    double _suite_end_time = get_time_precise(); \
    double _total_suite_time = _suite_end_time - _test_suite_start_time; \
    printf("Total Tests: %d\n", total_tests); \
    printf("✅ Passed: %d\n", tests_passed); \
    printf("❌ Failed: %d\n", tests_failed); \
    printf("⏱️  Total Test Time: %.6f seconds\n", _total_test_time); \
    printf("🕒 Total Suite Time: %.6f seconds\n", _total_suite_time); \
    printf("📊 Framework Overhead: %.6f seconds\n", _total_suite_time - _total_test_time); \
    if (tests_failed == 0) { \
        return 0; \
    } else { \
        return 1; \
    }

#endif
