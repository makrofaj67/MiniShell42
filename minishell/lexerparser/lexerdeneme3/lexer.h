#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <string.h>

// Token türleri
typedef enum e_token_type {
    WORD,
    PIPE,
    REDIR_OUT,
    REDIR_IN,
    REDIR_APPEND,
    REDIR_HEREDOC
}   t_token_type;

// Token yapısı
typedef struct s_token {
    t_token_type    type;
    char            *value;
    struct s_token  *next;
}   t_token;

// Token listesi işlemleri
void    add_token(t_token **head, t_token_type type, const char *value);
void    free_tokens(t_token *head);
void    print_tokens(const t_token *head);

// Lexer fonksiyonu
t_token *lexer(const char *input);

// Fonksiyon prototipi ekleniyor
int has_unclosed_quote(const char *input);

#endif
