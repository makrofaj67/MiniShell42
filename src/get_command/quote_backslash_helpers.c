/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_backslash_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 15:59:47 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 16:52:01 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	trim_backslash_if_needed(char *str)
{
	int	len;

	len = strlen(str);
	if (len > 0 && str[len - 1] == '\\')
	{
		str[len - 1] = '\0';
		return (1);
	}
	return (0);
}

int	has_unclosed_quotes(const char *str)
{
	int	quote_char;
	int	i;

	i = 0;
	quote_char = 0;
	while (str[i])
	{
		if (!quote_char && (str[i] == '\'' || str[i] == '"'))
			quote_char = str[i];
		else if (quote_char == '\'' && str[i] == '\'')
			quote_char = 0;
		else if (quote_char == '"')
		{
			if (str[i] == '\\' && (str[i + 1] == '"' || str[i + 1] == '\\'))
				i++;
			else if (str[i] == '"')
				quote_char = 0;
		}
		i++;
	}
	return (quote_char != 0);
}
