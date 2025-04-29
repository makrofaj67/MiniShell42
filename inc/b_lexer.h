/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_lexer.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:18:09 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:33:50 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef B_LEXER_H
# define B_LEXER_H

typedef enum e_t_type
{
	WORD,
	PIPE,
	RDRT_IN,
	RDRT_OUT,
	APPEND,
	HEREDOC,
	SEMICOLON,
	AND,
	OR,
	PRNT_OPEN,
	PRNT_CLOSE
}	t_t_type;

typedef struct s_t_node
{
	char			*value;
	t_t_type		type;
	struct s_t_node	*next;
	struct s_t_node	*prev;
}	t_t_node;

typedef struct s_t_list
{
	t_t_node	*head;
	t_t_node	*tail;
}	t_t_list;

/* Main lexer function */
t_t_list	*get_lexed(char *command);

/* Lexer helpers */
t_t_type	get_enum_type(char *unproc_command);
char		*get_value(char *unproc_command);
int			get_token_len(char *str);

/* Token character helpers */
int			is_special_char(char c);
int			is_space(char c);
int			check_quote_state(char c, char *quote);
int			handle_special_chars(char *str);
int			handle_backslash(char *str, int i, char quote, int *inc);

/* Linked list functions */
t_t_list	*init_token_list(void);
t_t_node	*create_token_node(char *value, t_t_type type);
void		add_token_to_list(t_t_list *list, char *unprocessed_token);
void		free_token_list(t_t_list *list);

/* Debug function */
void		print_token_list(t_t_list *list);

#endif
