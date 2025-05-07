/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parser.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:36:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:29:49 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef C_PARSER_H
# define C_PARSER_H
# include "b_lexer.h"

/* ------------------------------ Enums ------------------------------ */
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
}	t_redir_type;

/* ------------------------------ Structures ------------------------------ */
/**
 * @brief	Redirection structure
 * @details	Stores the type and target information of a redirection for a command.
 * @usage	Created in the add_redir function and added to command data structure
 *          in the parse_simple_command function. Stores the redirection type and target file.
 */
typedef struct s_redirection
{
	t_redir_type	type;
	char			*filename;
}	t_redirection;

/**
 * @brief	Command value structure
 * @details	Stores all components of a command (arguments and redirections).
 * @usage	Created in the parse_simple_command function and added to the AST command node
 *          with the create_command_node function. Stores the command's argument array
 *          and redirection array.
 */
typedef struct t_command_value
{
	char			**arg_array;
	t_redirection	**redirections;
}	command_value;

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
typedef struct s_cmdval_node
{
	void					*value;
	struct s_cmdval_node	*prev;
	struct s_cmdval_node	*next;
}	t_cmdval_node;

/**
 * @brief	Generic linked list structure
 * @details	Main structure used to manage a doubly linked list.
 * @usage	Created in the init_cmdval_list function.
 *          Used in the parse_simple_command function to store word and redirection lists.
 *          Tracks the number of elements in the list, and the start and end nodes.
 */
typedef struct s_cmdval_list
{
	int				size;
	t_cmdval_node	*head;
	t_cmdval_node	*tail;
}	t_cmdval_list;

/* ------------------------------ Main Parsing Functions ------------------------------ */
/**
 * @brief Main entry point for parsing tokens into an AST
 * 
 * @param tokens Token list to parse
 * @return ast_node* Root node of the created AST
 */
ast_node	*parse_tokens(t_token_list *tokens);

/**
 * @brief Parse a simple command from a token list
 * 
 * @param tokens Token list to parse
 * @return command_value* Command value containing arguments and redirections
 */
command_value	*parse_simple_command(t_token_list *tokens);

/**
 * @brief Parse a pipe expression, splitting into left and right parts
 * 
 * @param tokens Token list to parse
 * @param pipe_location Location of the pipe token
 * @return ast_node** Array with left and right AST subtrees
 */
ast_node		**parse_simple_pipe(t_token_list *tokens,
					t_token_node *pipe_location);

/* ------------------------------ List Management Functions ------------------------------ */
/**
 * @brief Initialize a new command value list
 * 
 * @return t_cmdval_list* New list or NULL on error
 */
t_cmdval_list	*init_cmdval_list(void);

/**
 * @brief Add a node to a command value list
 * 
 * @param list List to add to
 * @param value Value to store in the new node
 * @return int 1 on success, 0 on failure
 */
int				add_node_to_cmdval_list(t_cmdval_list *list, void *value);

/**
 * @brief Fill a command value list with data from tokens
 * 
 * @param tokens Token list to process
 * @param r_list Redirection list to fill
 * @param w_list Word list to fill
 * @return t_cmdval_list** Array containing both lists
 */
t_cmdval_list	**fill_cmdval_lists(t_token_list *tokens, t_cmdval_list *r_list,
					t_cmdval_list *w_list);

/* ------------------------------ Token Processing Functions ------------------------------ */
/**
 * @brief Add a word token to a command value list
 * 
 * @param node Token node containing the word
 * @param list List to add to
 */
void			add_word(t_token_node *node, t_cmdval_list *list);

/**
 * @brief Add a redirection token to a command value list
 * 
 * @param node Token node containing the redirection
 * @param list List to add to
 */
void			add_redir(t_token_node *node, t_cmdval_list *list);

/**
 * @brief Find the last pipe token in a token list
 * 
 * @param tokens Token list to search
 * @return t_token_node* Last pipe token or NULL if none found
 */
t_token_node	*find_last_pipe(t_token_list *tokens);

/**
 * @brief Get tokens before a pipe location
 * 
 * @param tokens Original token list
 * @param pipe_location Pipe token location
 * @return t_token_list* New list with tokens before the pipe
 */
t_token_list	*get_tokens_before(t_token_list *tokens,
					t_token_node *pipe_location);

/**
 * @brief Get tokens after a pipe location
 * 
 * @param tokens Original token list
 * @param pipe_location Pipe token location
 * @return t_token_list* New list with tokens after the pipe
 */
t_token_list	*get_tokens_after(t_token_list *tokens,
					t_token_node *pipe_location);

/* ------------------------------ AST Construction Functions ------------------------------ */
/**
 * @brief Create a pipe node in the AST
 * 
 * @param left_child Left subtree
 * @param right_child Right subtree
 * @return ast_node* New pipe node
 */
ast_node		*create_pipe_node(ast_node *left_child, ast_node *right_child);

/**
 * @brief Create a command node in the AST
 * 
 * @param cmd_details Command value to store in the node
 * @return ast_node* New command node
 */
ast_node		*create_command_node(command_value *cmd_details);

/* ------------------------------ Memory Management Functions ------------------------------ */
/**
 * @brief Free an AST and all its nodes
 * 
 * @param node Root node of the AST to free
 */
void			free_ast(ast_node *node);

/**
 * @brief Free a command value structure
 * 
 * @param details Command value to free
 */
void			free_command_value(command_value *details);

/**
 * @brief Free list nodes without freeing their contents
 * 
 * @param list List to free
 */
void			free_cmdval_list_nodes_only(t_cmdval_list *list);

/**
 * @brief Free a list and its contents
 * 
 * @param list List to free
 * @param is_redir_list Whether this list contains redirections (1) or words (0)
 */
void			free_cmdval_list_with_contents(t_cmdval_list *list,
					int is_redir_list);

/* ------------------------------ Debugging Functions ------------------------------ */
/**
 * @brief Visualize an AST in a pretty tree format with colors
 * 
 * @param root Root node of the AST to visualize
 */
void			visualize_ast(ast_node *root);

#endif
