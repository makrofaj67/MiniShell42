#include "../../inc/f_tests/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* AST Doğrulama yardımcı fonksiyonları */
int validate_command_node(ast_node *node, char **expected_args, int arg_count) {
    if (!node || node->type != COMMAND_NODE || !node->value)
        return 0;
    
    command_value *cmd = node->value;
    if (!cmd->arg_array)
        return 0;
    
    // Argüman sayısını kontrol et
    int actual_count = 0;
    while (cmd->arg_array[actual_count] != NULL)
        actual_count++;
    
    if (actual_count != arg_count)
        return 0;
    
    // Tüm argümanları kontrol et
    for (int i = 0; i < arg_count; i++) {
        if (strcmp(cmd->arg_array[i], expected_args[i]) != 0)
            return 0;
    }
    
    return 1;
}

/* Parser fonksiyonları testleri */
void test_parse_simple_command() {
    char *command = "echo hello world";
    t_token_list *tokens = tokenize_command(command, 0);
    command_value *cmd = parse_simple_command(tokens);
    
    ASSERT_PTR_NOT_NULL(cmd, "Komut değeri oluşturulmalı");
    ASSERT_PTR_NOT_NULL(cmd->arg_array, "Komut argüman dizisi oluşturulmalı");
    
    // Argümanları kontrol et
    ASSERT_STR_EQ("echo", cmd->arg_array[0], "İlk argüman 'echo' olmalı");
    ASSERT_STR_EQ("hello", cmd->arg_array[1], "İkinci argüman 'hello' olmalı");
    ASSERT_STR_EQ("world", cmd->arg_array[2], "Üçüncü argüman 'world' olmalı");
    ASSERT_PTR_NULL(cmd->arg_array[3], "Dördüncü argüman NULL olmalı");
    
    // Temizlik
    free_command_value(cmd);
    free_token_list(tokens);
}

void test_parse_command_with_redirections() {
    char *command = "echo hello > output.txt";
    t_token_list *tokens = tokenize_command(command, 0);
    command_value *cmd = parse_simple_command(tokens);
    
    ASSERT_PTR_NOT_NULL(cmd, "Komut değeri oluşturulmalı");
    ASSERT_PTR_NOT_NULL(cmd->arg_array, "Komut argüman dizisi oluşturulmalı");
    ASSERT_PTR_NOT_NULL(cmd->redirections, "Yönlendirme dizisi oluşturulmalı");
    
    // Argümanları kontrol et
    ASSERT_STR_EQ("echo", cmd->arg_array[0], "İlk argüman 'echo' olmalı");
    ASSERT_STR_EQ("hello", cmd->arg_array[1], "İkinci argüman 'hello' olmalı");
    ASSERT_PTR_NULL(cmd->arg_array[2], "Üçüncü argüman NULL olmalı");
    
    // Yönlendirmeleri kontrol et
    ASSERT_PTR_NOT_NULL(cmd->redirections[0], "İlk yönlendirme var olmalı");
    ASSERT_EQ(REDIR_OUT, cmd->redirections[0]->type, "Yönlendirme tipi REDIR_OUT olmalı");
    ASSERT_STR_EQ("output.txt", cmd->redirections[0]->filename, "Dosya adı 'output.txt' olmalı");
    ASSERT_PTR_NULL(cmd->redirections[1], "İkinci yönlendirme NULL olmalı");
    
    // Temizlik
    free_command_value(cmd);
    free_token_list(tokens);
}

