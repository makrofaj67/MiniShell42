/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:35:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:35:00 by rakman           ###   ########.fr       */
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
** Parse an environment variable assignment string
** Returns an array with [key, value]
*/
static char **parse_env_assignment(char *arg)
{
	char	**result;
	char	*equals;
	int		key_len;

	// Allocate result array
	result = (char **)malloc(sizeof(char *) * 3);
	if (!result)
		return (NULL);
	
	// Find equals sign
	equals = ft_strchr(arg, '=');
	if (!equals)
	{
		// No assignment, just a variable name
		result[0] = ft_strdup(arg);
		result[1] = ft_strdup("");  // Empty value
		result[2] = NULL;
		return (result);
	}
	
	// Split into key and value
	key_len = equals - arg;
	result[0] = ft_substr(arg, 0, key_len);
	result[1] = ft_strdup(equals + 1);
	result[2] = NULL;
	
	return (result);
}

/*
** Print environment variables for export command
** Similar to print_nodes but with "declare -x " prefix and quotes around values
*/
static void print_export(t_env **env_list)
{
	t_env	*env;

	env = *env_list;
	while (env)
	{
		printf("declare -x %s", env->key);
		if (env->value && *env->value)
			printf("=\"%s\"", env->value);
		printf("\n");
		env = env->next;
	}
}

/*
** Update an existing environment variable
*/
static void update_env_var(char *key, char *value, t_env **env_list)
{
	t_env	*env;

	env = *env_list;
	while (env)
	{
		if (!ft_strcmp(env->key, key))
		{
			// Found the variable, update its value
			free(env->value);
			env->value = ft_strdup(value);
			return;
		}
		env = env->next;
	}
	
	// Variable not found, add it
	add_env_var(key, value, env_list);
}

/*
** Export built-in command implementation
** Handles displaying and setting environment variables
*/
void export_cmd(char *arg, t_env **env_list, t_env **export_list)
{
	char	**kv;
	
	// No arguments, display sorted list
	if (!arg)
	{
		print_export(export_list);
		return;
	}
	
	// Parse argument
	kv = parse_env_assignment(arg);
	if (!kv)
		return;
	
	// Validate key
	if (!is_valid_env_key(kv[0]))
	{
		fprintf(stderr, "export: `%s': not a valid identifier\n", arg);
		free(kv[0]);
		free(kv[1]);
		free(kv);
		return;
	}
	
	// Update both environment and export lists
	update_env_var(kv[0], kv[1], env_list);
	update_env_var(kv[0], kv[1], export_list);
	
	// Clean up
	free(kv[0]);
	free(kv[1]);
	free(kv);
}
