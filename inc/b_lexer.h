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

#include "f_builtins.h"
/**
** Token tnpes for the lexical analyzer
*/
typedef enum e_token_type
{
	WORD,
	PIPE,
	RDRT_IN,
	RDRT_OUT,
	APPEND,
	HEREDOC,
	TOKEN_ERROR,
	TOKEN_UNKNOWN,
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

char *get_expanded(char *command, int exit_status, t_env *env_list);
t_token_type    get_token_type(char *token_str);
t_token_list	*create_token_list(void);
t_token_node	*create_token(char *value, t_token_type type);
void			add_token(t_token_list *list, char *value);
void			free_token_list(t_token_list *list);

void			print_tokens(t_token_list *list);

#endif
