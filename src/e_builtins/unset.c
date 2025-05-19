/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:40:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:40:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Check if a character is valid for an environment variable name
** Valid characters are alphanumeric and underscore
*/
static int is_valid_env_char(char c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
			(c >= '0' && c <= '9') || c == '_');
}

/*
** Check if a key is valid for an environment variable
** Must start with a letter or underscore, and contain only valid characters
*/
static int is_valid_env_key(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	
	if (*key != '_' && !(*key >= 'a' && *key <= 'z') && !(*key >= 'A' && *key <= 'Z'))
		return (0);
	
	i = 1;
	while (key[i])
	{
		if (!is_valid_env_char(key[i]))
			return (0);
		i++;
	}
	
	return (1);
}

/*
** Remove an environment variable from the list
*/
static void remove_env_var(char *key, t_env **env_list)
{
	t_env	*prev;
	t_env	*curr;

	if (!env_list || !*env_list)
		return;
	
	// Handle case where target is first node
	if (!ft_strcmp((*env_list)->key, key))
	{
		curr = *env_list;
		*env_list = (*env_list)->next;
		free(curr->key);
		free(curr->value);
		free(curr);
		return;
	}
	
	// Search for node
	prev = *env_list;
	curr = prev->next;
	while (curr)
	{
		if (!ft_strcmp(curr->key, key))
		{
			prev->next = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

/*
** Unset built-in command implementation
** Removes variables from environment
*/
void unset_cmd(char *arg, t_env **env_list)
{
	// Validate key
	if (!is_valid_env_key(arg))
	{
		fprintf(stderr, "unset: `%s': not a valid identifier\n", arg);
		return;
	}
	
	// Remove from environment
	remove_env_var(arg, env_list);
}
