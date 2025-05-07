/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_visualizer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 16:42:30 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:29:52 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * @brief Creates a formatted indentation string for tree visualization
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The prefix string to build upon
 * @return char* Formatted indentation string
 */
char	*get_tree_branch(int depth, int is_last, char *prefix)
{
	char	*new_prefix;
	char	*branch;
	size_t	needed_size;

	if (depth == 0)
		return (strdup(""));
	if (is_last)
		branch = "└── ";
	else
		branch = "├── ";
	needed_size = strlen(prefix) + strlen(branch) + 1;
	new_prefix = malloc(needed_size);
	if (!new_prefix)
		return (strdup(""));
	snprintf(new_prefix, needed_size, "%s%s", prefix, branch);
	return (new_prefix);
}

/**
 * @brief Creates the prefix for the next level of the tree
 * 
 * @param depth Current depth level in the tree
 * @param is_last Whether this is the last child at this level
 * @param prefix The current prefix string
 * @return char* Prefix for the next level
 */
char	*get_next_prefix(int depth, int is_last, char *prefix)
{
	char	*next_prefix;
	char	*extension;
	size_t	needed_size;

	if (depth == 0)
		return (strdup(""));
	if (is_last)
		extension = "    ";
	else
		extension = "│   ";
	needed_size = strlen(prefix) + strlen(extension) + 1;
	next_prefix = malloc(needed_size);
	if (!next_prefix)
		return (strdup(""));
	snprintf(next_prefix, needed_size, "%s%s", prefix, extension);
	return (next_prefix);
}

/**
 * @brief Visualizes a redirection with ASCII art
 * 
 * @param redir The redirection to visualize
 */
void	visualize_redirection(t_redirection *redir)
{
	char	*redir_symbol;

	redir_symbol = "?";
	if (redir->type == REDIR_IN)
		redir_symbol = "<";
	else if (redir->type == REDIR_OUT)
		redir_symbol = ">";
	else if (redir->type == REDIR_APPEND)
		redir_symbol = ">>";
	else if (redir->type == REDIR_HEREDOC)
		redir_symbol = "<<";
	printf(" %s %s", redir_symbol, redir->filename);
}

/**
 * @brief Recursively visualizes the AST starting from the given node
 * 
 * @param node The AST node to visualize
 * @param depth Current depth in the tree
 * @param is_last Whether this node is the last child of its parent
 * @param prefix Current prefix for formatting
 */
static void	print_command_args(command_value *value, char *branch)
{
	int	i;

	printf("%s\033[1;32mCOMMAND\033[0m: ", branch);
	i = 0;
	while (value->arg_array[i])
	{
		if (i > 0)
			printf(" ");
		printf("\033[1;33m%s\033[0m", value->arg_array[i]);
		i++;
	}
	if (value->redirections)
	{
		i = 0;
		while (value->redirections[i])
		{
			visualize_redirection(value->redirections[i]);
			i++;
		}
	}
	printf("\n");
}

static void	visualize_cmd_node(ast_node *node, char *branch)
{
	if (node->value && node->value->arg_array && node->value->arg_array[0])
		print_command_args(node->value, branch);
	else
		printf("%s\033[1;31mEMPTY COMMAND\033[0m\n", branch);
}

void	visualize_ast_recursive(ast_node *node, int depth, int is_last,
		char *prefix)
{
	char	*branch;
	char	*next_prefix;

	if (node == NULL)
		return ;
	branch = get_tree_branch(depth, is_last, prefix);
	if (node->type == PIPE_NODE)
	{
		printf("%s\033[1;36mPIPE\033[0m\n", branch);
		next_prefix = get_next_prefix(depth, is_last, prefix);
		visualize_ast_recursive(node->left, depth + 1, 0, next_prefix);
		visualize_ast_recursive(node->right, depth + 1, 1, next_prefix);
		free(next_prefix);
	}
	else if (node->type == COMMAND_NODE)
		visualize_cmd_node(node, branch);
	free(branch);
}

/**
 * @brief Visualizes the AST in a pretty tree format with colors
 * 
 * @param root The root node of the AST to visualize
 */
void	visualize_ast(ast_node *root)
{
	if (root == NULL)
	{
		printf("\033[1;31mEmpty AST (NULL)\033[0m\n");
		return ;
	}
	printf("\n\033[1;35m===== AST Visualization =====\033[0m\n\n");
	visualize_ast_recursive(root, 0, 1, "");
	printf("\n\033[1;35m============================\033[0m\n\n");
}


