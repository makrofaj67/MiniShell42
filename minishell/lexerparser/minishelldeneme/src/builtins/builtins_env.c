/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:15:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include <stdio.h>
#include <string.h>

int	builtin_export(char **argv, t_env **env)
{
	int	idx;
	char	*eq;

	idx = 1;
	while (argv[idx])
	{
		eq = strchr(argv[idx], '=');
		if (eq)
		{
			*eq = 0;
			set_env_value(env, argv[idx], eq + 1);
			*eq = '=';
		}
		idx++;
	}
	return (0);
}

int	builtin_unset(char **argv, t_env **env)
{
	int	idx;

	idx = 1;
	while (argv[idx])
	{
		unset_env_value(env, argv[idx]);
		idx++;
	}
	return (0);
}

int	builtin_env(t_env *env)
{
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
	return (0);
}
