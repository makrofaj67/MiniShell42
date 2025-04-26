/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:23:28 by rakman            #+#    #+#             */
/*   Updated: 2023/04/26 19:17:20 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H
# include <signal.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	APPEND,
	HEREDOC,
	ERROR
}	t_token_type;

typedef struct s_token_node
{
	t_token_type		type;
	char				*value;
	struct s_token_node	*next;
	struct s_token_node	*prev;
}	t_token_node;

typedef struct s_token_list
{
	int				size;
	t_token_node	*head;
	t_token_node	*tail;
}	t_token_list;

/* linked_list_operations.c */
t_token_list	*init_token_list(void);
t_token_node	*create_token_node(t_token_type type, char *value);
void			add_token(t_token_node *node, t_token_list *list);
void			free_token_list(t_token_list *list);
const char		*token_type_to_string(t_token_type type);

/* token_conversions.c */
t_token_type	determine_token_type(char *token);
t_token_node	*token_to_node(char *token);
t_token_list	*create_token_list_from_array(char **tokens);

/* extract_tokens.c */
char			**extract_tokens(char *command);
char			**init_tokens_array(int size);
void			free_tokens_array(char **tokens);

/* token_utils.c */
int				handle_quotes(char *command, int start);
int				get_token_end(char *command, int start);
char			*extract_token(char *command, int start, int end);
char			*get_token_value(char *cmd, int start, int end);

/* main.c */
void			print_tokens(t_token_list *list);
t_token_list	*tokenize_command(char *command);

#endif
