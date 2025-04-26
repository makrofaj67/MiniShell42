/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 22:45:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "executor.h"

static int	parse_and_execute_exit(char **cmds, t_env **env, int i)
{
	char	**args;

	args = split_args(cmds[i], *env, g_last_status);
	if (is_builtin(args[0]) && !strcmp(args[0], "exit"))
	{
		free_pipes(cmds);
		builtin_exit(args);
		return (1);
	}
	return (0);
}

int	parse_and_execute(char *line, t_env **env)
{
	char	**cmds;
	int		n;
	int		prev_fd;
	int		last_status;
	int		i;

	if (!line || !*line)
		return (0);
	cmds = split_pipes(line);
	n = 0;
	while (cmds[n])
		n++;
	prev_fd = -1;
	last_status = 0;
	i = 0;
	while (i < n)
	{
		if (parse_and_execute_exit(cmds, env, i))
			return (1);
		execute_command(cmds[i], env, &prev_fd, i, n);
		i++;
	}
	wait_for_children(n, &last_status);
	g_last_status = last_status;
	if (prev_fd != -1)
		close(prev_fd);
	free_pipes(cmds);
	return (0);
}