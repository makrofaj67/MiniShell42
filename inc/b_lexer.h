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

typedef struct s_env_and_exit
{
	int exit_status;
	t_env *env_list;
	t_env *export_list;
}	t_env_and_exit;

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

char *get_expanded(char *command, int *exit_status, t_env *env_list);
char *get_quote_trimmed(char *command);
char	*handle_variable_expansion(char *raw_command, int *i,
    t_env_and_exit *envx, char *result);
char *init_expanded_str(void);
char *get_var_value(char *varname, int exit_status, t_env *env_list);
char *init_expanded_str(void);
t_token_list	*create_tokens(char *input);
char *handle_backslash(char *raw_command, int *i_ptr, char *current_result);

t_token_type    get_token_type(char *token_str);
t_token_list	*create_token_list(void);
t_token_node	*create_token(char *value, t_token_type type);
void			add_token(t_token_list *list, char *value);
void			free_token_list(t_token_list *list);


int is_need_for_expanding(char *raw_command, int i);
int get_quote_state_at_position(char *command, int pos);
void xhandle_two(char *command, int *index, int *state);
void xhandle_one(char *command, int *index, int *state);
void xhandle_zero(char *command, int *index, int *state);

int count_digits(long n);

char *get_varname(char *r_cmd, int *i);
char *add_str_to_result(char *result, char *str);
char *add_char_to_result(char *result, char c);
char *init_expanded_str(void);

void			print_tokens(t_token_list *list);

#endif
