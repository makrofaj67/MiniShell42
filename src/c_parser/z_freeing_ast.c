/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_freeing_ast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:26:30 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:29:52 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * @brief Free a string array with null termination
 * 
 * @param arg_array Array of strings to free
 */
static void	free_arg_array(char **arg_array)
{
	int	i;

	if (arg_array == NULL)
		return ;
	i = 0;
	while (arg_array[i] != NULL)
	{
		free(arg_array[i]);
		i++;
	}
	free(arg_array);
}

/**
 * @brief Free an array of redirection structures
 * 
 * @param redirections Array of redirection structures to free
 */
static void	free_redirection_array(t_redirection **redirections)
{
	int	i;

	if (redirections == NULL)
		return ;
	i = 0;
	while (redirections[i] != NULL)
	{
		if (redirections[i]->filename != NULL)
			free(redirections[i]->filename);
		free(redirections[i]);
		i++;
	}
	free(redirections);
}

/**
 * @brief Free all resources associated with a command value structure
 * 
 * @param details Command value structure to free
 */
void	free_command_value(command_value *details)
{
	if (details == NULL)
		return ;
	if (details->arg_array != NULL)
		free_arg_array(details->arg_array);
	if (details->redirections != NULL)
		free_redirection_array(details->redirections);
	free(details);
}

/**
 * @brief Free all resources in an AST node and its children
 * 
 * @param node AST node to free
 */
void	free_ast(ast_node *node)
{
	if (node == NULL)
		return ;
	if (node->type == COMMAND_NODE)
		free_command_value(node->value);
	else if (node->type == PIPE_NODE)
	{
		free_ast(node->left);
		free_ast(node->right);
	}
	free(node);
}