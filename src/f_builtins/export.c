/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:01:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:01:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/f_builtins.h"

int		compare_env(t_env **env, t_env **env_var)
{
	t_env *tmp1;
	t_env *tmp2;

	if (!env || !*env || !env_var || !*env_var)
		return (0);
		
	tmp2 = *env_var;
	while (tmp2 != NULL)
	{
		tmp1 = *env;
		while (tmp1 != NULL)
		{
			if (mini_strcmp_path(tmp1->key, tmp2->key) == 1)
				return (0);
			tmp1 = tmp1->next;
		}
		tmp2 = tmp2->next;
	}
	return (1);
}

void	add_env_var(char *key, char *value, t_env **env)
{
	int	i;

	i = 0;
	while (key[i])
		i++;
	(*env)->key = malloc((i + 1) * sizeof(char));
	i = 0;
	while (value[i])
		i++;
	(*env)->value = malloc((i + 1) * sizeof(char));
	i = 0;
	while (key[i])
	{
		(*env)->key[i] = key[i];
		i++;
	}
	(*env)->key[i] = '\0';
	i = 0;
	while (value[i])
	{
		(*env)->value[i] = value[i];
		i++;
	}
	(*env)->value[i] = '\0';
	(*env)->next = NULL;
}

void	export_cmd(char *arg, t_env **env, t_env **env_var)
{
	t_env	*tmp;
	t_env *new_node;

	if (!arg || !env || !env_var || !*env_var)
		return;
		
	if (compare_env(env, env_var) == 0)
		return;
		
	tmp = *env_var;
	while (tmp != NULL)
	{
		if (mini_strcmp_path(tmp->key, arg) == 1)
		{
			t_env *last = mini_lstlast(*env);
			new_node = malloc(sizeof(t_env));
			if (!new_node)
				return;
			add_env_var(tmp->key, tmp->value, &new_node);
			last->next = new_node;
			return;
		}
		tmp = tmp->next;
	}
}
