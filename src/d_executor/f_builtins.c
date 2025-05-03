/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_builtins.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 21:50:15 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 21:48:59 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include "../../inc/f_builtins.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>

/**
 * Check if a command is a builtin
 */
int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	return (strcmp(cmd, "echo") == 0 || strcmp(cmd, "cd") == 0 ||
			strcmp(cmd, "pwd") == 0 || strcmp(cmd, "export") == 0 ||
			strcmp(cmd, "unset") == 0 || strcmp(cmd, "env") == 0 ||
			strcmp(cmd, "exit") == 0);
}

/**
 * Execute a builtin command
 */
int execute_builtin(command_value *cmd, t_exec_context *context)
{
	char *command;

	if (!cmd || !cmd->arg_array || !cmd->arg_array[0])
		return (1);
	command = cmd->arg_array[0];
	if (strcmp(command, "echo") == 0)
		return (builtin_echo(cmd->arg_array, context->envp));
	else if (strcmp(command, "cd") == 0)
		return (builtin_cd(cmd->arg_array, context->envp));
	else if (strcmp(command, "pwd") == 0)
		return (builtin_pwd(cmd->arg_array, context->envp));
	else if (strcmp(command, "export") == 0)
		return (builtin_export(cmd->arg_array, context->envp));
	else if (strcmp(command, "unset") == 0)
		return (builtin_unset(cmd->arg_array, context->envp));
	else if (strcmp(command, "env") == 0)
		return (builtin_env(cmd->arg_array, context->envp));
	else if (strcmp(command, "exit") == 0)
		return (builtin_exit(cmd->arg_array, context->envp));
	return (1);
}

/**
 * Implementation of echo builtin with -n option
 */
int builtin_echo(char **args, char **envp)
{
	int i;
	int n_flag;

	(void)envp;
	n_flag = 0;
	i = 1;
	// Check for -n option
	if (args[i] && strcmp(args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	// Print all arguments with spaces in between
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	// Print newline unless -n flag is set
	if (!n_flag)
		printf("\n");
	fflush(stdout);
	return (0);
}

/**
 * Implementation of cd builtin
 */
int builtin_cd(char **args, char **envp)
{
	char *path;
	char *home;
	int i;

	(void)envp;
	if (!args[1])
	{
		// If no argument, go to HOME directory
		i = 0;
		home = NULL;
		while (envp[i])
		{
			if (strncmp(envp[i], "HOME=", 5) == 0)
			{
				home = envp[i] + 5;
				break;
			}
			i++;
		}
		if (!home)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			return (1);
		}
		path = home;
	}
	else
	{
		path = args[1];
	}
	// Change directory
	if (chdir(path) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

/**
 * Implementation of pwd builtin
 */
int builtin_pwd(char **args, char **envp)
{
	char cwd[PATH_MAX];

	(void)args;
	(void)envp;
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

/**
 * Find environment variable by name
 */
int find_env_var(char **envp, char *name)
{
	int i;
	int len;

	i = 0;
	len = strlen(name);
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && 
			(envp[i][len] == '=' || envp[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

/**
 * Count elements in NULL-terminated array
 */
int count_array(char **array)
{
	int count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}

/**
 * Check if environment variable name is valid
 */
int is_valid_env_name(char *name)
{
	int i;

	if (!name || !name[0] || !(isalpha(name[0]) || name[0] == '_'))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!(isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/**
 * Add or update environment variable
 */
char **add_env_var(char **envp, char *var)
{
	char **new_envp;
	char *name;
	char *equals;
	int i;
	int count;

	equals = strchr(var, '=');
	if (!equals)
		return (envp);  // Not a valid variable assignment
	
	// Extract variable name
	name = (char *)malloc(equals - var + 1);
	if (!name)
		return (envp);
	strncpy(name, var, equals - var);
	name[equals - var] = '\0';
	
	// Check if name is valid
	if (!is_valid_env_name(name))
	{
		fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", name);
		free(name);
		return (envp);
	}
	
	// Find if variable already exists
	i = find_env_var(envp, name);
	if (i >= 0)
	{
		// Replace existing variable
		free(envp[i]);
		envp[i] = strdup(var);
		free(name);
		return (envp);
	}
	
	// Add new variable
	count = count_array(envp);
	new_envp = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(name);
		return (envp);
	}
	
	// Copy existing variables
	for (i = 0; i < count; i++)
		new_envp[i] = envp[i];
	
	// Add new variable
	new_envp[count] = strdup(var);
	new_envp[count + 1] = NULL;
	
	free(name);
	// We're not freeing the original envp pointer since it might be the original main() argument
	return (new_envp);
}

/**
 * Implementation of export builtin
 */
int builtin_export(char **args, char **envp)
{
	int i;
	int ret;

	ret = 0;
	if (!args[1])
	{
		// Print all variables sorted and in export format
		i = 0;
		while (envp[i])
		{
			printf("declare -x %s\n", envp[i]);
			i++;
		}
		return (0);
	}
	
	// Add each variable to environment
	i = 1;
	while (args[i])
	{
		if (strchr(args[i], '='))
			envp = add_env_var(envp, args[i]);
		else if (!is_valid_env_name(args[i]))
		{
			fprintf(stderr, "minishell: export: '%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
}

/**
 * Remove environment variable
 */
char **remove_env_var(char **envp, char *name)
{
	int i;
	int j;
	int count;
	char **new_envp;

	i = find_env_var(envp, name);
	if (i < 0)
		return (envp);  // Variable doesn't exist
	
	count = count_array(envp);
	new_envp = (char **)malloc(sizeof(char *) * count);
	if (!new_envp)
		return (envp);
	
	// Copy variables except the one to remove
	for (j = 0; j < i; j++)
		new_envp[j] = envp[j];
	for (j = i; j < count - 1; j++)
		new_envp[j] = envp[j + 1];
	new_envp[count - 1] = NULL;
	
	free(envp[i]);  // Free the removed variable
	// We're not freeing the original envp pointer since it might be the original main() argument
	return (new_envp);
}

/**
 * Implementation of unset builtin
 */
int builtin_unset(char **args, char **envp)
{
	int i;
	int ret;

	ret = 0;
	i = 1;
	while (args[i])
	{
		if (is_valid_env_name(args[i]))
			envp = remove_env_var(envp, args[i]);
		else
		{
			fprintf(stderr, "minishell: unset: '%s': not a valid identifier\n", args[i]);
			ret = 1;
		}
		i++;
	}
	return (ret);
}

/**
 * Implementation of env builtin
 */
int builtin_env(char **args, char **envp)
{
	int i;

	(void)args;
	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
	return (0);
}

/**
 * Implementation of exit builtin
 */
int builtin_exit(char **args, char **envp)
{
	int status;

	(void)envp;
	printf("exit\n");
	if (!args[1])
		exit(0);
	
	// Check if argument is a number
	status = 0;
	for (int i = 0; args[1][i]; i++)
	{
		if (!isdigit(args[1][i]) && !(i == 0 && (args[1][i] == '+' || args[1][i] == '-')))
		{
			fprintf(stderr, "minishell: exit: %s: numeric argument required\n", args[1]);
			exit(255);
		}
	}
	
	// Convert argument to exit status
	status = atoi(args[1]);
	if (args[2])
	{
		fprintf(stderr, "minishell: exit: too many arguments\n");
		return (1);
	}
	
	exit(status & 255);  // Exit status is masked to 8 bits
	return (0);  // This line will never be reached
}