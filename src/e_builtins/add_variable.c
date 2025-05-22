/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_variable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:30 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:30 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int		is_variable(char **args)
{
	int	i;
	int	len;

	i = 0;
	if ( args[0] == NULL)
		return (0);
	len = mini_strlen(args[0]);
	if (len < 3 && args[1] == NULL)
		return (0);
	while (args[0][i])
	{
		if (args[0][i + 1] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	env_lengths(t_env **len, char **args)
{
	int	i;
	int j;

	i = 0;
	*len = malloc(sizeof(t_env));
	while (args[0][i] != '=')
		i++;
	j = i;
	i++;
	while (args[0][i] != '\0')
		i++;
	i = i - j;
	(*len)->key = malloc((j + 1) * sizeof(char));
	(*len)->value = malloc((i + 1) * sizeof(char));
	if (!(*len)->key || !(*len)->value)
		exit(1);
}

void	add_variable(char **args, t_env **env)
{
	int i;
	int j;

	j = 0;
	i = 0;
	env_lengths(env, args);
	while (args[0][i] != '\0')
	{
		(*env)->key[i] = args[0][i];
		i++;
		if (args[0][i] == '=')
			break;
	}
	(*env)->key[i] = '\0';
	i++; 
	while (args[0][i] != '\0')
	{
		(*env)->value[j] = args[0][i];
		i++;
		j++;
	}
	(*env)->value[j] = '\0';
	(*env)->next = NULL;
}

void	add_list_variable(char **args, t_env **env)
{
    if (*env != NULL)
    {
        t_env *last = mini_lstlast(*env);
        t_env *new_node = NULL;
        add_variable(args, &new_node);
        last->next = new_node;
    }
    else
    {
        add_variable(args, env);
    }	
}
