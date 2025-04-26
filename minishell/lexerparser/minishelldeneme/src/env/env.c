/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 23:25:00 by luka              #+#    #+#             */
/*   Updated: 2025/04/26 22:51:08 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include <stdlib.h>
#include <string.h>

static t_env	*new_env(const char *key, const char *value)
{
	t_env	*env;

	env = malloc(sizeof(t_env));
	if (!env)
		return (NULL);
	env->key = strdup(key);
	env->value = strdup(value);
	env->next = NULL;
	return (env);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*last;
	int		i;
	char	*eq;

	env = NULL;
	last = NULL;
	i = 0;
	while (envp[i])
	{
		eq = strchr(envp[i], '=');
		if (!eq)
		{
			i++;
			continue ;
		}
		*eq = 0;
		last = new_env(envp[i], eq + 1);
		*eq = '=';
		if (!env)
			env = last;
		else
		{
			t_env *tmp = env;
			while (tmp->next)
				tmp = tmp->next;
			tmp->next = last;
		}
		i++;
	}
	return (env);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		free(env->key);
		free(env->value);
		env = env->next;
		free(tmp);
	}
}

char	*get_env_value(t_env *env, const char *key)
{
	while (env)
	{
		if (!strcmp(env->key, key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	set_env_value(t_env **env, const char *key, const char *value)
{
	t_env	*cur;
	t_env	*new;

	cur = *env;
	while (cur)
	{
		if (!strcmp(cur->key, key))
		{
			free(cur->value);
			cur->value = strdup(value);
			return ;
		}
		cur = cur->next;
	}
	new = malloc(sizeof(t_env));
	new->key = strdup(key);
	new->value = strdup(value);
	new->next = *env;
	*env = new;
}

void	unset_env_value(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (!strcmp(cur->key, key))
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

char	**env_to_envp(t_env *env)
{
	int		count;
	t_env	*tmp;
	char	**envp;
	int		i;
	int		len;
	char	*str;

	count = 0;
	tmp = env;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	envp = malloc(sizeof(char *) * (count + 1));
	i = 0;
	tmp = env;
	while (tmp)
	{
		len = strlen(tmp->key) + 1 + strlen(tmp->value) + 1;
		str = malloc(len);
		sprintf(str, "%s=%s", tmp->key, tmp->value);
		envp[i++] = str;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

void	free_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		free(envp[i++]);
	free(envp);
}