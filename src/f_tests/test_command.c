#include "../../inc/__minishell.h"
#include "../../inc/f_tests/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Command modülü testleri için mock */
char *mock_readline(const char *prompt) {
    (void)prompt;
    /* Test için sabit bir değer döndür */
    return strdup("test_command");
}

/* Özel komut işleme testleri */
void test_is_command_blank() {
    ASSERT_TRUE(is_command_blank(""), "Boş komut testi");
    ASSERT_TRUE(is_command_blank("  \t \n"), "Boşluk karakterleri testi");
    ASSERT_FALSE(is_command_blank("echo hello"), "Normal komut testi");
    ASSERT_FALSE(is_command_blank("  echo  "), "Boşluklu normal komut testi");
}

void test_has_unclosed_quotes() {
    ASSERT_FALSE(has_unclosed_quotes("echo hello"), "Alıntısız komut testi");
    ASSERT_FALSE(has_unclosed_quotes("echo \"hello\""), "Çift tırnaklı kapanan komut testi");
    ASSERT_FALSE(has_unclosed_quotes("echo 'hello'"), "Tek tırnaklı kapanan komut testi");
    ASSERT_TRUE(has_unclosed_quotes("echo \"hello"), "Kapanmayan çift tırnak testi");
    ASSERT_TRUE(has_unclosed_quotes("echo 'hello"), "Kapanmayan tek tırnak testi");
    ASSERT_FALSE(has_unclosed_quotes("echo \"'hello'\""), "İç içe tırnak testi");
    ASSERT_TRUE(has_unclosed_quotes("echo \"'hello"), "İç içe kapanmayan tırnak testi");
}

void test_parenthesis_status() {
    ASSERT_EQ(0, parenthesis_status("echo hello"), "Parantez olmayan komut testi");
    ASSERT_EQ(0, parenthesis_status("(echo hello)"), "Dengelenmiş parantez testi");
    ASSERT_EQ(0, parenthesis_status("(echo (hello))"), "İç içe dengelenmiş parantez testi");
    ASSERT_EQ(0, parenthesis_status("echo (hello) > (file)"), "Çoklu dengelenmiş parantez testi");
    ASSERT_EQ(1, parenthesis_status("(echo hello"), "Kapanmayan parantez testi");
    ASSERT_EQ(1, parenthesis_status("((echo hello)"), "Kapanmayan iç içe parantez testi");
    ASSERT_EQ(-1, parenthesis_status(")echo hello"), "Açılmayan parantez testi");
    ASSERT_EQ(-1, parenthesis_status("echo hello)"), "Fazladan kapanan parantez testi");
}

void test_pstack_operations() {
    t_pstack *stack = init_pstack();
    ASSERT_PTR_NOT_NULL(stack, "Stack oluşturma testi");
    ASSERT_EQ(0, stack->size, "Başlangıç stack boyutu testi");
    
    push_pstack(stack);
    ASSERT_EQ(1, stack->size, "Stack push sonrası boyut testi");
    ASSERT_PTR_NOT_NULL(stack->head, "Stack head testi");
    
    push_pstack(stack);
    ASSERT_EQ(2, stack->size, "İkinci push sonrası boyut testi");
    
    pop_pstack(stack);
    ASSERT_EQ(1, stack->size, "Pop sonrası boyut testi");
    
    pop_pstack(stack);
    ASSERT_EQ(0, stack->size, "Son pop sonrası boyut testi");
    
    free_pstack(stack);
}

/* Tüm Command testlerini çalıştır */
void run_command_tests() {
    printf("\033[1;36m===== Command Module Tests =====\033[0m\n");
    
    register_test("is_command_blank", test_is_command_blank);
    register_test("has_unclosed_quotes", test_has_unclosed_quotes);
    register_test("parenthesis_status", test_parenthesis_status);
    register_test("pstack_operations", test_pstack_operations);
}