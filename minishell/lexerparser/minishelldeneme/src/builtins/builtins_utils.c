/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:05:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <sys/stat.h>

int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	if (!strcmp(cmd, "export"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(char **argv, t_env **env)
{
	if (!strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	if (!strcmp(argv[0], "cd"))
		return (builtin_cd(argv, env));
	if (!strcmp(argv[0], "pwd"))
		return (builtin_pwd());
	if (!strcmp(argv[0], "export"))
		return (builtin_export(argv, env));
	if (!strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env));
	if (!strcmp(argv[0], "env"))
		return (builtin_env(*env));
	if (!strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}

char	*find_in_path(const char *cmd, t_env *env)
{
	char	*path;
	char	*paths;
	char	*saveptr;
	char	*dir;
	struct stat st;
	char	full[1024];

	if (!cmd || strchr(cmd, '/'))
		return (strdup(cmd));
	path = get_env_value(env, "PATH");
	if (!path)
		return (NULL);
	paths = strdup(path);
	saveptr = NULL;
	dir = strtok_r(paths, ":", &saveptr);
	while (dir)
	{
		snprintf(full, sizeof(full), "%s/%s", dir, cmd);
		if (!stat(full, &st) && (st.st_mode & S_IXUSR))
		{
			free(paths);
			return (strdup(full));
		}
		dir = strtok_r(NULL, ":", &saveptr);
	}
	free(paths);
	return (NULL);
}
