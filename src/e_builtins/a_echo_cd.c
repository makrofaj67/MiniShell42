/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_echo_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:50:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:50:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	ft_echo(char **args)
{
	int	i;
	int	n_flag;

	n_flag = 0;
	i = 0;
	if (args && args[0] && ft_strcmp(args[0], "-n") == 0)
	{
		n_flag = 1;
		i = 1;
	}
	while (args && args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (!n_flag)
		printf("\n");
	return (0);
}

static void	update_pwd_vars(t_variable_list *env_list, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return ;
	set_variable(env_list, "OLDPWD", old_pwd, 1);
	set_variable(env_list, "PWD", cwd, 1);
}

int	ft_cd(char **args, t_variable_list *env_list)
{
	char	*path;
	char	*old_pwd;
	char	cwd[PATH_MAX];

	if (!args || !args[0])
	{
		path = get_variable_value(env_list, "HOME");
		if (!path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
	}
	else
		path = args[0];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (1);
	old_pwd = ft_strdup(cwd);
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		free(old_pwd);
		return (1);
	}
	update_pwd_vars(env_list, old_pwd);
	free(old_pwd);
	return (0);
}
