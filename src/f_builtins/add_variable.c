/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:30 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int		is_variable(const char *str)
{
	int	i;
	int	len;

	i = 0;
	len = mini_strlen(str);
	if (len < 3)
		return (0);
	while (str[i])
	{
		if (str[i + 1] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	env_lengths(t_env *env, int *count_vars, int *max_len)
{
	int	i;
	int j;

	i = 0;
	while (env->key[i] != '=')
		i++;
	j = i;
	i++;
	while (env->key[i] != '\0')
		i++;
	*count_vars = j;
	*max_len = i - j;
}

void	add_variable(const char *arg, t_env **env_list)
{
	int i;
	int j;
	t_env *new_env;

	j = 0;
	i = 0;
	new_env = malloc(sizeof(t_env));
	if (!new_env)
		exit(1);
	
	// Anahtar değerini kopyala
	while (arg[i] != '=')
		i++;
	new_env->key = malloc((i + 1) * sizeof(char));
	if (!new_env->key)
		exit(1);
	i = 0;
	while (arg[i] != '=')
	{
		new_env->key[i] = arg[i];
		i++;
	}
	new_env->key[i] = '\0';
	
	// Değeri kopyala
	i++;
	j = 0;
	while (arg[i + j])
		j++;
	new_env->value = malloc((j + 1) * sizeof(char));
	if (!new_env->value)
		exit(1);
	j = 0;
	while (arg[i])
	{
		new_env->value[j] = arg[i];
		i++;
		j++;
	}
	new_env->value[j] = '\0';
	new_env->next = NULL;
	
	*env_list = new_env;
}

void	add_list_variable(char **args, t_env **env)
{
    if (*env != NULL)
    {
        t_env *last = mini_lstlast(*env);
        t_env *new_node = NULL;
        add_variable(args[0], &new_node);
        last->next = new_node;
    }
    else
    {
        add_variable(args[0], env);
    }	
}
