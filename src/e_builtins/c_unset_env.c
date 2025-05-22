/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_unset_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:00:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/22 12:00:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

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

int	ft_unset(char **args, t_variable_list *env_list)
{
	int	i;
	int	ret;

	ret = 0;
	if (!args || !args[0])
		return (0);
	i = 0;
	while (args[i])
	{
		if (!is_valid_identifier(args[i]))
		{
			fprintf(stderr, "minishell: unset: `%s': not a valid identifier\n",
				args[i]);
			ret = 1;
		}
		else
			unset_variable(env_list, args[i]);
		i++;
	}
	return (ret);
}

int	ft_env(t_variable_list *env_list)
{
	t_variable_node	*current;

	if (!env_list || !env_list->head)
		return (0);
	current = env_list->head;
	while (current)
	{
		if (current->is_exported)
			printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
	return (0);
}
