/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_execute_pipe.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:30:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:30:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	execute_left_child(ast_node *left, t_executor_data *data, int pipefd[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		if (left->type == COMMAND_NODE)
			status = execute_command_node(left, data);
		else
			status = execute_pipe_node(left, data);
		exit(status);
	}
	return (0);
}

static int	execute_right_child(ast_node *right, t_executor_data *data, int pipefd[2])
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (right->type == COMMAND_NODE)
			status = execute_command_node(right, data);
		else
			status = execute_pipe_node(right, data);
		exit(status);
	}
	data->last_pid = pid;
	return (0);
}

int	execute_pipe_node(ast_node *node, t_executor_data *data)
{
	int		pipefd[2];
	int		status;

	if (!node || !node->left || !node->right)
		return (1);
	if (pipe(pipefd) == -1)
		return (1);
	if (execute_left_child(node->left, data, pipefd) != 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	if (execute_right_child(node->right, data, pipefd) != 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		return (1);
	}
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(data->last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