void test_parse_command_with_multiple_redirections() {
    char *command = "cat < input.txt > output.txt";
    t_token_list *tokens = tokenize_command(command, 0);
    command_value *cmd = parse_simple_command(tokens);
    
    ASSERT_PTR_NOT_NULL(cmd, "Komut değeri oluşturulmalı");
    
    // Argümanları kontrol et
    ASSERT_STR_EQ("cat", cmd->arg_array[0], "İlk argüman 'cat' olmalı");
    ASSERT_PTR_NULL(cmd->arg_array[1], "İkinci argüman NULL olmalı");
    
    // Yönlendirmeleri kontrol et
    int redir_in_found = 0;
    int redir_out_found = 0;
    
    for (int i = 0; cmd->redirections[i] != NULL; i++) {
        if (cmd->redirections[i]->type == REDIR_IN && 
            strcmp(cmd->redirections[i]->filename, "input.txt") == 0) {
            redir_in_found = 1;
        }
        if (cmd->redirections[i]->type == REDIR_OUT && 
            strcmp(cmd->redirections[i]->filename, "output.txt") == 0) {
            redir_out_found = 1;
        }
    }
    
    ASSERT_TRUE(redir_in_found, "REDIR_IN yönlendirmesi 'input.txt' bulunmalı");
    ASSERT_TRUE(redir_out_found, "REDIR_OUT yönlendirmesi 'output.txt' bulunmalı");
    
    // Temizlik
    free_command_value(cmd);
    free_token_list(tokens);
}

void test_parse_simple_pipe() {
    char *command = "ls -l | grep test";
    t_token_list *tokens = tokenize_command(command, 0);
    
    // Son pipe'ı bul
    t_token_node *pipe_location = find_last_pipe(tokens);
    ASSERT_PTR_NOT_NULL(pipe_location, "Pipe token bulunmalı");
    
    // Pipe parse işlemi
    ast_node **pipe_parts = parse_simple_pipe(tokens, pipe_location);
    ASSERT_PTR_NOT_NULL(pipe_parts, "Pipe parçaları oluşturulmalı");
    ASSERT_PTR_NOT_NULL(pipe_parts[0], "Sol taraf oluşturulmalı");
    ASSERT_PTR_NOT_NULL(pipe_parts[1], "Sağ taraf oluşturulmalı");
    
    // Sol taraf doğrulama (ls -l)
    char *left_expected[] = {"ls", "-l"};
    ASSERT_TRUE(validate_command_node(pipe_parts[0], left_expected, 2), 
                "Sol komut 'ls -l' doğru parse edilmeli");
    
    // Sağ taraf doğrulama (grep test)
    char *right_expected[] = {"grep", "test"};
    ASSERT_TRUE(validate_command_node(pipe_parts[1], right_expected, 2),
                "Sağ komut 'grep test' doğru parse edilmeli");
    
    // Temizlik
    free_ast(pipe_parts[0]);
    free_ast(pipe_parts[1]);
    free(pipe_parts);
    free_token_list(tokens);
}

void test_parse_tokens_simple() {
    char *command = "echo hello";
    t_token_list *tokens = tokenize_command(command, 0);
    ast_node *root = parse_tokens(tokens);
    
    ASSERT_PTR_NOT_NULL(root, "AST kökü oluşturulmalı");
    ASSERT_EQ(COMMAND_NODE, root->type, "Kök tipi COMMAND_NODE olmalı");
    
    char *expected[] = {"echo", "hello"};
    ASSERT_TRUE(validate_command_node(root, expected, 2), 
                "Komut 'echo hello' doğru parse edilmeli");
    
    // Temizlik
    free_ast(root);
    free_token_list(tokens);
}

void test_parse_tokens_pipe() {
    char *command = "ls -l | grep test";
    t_token_list *tokens = tokenize_command(command, 0);
    ast_node *root = parse_tokens(tokens);
    
    ASSERT_PTR_NOT_NULL(root, "AST kökü oluşturulmalı");
    ASSERT_EQ(PIPE_NODE, root->type, "Kök tipi PIPE_NODE olmalı");
    ASSERT_PTR_NOT_NULL(root->left, "Sol alt ağaç oluşturulmalı");
    ASSERT_PTR_NOT_NULL(root->right, "Sağ alt ağaç oluşturulmalı");
    
    // Sol taraf doğrulama (ls -l)
    char *left_expected[] = {"ls", "-l"};
    ASSERT_TRUE(validate_command_node(root->left, left_expected, 2), 
                "Sol komut 'ls -l' doğru parse edilmeli");
    
    // Sağ taraf doğrulama (grep test)
    char *right_expected[] = {"grep", "test"};
    ASSERT_TRUE(validate_command_node(root->right, right_expected, 2),
                "Sağ komut 'grep test' doğru parse edilmeli");
    
    // Temizlik
    free_ast(root);
    free_token_list(tokens);
}

