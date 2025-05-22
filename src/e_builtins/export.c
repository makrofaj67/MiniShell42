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

#include "../../inc/__minishell.h"

int		compare_env(t_env **env, char *arg)
{
	t_env *tmp1;

	tmp1 = *env;

	while (tmp1 != NULL)
	{
		if (mini_strcmp_path(tmp1->key, arg) == 1)
		{
			tmp1 = NULL;
			return (0);
		}
		tmp1 = tmp1->next;
	}
	free(tmp1);
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

	new_node = NULL;
	tmp = *env_var;
	if (compare_env(env, arg) == 0)
		return ;
	while (tmp != NULL)
	{
		if (mini_strcmp_path(tmp->key, arg) == 1)
		{
        	t_env *last = mini_lstlast(*env);
        	new_node = malloc(sizeof(t_env));
        	add_env_var(tmp->key, tmp->value, &new_node);
        	last->next = new_node;
			return ;
		}
		tmp = tmp->next;
	}
}
