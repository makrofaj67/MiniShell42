#include "../../inc/__minishell.h"
#include "../../inc/f_tests/test_framework.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Token karşılaştırma yardımcı fonksiyonu */
int token_equals(t_token_node *token, const char *expected_value, t_token_type expected_type) {
    if (!token || !expected_value)
        return 0;
    return (strcmp(token->value, expected_value) == 0 && token->type == expected_type);
}

/* Lexer fonksiyonları testleri */
void test_tokenize_simple() {
    char *command = "echo hello world";
    t_token_list *tokens = tokenize_command(command, 0);
    
    ASSERT_PTR_NOT_NULL(tokens, "Tokenize listesi oluşturma");
    ASSERT_PTR_NOT_NULL(tokens->head, "Token listesi başının var olması");
    
    t_token_node *current = tokens->head;
    ASSERT_TRUE(token_equals(current, "echo", WORD), "İlk token 'echo' olmalı");
    
    current = current->next;
    ASSERT_PTR_NOT_NULL(current, "İkinci token var olmalı");
    ASSERT_TRUE(token_equals(current, "hello", WORD), "İkinci token 'hello' olmalı");
    
    current = current->next;
    ASSERT_PTR_NOT_NULL(current, "Üçüncü token var olmalı");
    ASSERT_TRUE(token_equals(current, "world", WORD), "Üçüncü token 'world' olmalı");
    
    current = current->next;
    ASSERT_PTR_NULL(current, "Başka token olmamalı");
    
    free_token_list(tokens);
}

void test_tokenize_pipes() {
    char *command = "ls -l | grep test | wc -l";
    t_token_list *tokens = tokenize_command(command, 0);
    
    ASSERT_PTR_NOT_NULL(tokens, "Tokenize listesi oluşturma");
    
    t_token_node *current = tokens->head;
    ASSERT_TRUE(token_equals(current, "ls", WORD), "İlk token 'ls' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "-l", WORD), "İkinci token '-l' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "|", PIPE), "Üçüncü token '|' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "grep", WORD), "Dördüncü token 'grep' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "test", WORD), "Beşinci token 'test' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "|", PIPE), "Altıncı token '|' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "wc", WORD), "Yedinci token 'wc' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "-l", WORD), "Sekizinci token '-l' olmalı");
    
    free_token_list(tokens);
}

void test_tokenize_redirections() {
    char *command = "echo hello > file < input >> append_file";
    t_token_list *tokens = tokenize_command(command, 0);
    
    ASSERT_PTR_NOT_NULL(tokens, "Tokenize listesi oluşturma");
    
    t_token_node *current = tokens->head;
    ASSERT_TRUE(token_equals(current, "echo", WORD), "İlk token 'echo' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "hello", WORD), "İkinci token 'hello' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, ">", RDRT_OUT), "Üçüncü token '>' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "file", WORD), "Dördüncü token 'file' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "<", RDRT_IN), "Beşinci token '<' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "input", WORD), "Altıncı token 'input' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, ">>", APPEND), "Yedinci token '>>' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "append_file", WORD), "Sekizinci token 'append_file' olmalı");
    
    free_token_list(tokens);
}

void test_tokenize_quotes() {
    char *command = "echo \"quoted text\" 'more text'";
    t_token_list *tokens = tokenize_command(command, 0);
    
    ASSERT_PTR_NOT_NULL(tokens, "Tokenize listesi oluşturma");
    
    t_token_node *current = tokens->head;
    ASSERT_TRUE(token_equals(current, "echo", WORD), "İlk token 'echo' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "quoted text", WORD), "İkinci token 'quoted text' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "more text", WORD), "Üçüncü token 'more text' olmalı");
    
    free_token_list(tokens);
}

void test_tokenize_complex() {
    char *command = "cat file.txt | grep \"hello world\" > output.txt";
    t_token_list *tokens = tokenize_command(command, 0);
    
    ASSERT_PTR_NOT_NULL(tokens, "Tokenize listesi oluşturma");
    
    t_token_node *current = tokens->head;
    ASSERT_TRUE(token_equals(current, "cat", WORD), "İlk token 'cat' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "file.txt", WORD), "İkinci token 'file.txt' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "|", PIPE), "Üçüncü token '|' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "grep", WORD), "Dördüncü token 'grep' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "hello world", WORD), "Beşinci token 'hello world' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, ">", RDRT_OUT), "Altıncı token '>' olmalı");
    
    current = current->next;
    ASSERT_TRUE(token_equals(current, "output.txt", WORD), "Yedinci token 'output.txt' olmalı");
    
    free_token_list(tokens);
}

void test_token_type_detection() {
    ASSERT_EQ(WORD, get_token_type("hello"), "WORD tipi doğru algılanmalı");
    ASSERT_EQ(PIPE, get_token_type("|"), "PIPE tipi doğru algılanmalı");
    ASSERT_EQ(RDRT_IN, get_token_type("<"), "RDRT_IN tipi doğru algılanmalı");
    ASSERT_EQ(RDRT_OUT, get_token_type(">"), "RDRT_OUT tipi doğru algılanmalı");
    ASSERT_EQ(APPEND, get_token_type(">>"), "APPEND tipi doğru algılanmalı");
    ASSERT_EQ(HEREDOC, get_token_type("<<"), "HEREDOC tipi doğru algılanmalı");
}

void test_is_operator_and_whitespace() {
    ASSERT_TRUE(is_operator('|'), "| operatör olmalı");
    ASSERT_TRUE(is_operator('<'), "< operatör olmalı");
    ASSERT_TRUE(is_operator('>'), "> operatör olmalı");
    ASSERT_FALSE(is_operator('a'), "a operatör olmamalı");
    
    ASSERT_TRUE(is_whitespace(' '), "space whitespace olmalı");
    ASSERT_TRUE(is_whitespace('\t'), "tab whitespace olmalı");
    ASSERT_TRUE(is_whitespace('\n'), "newline whitespace olmalı");
    ASSERT_FALSE(is_whitespace('a'), "a whitespace olmamalı");
}

void test_token_list_operations() {
    t_token_list *list = create_token_list();
    ASSERT_PTR_NOT_NULL(list, "Token listesi oluşturma");
    
    add_token(list, "test");
    ASSERT_PTR_NOT_NULL(list->head, "İlk token eklendikten sonra head olmalı");
    ASSERT_STR_EQ("test", list->head->value, "İlk token değeri doğru olmalı");
    
    add_token(list, "test2");
    ASSERT_STR_EQ("test2", list->tail->value, "İkinci token değeri doğru olmalı");
    
    free_token_list(list);
}

/* Tüm Lexer testlerini çalıştır */
void run_lexer_tests() {
    printf("\033[1;36m===== Lexer Module Tests =====\033[0m\n");
    
    register_test("tokenize_simple", test_tokenize_simple);
    register_test("tokenize_pipes", test_tokenize_pipes);
    register_test("tokenize_redirections", test_tokenize_redirections);
    register_test("tokenize_quotes", test_tokenize_quotes);
    register_test("tokenize_complex", test_tokenize_complex);
    register_test("token_type_detection", test_token_type_detection);
    register_test("is_operator_and_whitespace", test_is_operator_and_whitespace);
    register_test("token_list_operations", test_token_list_operations);
}