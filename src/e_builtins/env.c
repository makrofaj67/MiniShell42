/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:23 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	print_nodes(t_env **tmp)
{
	t_env *env;
    if (tmp == NULL || *tmp == NULL)
    {
        printf("Environment list is empty.\n");
        return;
    }
	env = *tmp;
	while (env != NULL)
	{
		printf("%s=%s\n", env->key, env->value);
		env = env->next;
	}
}

void	env_lengths_2(t_env **len, char *args)
{
	int	i;
	int j;

	i = 0;
	*len = malloc(sizeof(t_env));
	if (*len == NULL)
        exit(1);
	while (args[i] != '=')
		i++;
	j = i;
	i++;
	while (args[i] != '\0')
		i++;
	i = i - j;
	(*len)->key = malloc((j + 1) * sizeof(char));
	(*len)->value = malloc((i + 1) * sizeof(char));
	if (!(*len)->key || !(*len)->value)
		exit(1);
}
void	add_variable_2(char *args, t_env **env)
{
	int i;
	int j;

	j = 0;
	i = 0;
	env_lengths_2(env, args);
	while (args[i] != '\0')
	{
		(*env)->key[i] = args[i];
		i++;
		if (args[i] == '=')
			break;
	}
	(*env)->key[i] = '\0';
	i++; 
	while (args[i] != '\0')
	{
		(*env)->value[j] = args[i];
		i++;
		j++;
	}
	(*env)->value[j] = '\0';
	(*env)->next = NULL;
}

void	create_env(t_env **env)
{
	char	**env_var;
	int 	i;

	i = 0;
	env_var = __environ;
	while (env_var[i] != NULL)
	{
		if (*env != NULL)
   		{
       		t_env *last = mini_lstlast(*env);
        	t_env *new_node = NULL;
        	add_variable_2(env_var[i], &new_node);
        	last->next = new_node;
    	}
    	else
		{
        	add_variable_2(env_var[i], env);
		}
		i++;
	}
}

void free_env_list(t_env **env)
{
    t_env *current;
    t_env *next;

    if (env == NULL || *env == NULL)
        return;

    current = *env;
    while (current != NULL)
    {
        next = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next;
    }
    *env = NULL;
}