/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:30:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:30:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Print all nodes in the environment list
*/
void print_nodes(t_env **tmp)
{
	t_env	*env;

	env = *tmp;
	while (env)
	{
		if (env->value)
			printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

/*
** Initialize environment list from system environment
*/
extern char **environ;

void create_env(t_env **env)
{
	char	**env_array;
	int		i;
	char	*equals;
	char	*key;
	char	*value;

	env_array = environ;  // Get system environment
	i = 0;
	while (env_array[i])
	{
		equals = ft_strchr(env_array[i], '=');
		if (equals)
		{
			// Split into key and value
			key = ft_substr(env_array[i], 0, equals - env_array[i]);
			value = ft_strdup(equals + 1);
			
			if (key && value)
				add_env_var(key, value, env);
			
			free(key);
			free(value);
		}
		i++;
	}
}

/*
** Add an environment variable to the list
*/
void add_env_var(char *key, char *value, t_env **env)
{
	t_env	*new_env;
	t_env	*last;

	// Create new node
	new_env = (t_env *)malloc(sizeof(t_env));
	if (!new_env)
		return;
	
	new_env->key = ft_strdup(key);
	new_env->value = ft_strdup(value);
	new_env->next = NULL;
	
	// Add to end of list
	if (!*env)
		*env = new_env;
	else
	{
		last = *env;
		while (last->next)
			last = last->next;
		last->next = new_env;
	}
}

/*
** Free environment list and all memory associated with it
*/
void free_env_list(t_env *env_list)
{
	t_env	*current;
	t_env	*next;

	current = env_list;
	while (current)
	{
		next = current->next;
		if (current->key)
			free(current->key);
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}
