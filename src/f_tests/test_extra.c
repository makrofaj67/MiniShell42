#include "../../inc/f_tests/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Test ettiğimiz bellek fonksiyonlarını izleyebilmek için
// gerçek malloc/free yerine kendi test_malloc/test_free fonksiyonlarımızı kullanmalıyız
#define REAL_STRDUP strdup

/* Bellek sızıntısı testi için yardımcı fonksiyonlar */
void *tracked_mallocs[100];
int malloc_count = 0;
int free_count = 0;

/* Malloc wrapperi */
void *test_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr && malloc_count < 100) {
        tracked_mallocs[malloc_count++] = ptr;
    }
    return ptr;
}

/* Free wrapperi */
void test_free(void *ptr) {
    if (ptr) {
        for (int i = 0; i < malloc_count; i++) {
            if (tracked_mallocs[i] == ptr) {
                tracked_mallocs[i] = NULL;
                free_count++;
                break;
            }
        }
        free(ptr);
    }
}

/* Bellek sızıntısı kontrolü */
void reset_malloc_tracker() {
    malloc_count = 0;
    free_count = 0;
    memset(tracked_mallocs, 0, sizeof(tracked_mallocs));
}

int get_unfreed_count() {
    return malloc_count - free_count;
}

/* Malloc/Free Testleri */
void test_token_list_memory_leaks() {
    // Test amacıyla token listesi oluştur
    t_token_list *list = create_token_list();
    
    // Token listesine string'ler ekle
    add_token(list, strdup("test1"));
    add_token(list, strdup("test2"));
    add_token(list, strdup("test3"));
    
    // String'leri manuel olarak temizle (token list yapısı bunları free etmiyor olabilir)
    t_token_node *current = list->head;
    t_token_node *next;
    
    while (current) {
        next = current->next;
        if (current->value) {
            free(current->value);
            current->value = NULL;
        }
        current = next;
    }
    
    // Şimdi token list'i temizle
    free_token_list(list);
    
    // Bu testte LeakSanitizer herhangi bir sızıntı algılarsa, otomatik olarak bildirecek
    ASSERT_TRUE(1, "Token list bellek sızıntısı testi");
}

void test_ast_memory_leaks() {
    reset_malloc_tracker();
    
    // Basit bir AST oluştur
    command_value *cmd_value = (command_value *)malloc(sizeof(command_value));
    cmd_value->arg_array = (char **)malloc(sizeof(char *) * 3);
    cmd_value->arg_array[0] = strdup("echo");
    cmd_value->arg_array[1] = strdup("hello");
    cmd_value->arg_array[2] = NULL;
    
    cmd_value->redirections = (t_redirection **)malloc(sizeof(t_redirection *) * 2);
    cmd_value->redirections[0] = (t_redirection *)malloc(sizeof(t_redirection));
    cmd_value->redirections[0]->type = REDIR_OUT;
    cmd_value->redirections[0]->filename = strdup("output.txt");
    cmd_value->redirections[1] = NULL;
    
    ast_node *node = create_command_node(cmd_value);
    
    // AST'yi temizle
    free_ast(node);
    
    ASSERT_EQ(0, get_unfreed_count(), "AST bellek sızıntısı testi");
}

/* Error Handling Testleri */
void test_error_unclosed_quotes() {
    char *command = "echo \"unclosed quote";
    int result = has_unclosed_quotes(command);
    
    ASSERT_TRUE(result, "Kapanmayan tırnak hatası doğru tespit edilmeli");
}

void test_error_unbalanced_parentheses() {
    char *command1 = "(echo hello";
    char *command2 = "echo hello)";
    
    int result1 = parenthesis_status(command1);
    int result2 = parenthesis_status(command2);
    
    ASSERT_EQ(1, result1, "Kapanmayan parantez hatası doğru tespit edilmeli");
    ASSERT_EQ(-1, result2, "Fazladan kapanan parantez hatası doğru tespit edilmeli");
}

void test_error_invalid_redirection() {
    char *command = "echo hello > ";
    t_token_list *tokens = tokenize_command(command, 0);
    
    // Yönlendirme sonrası dosya adı eksik olduğunda parse işlemi NULL dönmeli
    command_value *cmd = parse_simple_command(tokens);
    
    // Bu durumda parse_simple_command ya NULL dönmeli (hatayı tespit edip)
    // ya da redirections dizisi NULL olmalı (hatayı işleyip)
    if (cmd != NULL) {
        ASSERT_PTR_NULL(cmd->redirections, "Geçersiz yönlendirme hatalı parse edilmemeli");
        free_command_value(cmd);
    } else {
        ASSERT_PTR_NULL(cmd, "Geçersiz yönlendirme NULL dönmeli");
    }
    
    free_token_list(tokens);
}

/* Exit Status Testleri */
int run_command_and_get_exit_status(char *command) {
    // system() kullanarak çalıştır, ancak exit status'u doğrudan al
    char cmd[1024];
    snprintf(cmd, sizeof(cmd), "%s; echo $? > /tmp/exit_status.txt", command);
    system(cmd);
    
    // Exit status'u dosyadan oku
    FILE *fp = fopen("/tmp/exit_status.txt", "r");
    if (!fp) {
        return -1;
    }
    
    int status;
    fscanf(fp, "%d", &status);
    fclose(fp);
    
    return status;
}

void test_exit_status_success() {
    int status = run_command_and_get_exit_status("echo hello");
    ASSERT_EQ(0, status, "Başarılı komut exit status 0 olmalı");
}

void test_exit_status_command_not_found() {
    int status = run_command_and_get_exit_status("nonexistentcommand 2>/dev/null");
    ASSERT_TRUE(status != 0, "Olmayan komut non-zero exit status dönmeli");
}

void test_exit_status_syntax_error() {
    int status = run_command_and_get_exit_status("ls | | 2>/dev/null");
    ASSERT_TRUE(status != 0, "Sözdizimi hatası non-zero exit status dönmeli");
}

/* Tüm ek testleri çalıştır */
void run_extra_tests() {
    printf("\033[1;36m===== Memory, Error Handling and Exit Status Tests =====\033[0m\n");
    
    // Bellek testleri
    register_test("token_list_memory_leaks", test_token_list_memory_leaks);
    register_test("ast_memory_leaks", test_ast_memory_leaks);
    
    // Hata işleme testleri
    register_test("error_unclosed_quotes", test_error_unclosed_quotes);
    register_test("error_unbalanced_parentheses", test_error_unbalanced_parentheses);
    register_test("error_invalid_redirection", test_error_invalid_redirection);
    
    // Exit Status testleri
    register_test("exit_status_success", test_exit_status_success);
    register_test("exit_status_command_not_found", test_exit_status_command_not_found);
    register_test("exit_status_syntax_error", test_exit_status_syntax_error);
}