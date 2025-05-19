/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:20:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:20:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Get the HOME directory path
** Returns a newly allocated string with the path, or NULL on error
*/
char *get_path_home(void)
{
	char	*home;

	home = getenv("HOME");
	if (!home)
		return (NULL);
	return (ft_strdup(home));
}

/*
** Get value of an environment variable from the environment list
** Returns a newly allocated string with the value, or NULL if not found
*/
static char *get_env_value(char *key, t_env *env_list)
{
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, key))
			return (ft_strdup(env_list->value));
		env_list = env_list->next;
	}
	return (NULL);
}

/*
** Update PWD and OLDPWD environment variables
*/
static void update_pwd_vars(t_env **env_list)
{
	char	cwd[PATH_MAX];
	char	*old_pwd;

	// Get current working directory
	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("cd: getcwd");
		return;
	}

	// Save old PWD
	old_pwd = get_env_value("PWD", *env_list);

	// Update PWD in env
	unset_cmd("PWD", env_list);
	add_env_var("PWD", cwd, env_list);

	// Update OLDPWD in env
	if (old_pwd)
	{
		unset_cmd("OLDPWD", env_list);
		add_env_var("OLDPWD", old_pwd, env_list);
		free(old_pwd);
	}
}

/*
** CD built-in command implementation
** Changes directory to specified path or HOME if no path is given
*/
int cd_cmd(char **args)
{
	char	*path;
	int		result;
	t_env	*env_list;
	
	// Get the environment list from main environment
	extern t_env *g_env_list;
	env_list = g_env_list;

	// No arguments, go to HOME
	if (!args[1])
	{
		path = get_path_home();
		if (!path)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = ft_strdup(args[1]);

	// Change directory
	result = chdir(path);
	if (result < 0)
	{
		perror("cd");
		free(path);
		return (1);
	}

	// Update PWD and OLDPWD environment variables
	update_pwd_vars(&env_list);

	free(path);
	return (0);
}
