/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:36:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/04 14:42:50 by rakman           ###   ########.fr       */
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
} t_redir_type;

typedef struct s_redirection
{
    t_redir_type type;
    char         *filename;
} t_redirection;

typedef struct t_command_value 
{
	char **arg_array;
	t_redirection **redirections;
} command_value;

typedef struct t_ast_node
{
	ast_type			type;
	struct t_ast_node	*left;
	struct t_ast_node	*right;
	command_value		*value;	
}	ast_node;

typedef struct s_generic_list_node {
    void *value;
    struct s_generic_list_node *prev;
    struct s_generic_list_node *next;
} t_gl_node;

typedef struct s_generic_list {
	int			size;
	t_gl_node *head;
	t_gl_node *tail;
} t_generic_list;



void free_command_value(command_value *details);
void free_generic_list_nodes_only(t_generic_list *list);
ast_node *parse_tokens(t_token_list *tokens);
void free_ast(ast_node *node);
void free_generic_list_with_contents(t_generic_list *list, int is_redir_list);
/**
 * @brief Visualizes the AST in a pretty tree format with colors
 * 
 * @param root The root node of the AST to visualize
 */
void visualize_ast(ast_node *root);

#endif
