/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_lexer.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:18:09 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:07:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef B_LEXER_H
# define B_LEXER_H

/* Token türleri - subject'e uygun olarak sadece gerekli token tipleri */
typedef enum e_token_type
{
	WORD,    /* Normal kelimeler ve komutlar */
	PIPE,    /* | karakteri */
	RDRT_IN, /* < karakteri (input redirection) */
	RDRT_OUT,/* > karakteri (output redirection) */
	APPEND,  /* >> karakteri (append mode output redirection) */
	HEREDOC  /* << karakteri (heredoc) */
}	t_token_type;

/* Token düğümü yapısı */
typedef struct s_token_node
{
	char				*value;
	t_token_type		type;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

/* Token listesi yapısı */
typedef struct s_token_list
{
	t_token_node	*head;
	t_token_node	*tail;
}	t_token_list;

/* Ana tokenization fonksiyonu */
t_token_list	*tokenize_command(char *command, int exit_status);

/* Token türlerini belirleme */
t_token_type	get_token_type(char *token_str);
int				is_operator(char c);
int				is_whitespace(char c);
int				is_redirection(char *str);
int				get_operator_len(const char *str);  // Updated to accept const char*

/* Tırnak işlemesi ve çevre değişkenleri */
int				is_quote(char c);
int				update_quote_state(char c, char *quote_state);
char			*process_token(char *token, int exit_status);

/* Token listesi işlemleri */
t_token_list	*create_token_list(void);
t_token_node	*create_token(char *value, t_token_type type);
void			add_token(t_token_list *list, char *value);
void			free_token_list(t_token_list *list);

/* Debug fonksiyonu */
void			print_tokens(t_token_list *list);

#endif
