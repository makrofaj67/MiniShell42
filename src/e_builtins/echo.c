/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:47:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 11:47:23 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	env_value(char *args, t_env *env, int n)
{
	t_env *tmp;

	tmp = env;
	while (tmp != NULL)
	{
		if (mini_strcmp_path(args, (tmp)->key))
		{
			printf("%s ", tmp->value);
			if (n == 0)
				printf("\n");
			return ;
		}
		tmp = tmp->next;
	}
}

void	echo_cmd(char **args, t_env **env)
{
	int	i;
	int n;

	i = 1;
	n = 0;
	if (!args || !args[1])
	{
		printf("\n");
		return;
	}
	if (mini_strcmp_path(args[i], "-n") == 1)
	{
		n = 1;
		i++;
	}
	if (args[i][0] == '$')
	{
		env_value(args[i], *env, n);
		return ;
	}
	while (args[i])
		printf("%s ", args[i++]);
	if (n == 0)
		printf("\n");
}
