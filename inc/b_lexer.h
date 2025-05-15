/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_lexer.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:18:09 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:34:56 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef B_LEXER_H
# define B_LEXER_H

/**
** Token types for the lexical analyzer
*/
typedef enum e_token_type
{
	WORD,
	PIPE,
	RDRT_IN,
	RDRT_OUT,
	APPEND,
	HEREDOC
}	t_token_type;

/**
** State tracking for token processing
*/
typedef struct s_token_state
{
	char	*token;
	char	*result;
	int		i;
	int		j;
	int		max_len;
	int		exit_status;
	char	quote_state;
}	t_token_state;

/**
** Node in the token linked list
*/
typedef struct s_token_node
{
	char				*value;
	t_token_type		type;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

/**
** Container for the token list
*/
typedef struct s_token_list
{
	t_token_node	*head;
	t_token_node	*tail;
}	t_token_list;

/**
** Main tokenization function
*/
t_token_list	*tokenize_command(char *command);

/**
** Token type classification
*/
t_token_type	get_token_type(char *token_str);
int				is_operator(char c);
int				is_whitespace(char c);
int				get_operator_len(const char *str);

/**
** Quote and variable handling
*/
int				is_quote(char c);
int				update_quote_state(char c, char *quote_state);
char			*process_token(char *token, int exit_status);
t_token_state	init_token_state(char *token, int exit_status);
void			handle_quotes(t_token_state *state);

/**
** Token list management
*/
t_token_list	*create_token_list(void);
t_token_node	*create_token(char *value, t_token_type type);
void			add_token(t_token_list *list, char *value);
void			free_token_list(t_token_list *list);

/**
** Debugging utility
*/
void			print_tokens(t_token_list *list);

#endif
