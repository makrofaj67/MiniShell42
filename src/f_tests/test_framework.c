#include "../../inc/f_tests/test_framework.h"
#include <stdio.h>
#include <stdlib.h>

/* Test sonuçları global değişkeni */
t_test_result g_test_results = {0, 0};

/* Test kaydetme fonksiyonu */
void register_test(const char *test_name, t_test_func test_func) {
    printf("\n\033[1;34m[TEST]\033[0m %s\n", test_name);
    test_func();
}

/* Test sonuçları için özet fonksiyonu */
void print_test_summary(void) {
    printf("\n\033[1;36m===== TEST SUMMARY =====\033[0m\n");
    printf("Total: %d | \033[1;32mPassed: %d\033[0m | \033[1;31mFailed: %d\033[0m\n\n", 
           g_test_results.passed + g_test_results.failed, 
           g_test_results.passed, 
           g_test_results.failed);
    
    if (g_test_results.failed == 0) {
        printf("\033[1;32mAll tests passed! 🎉\033[0m\n\n");
    } else {
        printf("\033[1;31mSome tests failed! 😢\033[0m\n\n");
    }
}

/* Test fonksiyonlarını çalıştıran main */
int main(void) {
    printf("\033[1;36m===== MiniShell42 Test Suite =====\033[0m\n");
    
    /* Command testleri */
    run_command_tests();
    
    /* Lexer testleri */
    run_lexer_tests();
    
    /* Parser testleri */
    run_parser_tests();
    
    /* Ek testler (malloc, free, error handling, exit status) */
    run_extra_tests();
    
    /* Test sonuçlarını göster */
    print_test_summary();
    
    return (g_test_results.failed > 0);
}