/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:36:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 14:04:03 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_H
# define C_PARSER_H
# include "b_lexer.h"


typedef enum e_ast_type
{
	PIPE_NODE,
	COMMAND_NODE
}	ast_type;


typedef enum e_redir_type
{
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	REDIR_HEREDOC
}	t_redir_type;


typedef struct s_redirection
{
	t_redir_type	type;
	char			*filename;
}	t_redirection;


typedef struct t_command_value
{
	char			**arg_array;
	t_redirection	**redirections;
	int             stdin_backup;  // To store the original stdin file descriptor
	int             stdout_backup; // To store the original stdout file descriptor
}	command_value;


typedef struct t_ast_node
{
	ast_type			type;
	struct t_ast_node	*left;
	struct t_ast_node	*right;
	command_value		*value;
}	ast_node;


typedef struct s_cmdval_node
{
	void					*value;
	struct s_cmdval_node	*prev;
	struct s_cmdval_node	*next;
}	t_cmdval_node;


typedef struct s_cmdval_list
{
	int				size;
	t_cmdval_node	*head;
	t_cmdval_node	*tail;
}	t_cmdval_list;


ast_node	*parse_tokens(t_token_list *tokens);


command_value	*parse_simple_command(t_token_list *tokens);


ast_node		**parse_simple_pipe(t_token_list *tokens,
					t_token_node *pipe_location);


t_cmdval_list	*init_cmdval_list(void);


int				add_node_to_cmdval_list(t_cmdval_list *list, void *value);


t_cmdval_list	**fill_cmdval_lists(t_token_list *tokens, t_cmdval_list *r_list,
					t_cmdval_list *w_list);


void			add_word(t_token_node *node, t_cmdval_list *list);


void			add_redir(t_token_node *node, t_cmdval_list *list);


t_token_node	*find_last_pipe(t_token_list *tokens);


t_token_list	*get_tokens_before(t_token_list *tokens,
					t_token_node *pipe_location);


t_token_list	*get_tokens_after(t_token_node *pipe_location);


ast_node		*create_pipe_node(ast_node *left_child, ast_node *right_child);


ast_node		*create_command_node(command_value *cmd_details);


void			free_ast(ast_node *node);


void			free_command_value(command_value *details);


void			free_cmdval_list_nodes_only(t_cmdval_list *list);


void			free_cmdval_list_with_contents(t_cmdval_list *list,
					int is_redir_list);


void			visualize_ast(ast_node *root);


char			*get_tree_branch(int depth, int is_last, char *prefix);


char			*get_next_prefix(int depth, int is_last, char *prefix);


void			visualize_redirection(t_redirection *redir);

#endif
