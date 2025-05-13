/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   x_visualizer_ast.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:18:45 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:34:55 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

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
