/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_create_ast_nodes.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:23:10 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 16:24:25 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

ast_node *create_pipe_node(ast_node *left_child, ast_node *right_child)
{
	ast_node *pipe_node;
	pipe_node = (ast_node *)malloc(sizeof(ast_node));
	if (pipe_node == NULL)
		return (NULL);
	pipe_node->type = PIPE_NODE;
	pipe_node->left = left_child;
	pipe_node->right = right_child;
	pipe_node->value = NULL;
	return(pipe_node);
}

ast_node *create_command_node(command_value *cmd_details)
{
	ast_node *command_node;
	command_node = (ast_node *)malloc(sizeof(ast_node));
	if (command_node == NULL)
		return (NULL);
	command_node->type = COMMAND_NODE;
	command_node->left = NULL;
	command_node->right = NULL;
	command_node->value = cmd_details;
	return(command_node);
}


