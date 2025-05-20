/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1a_get_var_val.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:20:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:42:36 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*add_char_to_result(char *result, char c)
{
	char		*new_result;
	size_t		len;

	len = ft_strlen(result);
	new_result = (char *)malloc(sizeof(char) * (len + 2));
	if (new_result == NULL)
	{
		free(result);
		return (NULL);
	}
	ft_strcpy(new_result, result);
	new_result[len] = c;
	new_result[len + 1] = '\0';
	free(result);
	return (new_result);
}

char	*add_str_to_result(char *result, char *str)
{
	char	*new_result;

	new_result = ft_strjoin(result, str);
	free(result);
	return (new_result);
}

char	*get_var_value(char *varname, int exit_status, t_env *env_list)
{
	t_env	*current;
	char	*exit_status_str;

	if (ft_strcmp(varname, "?") == 0)
	{
		exit_status_str = ft_itoa(exit_status);
		if (!exit_status_str)
			return (ft_strdup("0"));
		return (exit_status_str);
	}	
	current = env_list;
	while (current)
	{
		if (ft_strcmp(current->key, varname) == 0)
			return (ft_strdup(current->value));
		current = current->next;
	}
	return (ft_strdup(""));
}

char	*get_varname(char *r_cmd, int *i)
{
	int	dollar_pos;
	int	vst;
	int	vln;

	dollar_pos = *i;
	vst = dollar_pos + 1;
	vln = 0;
	if (r_cmd[vst] == '?')
	{
		vln = 1;
		*i = vst + vln;
		return (ft_strdup("?"));
	}
	while (r_cmd[vst + vln]
		&& ((r_cmd[vst + vln] >= 'a' && r_cmd[vst + vln] <= 'z')
			|| (r_cmd[vst + vln] >= 'A' && r_cmd[vst + vln] <= 'Z')
			|| (r_cmd[vst + vln] >= '0' && r_cmd[vst + vln] <= '9')
			|| r_cmd[vst + vln] == '_'))
		vln++;
	*i = vst + vln;
	return (ft_substr(r_cmd, vst, vln));
}
