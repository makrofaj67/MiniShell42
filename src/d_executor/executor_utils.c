/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 03:10:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 03:10:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Check if a command is a built-in command
** Returns 1 if it is a builtin, 0 otherwise
*/
int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export") ||
			!ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "env") ||
			!ft_strcmp(cmd, "exit"));
}

/*
** Execute a built-in command
** Returns the exit status of the command
*/
int execute_builtin(char **args, t_env **env_list, t_env **export_list)
{
	if (!args || !args[0])
		return (1);

	if (!ft_strcmp(args[0], "echo"))
		return (echo_cmd(args, env_list));
	else if (!ft_strcmp(args[0], "cd"))
		return (cd_cmd(args));
	else if (!ft_strcmp(args[0], "pwd"))
	{
		pwd_cmd();
		return (0);
	}
	else if (!ft_strcmp(args[0], "export"))
	{
		if (args[1])
			export_cmd(args[1], env_list, export_list);
		else
			print_nodes(export_list); // Print sorted export list
		return (0);
	}
	else if (!ft_strcmp(args[0], "unset"))
	{
		if (args[1])
			unset_cmd(args[1], env_list);
		return (0);
	}
	else if (!ft_strcmp(args[0], "env"))
	{
		print_nodes(env_list); // Print environment variables
		return (0);
	}
	else if (!ft_strcmp(args[0], "exit"))
		return (exit_cmd(args));

	return (1);
}

/*
** Convert environment list to array for execve
*/
char **env_to_array(t_env *env_list)
{
	int		count;
	char	**env_array;
	t_env	*current;
	int		i;
	char	*tmp;

	// Count number of environment variables
	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}

	// Allocate array
	env_array = (char **)malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);

	// Fill array
	i = 0;
	current = env_list;
	while (current)
	{
		tmp = ft_strjoin(current->key, "=");
		if (!tmp)
		{
			free_array(env_array);
			return (NULL);
		}
		env_array[i] = ft_strjoin(tmp, current->value);
		free(tmp);
		if (!env_array[i])
		{
			free_array(env_array);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env_array[i] = NULL;
	return (env_array);
}

/*
** Find a command in PATH
** Returns the full path or NULL if not found
*/
char *find_command_in_path(char *cmd, t_env *env_list)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	// Get PATH environment variable
	path_env = NULL;
	while (env_list)
	{
		if (!ft_strcmp(env_list->key, "PATH"))
		{
			path_env = env_list->value;
			break;
		}
		env_list = env_list->next;
	}
	
	if (!path_env)
		return (NULL);

	// Split PATH into directories
	paths = split_by_char(path_env, ':');
	if (!paths)
		return (NULL);

	// Try each directory
	i = 0;
	while (paths[i])
	{
		full_path = path_join(paths[i], cmd);
		if (!full_path)
		{
			free_array(paths);
			return (NULL);
		}
		
		if (access(full_path, X_OK) == 0)
		{
			free_array(paths);
			return (full_path);
		}
		
		free(full_path);
		i++;
	}
	
	free_array(paths);
	return (NULL);
}

/*
** Join path components
*/
char *path_join(char *dir, char *file)
{
	char	*tmp;
	char	*result;
	int		len;

	len = ft_strlen(dir);
	
	// Check if dir ends with '/'
	if (len > 0 && dir[len - 1] == '/')
		return (ft_strjoin(dir, file));
	
	// Otherwise, add a '/'
	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	
	result = ft_strjoin(tmp, file);
	free(tmp);
	
	return (result);
}

/*
** Free an array of strings
*/
void free_array(char **array)
{
	int	i;

	if (!array)
		return;
	
	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/*
** Split a string by a delimiter character
*/
char **split_by_char(char *str, char c)
{
	int		count;
	char	**result;
	int		i;
	int		start;
	int		j;

	// Count number of substrings
	count = 1;
	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			count++;
		i++;
	}

	// Allocate array
	result = (char **)malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);

	// Fill array
	i = 0;
	j = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == c || str[i + 1] == '\0')
		{
			if (str[i] == c)
				result[j] = ft_substr(str, start, i - start);
			else
				result[j] = ft_substr(str, start, i - start + 1);
			
			if (!result[j])
			{
				free_array(result);
				return (NULL);
			}
			j++;
			start = i + 1;
		}
		i++;
	}
	result[j] = NULL;
	return (result);
}

/*
** Exit built-in command
*/
int exit_cmd(char **args)
{
	int	exit_code;

	if (!args[1])
	{
		printf("exit\n");
		exit(0);
	}
	
	// Check if argument is numeric
	if (!is_numeric(args[1]))
	{
		printf("exit\n");
		fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
		exit(255);
	}
	
	// Check if there are too many arguments
	if (args[1] && args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	
	exit_code = ft_atoi(args[1]);
	printf("exit\n");
	exit(exit_code % 256);
}

/*
** Check if a string is numeric
*/
int is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	
	return (1);
}

/*
** Convert string to integer
*/
int ft_atoi(const char *str)
{
	int	result;
	int	sign;
	int	i;

	result = 0;
	sign = 1;
	i = 0;
	
	// Skip whitespace
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	
	// Handle sign
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	
	// Convert digits
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	
	return (result * sign);
}
