/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_execute_ast.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:20:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:20:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	execute_ast(ast_node *root, t_variable_list *env_list, int *exit_status)
{
	t_executor_data	data;

	if (!root)
		return (0);
	data.env_list = env_list;
	data.last_exit_status = *exit_status;
	data.last_pid = 0;
	if (root->type == COMMAND_NODE)
		*exit_status = execute_command_node(root, &data);
	else if (root->type == PIPE_NODE)
		*exit_status = execute_pipe_node(root, &data);
	return (*exit_status);
}

int	execute_command_node(ast_node *node, t_executor_data *data)
{
	int	status;

	status = 0;
	if (!node || !node->value)
		return (1);
	if (handle_redirections(node->value) != 0)
		return (1);
	status = execute_simple_command(node->value, data);
	restore_redirections(node->value);
	return (status);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "env") == 0
		|| ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_simple_command(command_value *cmd, t_executor_data *data)
{
	if (!cmd || !cmd->arg_array || !cmd->arg_array[0])
		return (0);
	if (is_builtin(cmd->arg_array[0]))
		return (execute_builtin(cmd, data));
	else
		return (execute_external_command(cmd, data));
}

int	execute_builtin(command_value *cmd, t_executor_data *data)
{
	char	*command;
	int		status;

	command = cmd->arg_array[0];
	status = 0;
	if (ft_strcmp(command, "echo") == 0)
		status = ft_echo(cmd->arg_array + 1);
	else if (ft_strcmp(command, "cd") == 0)
		status = ft_cd(cmd->arg_array + 1, data->env_list);
	else if (ft_strcmp(command, "pwd") == 0)
		status = ft_pwd();
	else if (ft_strcmp(command, "export") == 0)
		status = ft_export(cmd->arg_array + 1, data->env_list);
	else if (ft_strcmp(command, "unset") == 0)
		status = ft_unset(cmd->arg_array + 1, data->env_list);
	else if (ft_strcmp(command, "env") == 0)
		status = ft_env(data->env_list);
	else if (ft_strcmp(command, "exit") == 0)
		status = ft_exit(cmd->arg_array + 1, &data->last_exit_status);
	return (status);
}