void test_parse_tokens_complex_pipe() {
    char *command = "ls -l | grep test | wc -l";
    t_token_list *tokens = tokenize_command(command, 0);
    ast_node *root = parse_tokens(tokens);
    
    ASSERT_PTR_NOT_NULL(root, "AST kökü oluşturulmalı");
    ASSERT_EQ(PIPE_NODE, root->type, "Kök tipi PIPE_NODE olmalı");
    ASSERT_PTR_NOT_NULL(root->left, "Sol alt ağaç oluşturulmalı");
    ASSERT_PTR_NOT_NULL(root->right, "Sağ alt ağaç oluşturulmalı");
    
    // Sol alt ağaç pipe node olmalı (ls -l | grep test)
    ASSERT_EQ(PIPE_NODE, root->left->type, "Sol alt ağaç PIPE_NODE olmalı");
    
    // Son pipe parçası command node olmalı (wc -l)
    ASSERT_EQ(COMMAND_NODE, root->right->type, "Sağ alt ağaç COMMAND_NODE olmalı");
    
    char *right_expected[] = {"wc", "-l"};
    ASSERT_TRUE(validate_command_node(root->right, right_expected, 2),
                "Sağ komut 'wc -l' doğru parse edilmeli");
    
    // Sol alt ağacın alt parçaları
    ASSERT_PTR_NOT_NULL(root->left->left, "En soldaki alt ağaç oluşturulmalı");
    ASSERT_PTR_NOT_NULL(root->left->right, "Ortadaki alt ağaç oluşturulmalı");
    
    // ls -l doğrulama
    char *left_left_expected[] = {"ls", "-l"};
    ASSERT_TRUE(validate_command_node(root->left->left, left_left_expected, 2),
                "En soldaki komut 'ls -l' doğru parse edilmeli");
    
    // grep test doğrulama
    char *left_right_expected[] = {"grep", "test"};
    ASSERT_TRUE(validate_command_node(root->left->right, left_right_expected, 2),
                "Ortadaki komut 'grep test' doğru parse edilmeli");
    
    // Temizlik
    free_ast(root);
    free_token_list(tokens);
}

void test_cmdval_list_operations() {
    t_cmdval_list *list = init_cmdval_list();
    ASSERT_PTR_NOT_NULL(list, "Komut değer listesi oluşturulmalı");
    ASSERT_EQ(0, list->size, "Liste başlangıçta boş olmalı");
    
    // Listeye ekleme
    char *test_word = strdup("test");
    int result = add_node_to_cmdval_list(list, test_word);
    ASSERT_EQ(1, result, "Listeye ekleme başarılı olmalı");
    ASSERT_EQ(1, list->size, "Liste boyutu 1 olmalı");
    ASSERT_PTR_NOT_NULL(list->head, "Liste başı NULL olmamalı");
    ASSERT_STR_EQ("test", (char*)list->head->value, "Liste başındaki değer 'test' olmalı");
    
    // İkinci ekleme
    char *test_word2 = strdup("test2");
    result = add_node_to_cmdval_list(list, test_word2);
    ASSERT_EQ(1, result, "İkinci ekleme başarılı olmalı");
    ASSERT_EQ(2, list->size, "Liste boyutu 2 olmalı");
    ASSERT_STR_EQ("test2", (char*)list->tail->value, "Liste sonundaki değer 'test2' olmalı");
    
    // Temizlik
    free_cmdval_list_with_contents(list, 0);
}

/* Tüm Parser testlerini çalıştır */
void run_parser_tests() {
    printf("\033[1;36m===== Parser Module Tests =====\033[0m\n");
    
    register_test("parse_simple_command", test_parse_simple_command);
    register_test("parse_command_with_redirections", test_parse_command_with_redirections);
    register_test("parse_command_with_multiple_redirections", test_parse_command_with_multiple_redirections);
    register_test("parse_simple_pipe", test_parse_simple_pipe);
    register_test("parse_tokens_simple", test_parse_tokens_simple);
    register_test("parse_tokens_pipe", test_parse_tokens_pipe);
    register_test("parse_tokens_complex_pipe", test_parse_tokens_complex_pipe);
    register_test("cmdval_list_operations", test_cmdval_list_operations);
}