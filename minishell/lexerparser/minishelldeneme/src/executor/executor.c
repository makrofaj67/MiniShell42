/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:55:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include "parser.h"

static void	exec_child(char *cmd, t_env **env, int in_fd, int out_fd)
{
	char	**argv;
	char	*path;
	char	**envp;

	handle_redirection(cmd, &in_fd, &out_fd);
	if (in_fd != -1)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != -1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
	argv = split_args(cmd, *env, g_last_status);
	if (is_builtin(argv[0]))
		exec_builtin(argv, env);
	else
	{
		path = find_in_path(argv[0], *env);
		if (!path)
		{
			fprintf(stderr, "%s: command not found\n", argv[0]);
			exit(127);
		}
		envp = env_to_envp(*env);
		execve(path, argv, envp);
		perror("execve");
		free_envp(envp);
		exit(127);
	}
}

void	execute_command(char *cmd, t_env **env, int *prev_fd, int idx, int n)
{
	int		pipefd[2];
	pid_t	pid;
	int		in_fd;
	int		out_fd;

	in_fd = -1;
	out_fd = -1;
	if (idx < n - 1)
		pipe(pipefd);
	pid = fork();
	if (pid == 0)
	{
		if (idx > 0)
		{
			dup2(*prev_fd, 0);
			close(*prev_fd);
		}
		if (idx < n - 1)
		{
			close(pipefd[0]);
			dup2(pipefd[1], 1);
			close(pipefd[1]);
		}
		exec_child(cmd, env, in_fd, out_fd);
	}
	else if (pid > 0)
	{
		if (*prev_fd != -1)
			close(*prev_fd);
		if (idx < n - 1)
		{
			close(pipefd[1]);
			*prev_fd = pipefd[0];
		}
	}
}

void	wait_for_children(int n, int *last_status)
{
	int	status;
	int	i;

	i = 0;
	while (i < n)
	{
		status = 0;
		wait(&status);
		if (i == n - 1)
		{
			if (WIFEXITED(status))
				*last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				*last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
}