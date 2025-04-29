/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_char_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:33:50 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ';' || c == '&'
		|| c == '(' || c == ')');
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

int	check_quote_state(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
	{
		*quote = c;
		return (1);
	}
	else if (*quote && c == *quote)
	{
		*quote = 0;
		return (1);
	}
	return (0);
}

int	handle_special_chars(char *str)
{
	if ((str[0] == '&' && str[1] == '&')
		|| (str[0] == '|' && str[1] == '|')
		|| (str[0] == '<' && str[1] == '<')
		|| (str[0] == '>' && str[1] == '>'))
		return (2);
	return (1);
}

int	handle_backslash(char *str, int i, char quote, int *inc)
{
	if (quote == '"' && str[i] == '\\' && str[i + 1])
	{
		if (str[i + 1] == '"' || str[i + 1] == '\\')
		{
			*inc = 1;
			return (1);
		}
	}
	return (0);
}
