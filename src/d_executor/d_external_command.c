/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_external_command.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:35:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:35:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	child_process(command_value *cmd, t_executor_data *data)
{
	char	*cmd_path;
	char	**env_array;

	cmd_path = find_command_path(cmd->arg_array[0], data->env_list);
	if (!cmd_path)
	{
		fprintf(stderr, "minishell: %s: command not found\n", cmd->arg_array[0]);
		exit(127);
	}
	env_array = create_env_array(data->env_list);
	if (!env_array)
	{
		free(cmd_path);
		exit(1);
	}
	execve(cmd_path, cmd->arg_array, env_array);
	fprintf(stderr, "minishell: %s: %s\n", cmd->arg_array[0], strerror(errno));
	free(cmd_path);
	free_str_array(env_array);
	exit(126);
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*temp;
	char	*result;

	temp = ft_strjoin(s1, s2);
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, s3);
	free(temp);
	return (result);
}

int	execute_external_command(command_value *cmd, t_executor_data *data)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (1);
	if (pid == 0)
		child_process(cmd, data);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}
