#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

#include "../__minishell.h"
#include <stdio.h>
#include <stdlib.h>

/* Test çalıştırma yapısı */
typedef struct s_test_result {
    int passed;
    int failed;
} t_test_result;

extern t_test_result g_test_results;

/* Test fonksiyonu imzası */
typedef void (*t_test_func)(void);

/* Test kaydetme fonksiyonu */
void register_test(const char *test_name, t_test_func test_func);

/* Assertion makroları */
#define ASSERT_TRUE(condition, message) \
    do { \
        if (condition) { \
            printf("\033[1;32m[PASS]\033[0m %s\n", message); \
            g_test_results.passed++; \
        } else { \
            printf("\033[1;31m[FAIL]\033[0m %s\n", message); \
            g_test_results.failed++; \
        } \
    } while (0)

#define ASSERT_FALSE(condition, message) ASSERT_TRUE(!(condition), message)

#define ASSERT_STR_EQ(expected, actual, message) \
    do { \
        if (expected == NULL && actual == NULL) { \
            printf("\033[1;32m[PASS]\033[0m %s (Both NULL)\n", message); \
            g_test_results.passed++; \
        } else if (expected == NULL || actual == NULL) { \
            printf("\033[1;31m[FAIL]\033[0m %s (One is NULL)\n", message); \
            g_test_results.failed++; \
        } else if (strcmp(expected, actual) == 0) { \
            printf("\033[1;32m[PASS]\033[0m %s (\"%s\" == \"%s\")\n", message, expected, actual); \
            g_test_results.passed++; \
        } else { \
            printf("\033[1;31m[FAIL]\033[0m %s (Expected: \"%s\", Got: \"%s\")\n", message, expected, actual); \
            g_test_results.failed++; \
        } \
    } while (0)

#define ASSERT_EQ(expected, actual, message) \
    do { \
        if ((expected) == (actual)) { \
            printf("\033[1;32m[PASS]\033[0m %s (%d == %d)\n", message, (int)(expected), (int)(actual)); \
            g_test_results.passed++; \
        } else { \
            printf("\033[1;31m[FAIL]\033[0m %s (Expected: %d, Got: %d)\n", message, (int)(expected), (int)(actual)); \
            g_test_results.failed++; \
        } \
    } while (0)

#define ASSERT_PTR_NOT_NULL(ptr, message) \
    do { \
        if ((ptr) != NULL) { \
            printf("\033[1;32m[PASS]\033[0m %s (Pointer is not NULL)\n", message); \
            g_test_results.passed++; \
        } else { \
            printf("\033[1;31m[FAIL]\033[0m %s (Expected non-NULL pointer)\n", message); \
            g_test_results.failed++; \
        } \
    } while (0)

#define ASSERT_PTR_NULL(ptr, message) \
    do { \
        if ((ptr) == NULL) { \
            printf("\033[1;32m[PASS]\033[0m %s (Pointer is NULL)\n", message); \
            g_test_results.passed++; \
        } else { \
            printf("\033[1;31m[FAIL]\033[0m %s (Expected NULL pointer)\n", message); \
            g_test_results.failed++; \
        } \
    } while (0)

/* Test sonuçları için özet fonksiyonu */
void print_test_summary(void);

/* Test modülleri fonksiyon prototipleri */
void run_command_tests(void);
void run_lexer_tests(void);
void run_parser_tests(void);
void run_extra_tests(void);  /* Yeni eklenen test fonksiyonu */

#endif /* TEST_FRAMEWORK_H */