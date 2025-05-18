/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_expanded.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:35:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 01:35:50 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static char	*handle_variable_expansion(char *raw_command, int *i,
	int exit_status, t_env *env_list, char *result)
{
	char	*varname;
	char	*value;

	varname = get_varname(raw_command, i);
	if (!varname)
	{
		free(result);
		return (NULL);
	}
	value = get_var_value(varname, exit_status, env_list);
	if (!value)
	{
		free(varname);
		free(result);
		return (NULL);
	}
	result = add_str_to_result(result, value);
	free(varname);
	free(value);
	return (result);
}

static char	*handle_regular_char(char *raw_command, int *i, char *result)
{
	result = add_char_to_result(result, raw_command[*i]);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}

char	*get_expanded(char *raw_command, int exit_status, t_env *env_list)
{
	int		i;
	char	*result;

	i = 0;
	result = init_expanded_str();
	if (!result)
		return (NULL);
	while (raw_command[i])
	{
		if (raw_command[i] == '\\')
			result = handle_backslash_expansion(raw_command, &i, result);
		else if (raw_command[i] == '$' && is_need_for_expanding(raw_command, i))
			result = handle_variable_expansion(raw_command, &i,
					exit_status, env_list, result);
		else
			result = handle_regular_char(raw_command, &i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}
