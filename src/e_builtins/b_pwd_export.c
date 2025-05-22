/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_pwd_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 11:55:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 11:55:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	ft_pwd(void)
{
	char	cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		fprintf(stderr, "minishell: pwd: %s\n", strerror(errno));
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static void	print_exported_vars(t_variable_list *env_list)
{
	t_variable_node	*current;

	if (!env_list || !env_list->head)
		return ;
	current = env_list->head;
	while (current)
	{
		if (current->is_exported)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		current = current->next;
	}
}

static int	is_valid_identifier(char *key)
{
	int	i;

	if (!key || !*key)
		return (0);
	if (!((key[0] >= 'a' && key[0] <= 'z') || (key[0] >= 'A' && key[0] <= 'Z')
			|| key[0] == '_'))
		return (0);
	i = 1;
	while (key[i])
	{
		if (!((key[i] >= 'a' && key[i] <= 'z')
				|| (key[i] >= 'A' && key[i] <= 'Z')
				|| (key[i] >= '0' && key[i] <= '9') || key[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	handle_export_arg(char *arg, t_variable_list *env_list)
{
	char	*key;
	char	*value;
	char	*equals_pos;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
	{
		key = ft_strdup(arg);
		if (!is_valid_identifier(key))
		{
			fprintf(stderr, "minishell: export: `%s': not a valid identifier\n",
				arg);
			free(key);
			return (1);
		}
		set_variable(env_list, key, "", 1);
		free(key);
		return (0);
	}
	key = ft_substr(arg, 0, equals_pos - arg);
	if (!is_valid_identifier(key))
	{
		fprintf(stderr, "minishell: export: `%s': not a valid identifier\n", arg);
		free(key);
		return (1);
	}
	value = ft_strdup(equals_pos + 1);
	set_variable(env_list, key, value, 1);
	free(key);
	free(value);
	return (0);
}

int	ft_export(char **args, t_variable_list *env_list)
{
	int	i;
	int	ret;

	ret = 0;
	if (!args || !args[0])
	{
		print_exported_vars(env_list);
		return (0);
	}
	i = 0;
	while (args[i])
	{
		if (handle_export_arg(args[i], env_list) != 0)
			ret = 1;
		i++;
	}
	return (ret);
}
