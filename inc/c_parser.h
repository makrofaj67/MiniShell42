/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:36:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 18:47:40 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_H
# define C_PARSER_H
# include "b_lexer.h"

/**
 * @brief	Enum defining Abstract Syntax Tree (AST) node types
 * @details	Determines the type of AST nodes created by the parse_tokens function.
 *          PIPE_NODE type is used in the "create_pipe_node" function.
 *          COMMAND_NODE type is used in the "create_command_node" function.
 * @usage	Used to identify the type of AST nodes and to determine how
 *          memory cleanup should be performed in the free_ast function.
 */
typedef enum e_ast_type
{
	PIPE_NODE,
	COMMAND_NODE
}	ast_type;

/**
 * @brief	Enum defining redirection types
 * @details	Used to determine the type of command redirections.
 *          Token types (RDRT_IN, RDRT_OUT, etc.) are converted to these enum values.
 * @usage	Used in the add_redir function to convert from token type to redirection type.
 *          Used in the visualize_redirection function to create redirection symbols.
 */
typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redir_type;

/**
 * @brief	Redirection structure
 * @details	Stores the type and target information of a redirection for a command.
 * @usage	Created in the add_redir function and added to command data structure
 *          in the parse_simple_command function. Stores the redirection type and target file.
 */
typedef struct s_redirection
{
    t_redir_type type;
    char         *filename;
} t_redirection;

/**
 * @brief	Command value structure
 * @details	Stores all components of a command (arguments and redirections).
 * @usage	Created in the parse_simple_command function and added to the AST command node
 *          with the create_command_node function. Stores the command's argument array
 *          and redirection array.
 */
typedef struct t_command_value 
{
	char **arg_array;
	t_redirection **redirections;
} command_value;

/**
 * @brief	Abstract Syntax Tree (AST) node structure
 * @details	Represents each node (pipe or command) in the command tree.
 * @usage	Created in the create_pipe_node and create_command_node functions.
 *          The parse_tokens function uses this structure to build the command tree.
 *          A node can either be a pipe node (with two subtrees) or a command node (with a value).
 */
typedef struct t_ast_node
{
	ast_type			type;
	struct t_ast_node	*left;
	struct t_ast_node	*right;
	command_value		*value;	
}	ast_node;

/**
 * @brief	Generic linked list node structure
 * @details	Represents nodes forming a doubly linked list, independent of content type.
 * @usage	Created in the add_node_to_cmdval_list function.
 *          Used for word and redirection lists.
 *          The void* provides flexibility to store different types of data.
 */
typedef struct s_cmdval_node {
    void					*value;
    struct s_cmdval_node	*prev;
    struct s_cmdval_node	*next;
} t_cmdval_node;

/**
 * @brief	Generic linked list structure
 * @details	Main structure used to manage a doubly linked list.
 * @usage	Created in the init_cmdval_list function.
 *          Used in the parse_simple_command function to store word and redirection lists.
 *          Tracks the number of elements in the list, and the start and end nodes.
 */
typedef struct s_cmdval_list {
	int				size;
	t_cmdval_node	*head;
	t_cmdval_node	*tail;
} t_cmdval_list;


ast_node	**parse_simple_pipe(t_token_list *tokens,
				t_token_node *pipe_location);













ast_node *create_pipe_node(ast_node *left_child, ast_node *right_child);
t_token_node *find_last_pipe(t_token_list *tokens);
ast_node *create_command_node(command_value *cmd_details);
command_value *parse_simple_command(t_token_list *tokens);
void add_word(t_token_node *node, t_cmdval_list *list);
void add_redir(t_token_node *node, t_cmdval_list *list) ;
int add_node_to_cmdval_list(t_cmdval_list *list, void *value);
t_cmdval_list *init_cmdval_list(void);
t_token_list *get_tokens_before(t_token_list *tokens, t_token_node *pipe_location);
t_token_list *get_tokens_after(t_token_list *tokens, t_token_node *pipe_location);

void free_command_value(command_value *details);
void free_cmdval_list_nodes_only(t_cmdval_list *list);
ast_node *parse_tokens(t_token_list *tokens);
void free_ast(ast_node *node);
void free_cmdval_list_with_contents(t_cmdval_list *list, int is_redir_list);
/**
 * @brief Visualizes the AST in a pretty tree format with colors
 * 
 * @param root The root node of the AST to visualize
 */
void visualize_ast(ast_node *root);

#endif
