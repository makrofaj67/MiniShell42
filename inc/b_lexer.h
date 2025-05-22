/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_lexer.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 19:18:09 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:40:56 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef B_LEXER_H
# define B_LEXER_H

# include "0_core_env.h" // Include for t_variable_list
# include "e_builtins.h"

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

typedef struct s_token_node
{
	char				*value;
	t_token_type		type;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

typedef struct s_token_list
{
	t_token_node	*head;
	t_token_node	*tail;
}	t_token_list;

char			*get_expanded(char *raw_command, int *exit_status_ptr,
					t_variable_list *variables);
char			*handle_variable_expansion(char *raw_command, int *i,
					int *exit_status_ptr, t_variable_list *variables,
					char *result);
char			*get_varname(char *r_cmd, int *i);
char			*add_char_to_result(char *result, char c);
char			*add_str_to_result(char *result, char *str);
int				is_need_for_expanding(char *raw_command, int i);
int				get_quote_state_at_position(char *command, int pos);
char			*get_quote_trimmed(char *command);

t_token_list	*create_token_list(void); // Added prototype
t_token_list	*create_tokens(char *input);
void			add_token(t_token_list *list, char *value);
t_token_type	get_token_type(char *token_str);
void			free_token_list(t_token_list *list);
void			print_tokens(t_token_list *list);

#endif
