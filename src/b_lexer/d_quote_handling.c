/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_quote_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:45:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:32:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Process token for environment variables - Part 2
** Handles the environment variable expansion
*/
static int	process_token_part2(char *token, int *i, char *result, int *j)
{
	char	env_name[256];
	char	*env_value;
	int		k;

	if (isalnum(token[*i]) || token[*i] == '_')
	{
		k = 0;
		while ((isalnum(token[*i]) || token[*i] == '_') && k < 255)
			env_name[k++] = token[(*i)++];
		env_name[k] = '\0';
		env_value = getenv(env_name);
		if (env_value)
		{
			k = 0;
			while (env_value[k])
				result[(*j)++] = env_value[k++];
		}
		return (1);
	}
	return (0);
}

/*
** Check if a character is a quote (single or double)
** Returns 1 if the character is a quote, 0 otherwise
*/
int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/*
** Update the quote state based on current character
** Returns 1 if the quote state changed, 0 otherwise
*/
int	update_quote_state(char c, char *quote_state)
{
	if (!*quote_state && is_quote(c))
	{
		*quote_state = c;
		return (1);
	}
	else if (*quote_state == c)
	{
		*quote_state = 0;
		return (1);
	}
	return (0);
}

/*
** Process token for quotes and environment variables - Part 1
** Handles token processing and environment variable expansion
*/
static char	*process_token_part1(char *token, int exit_status,
								char *result, int max_len)
{
	int		i;
	int		j;
	char	quote_state;
	char	exit_str[12];
	int		k;

	i = 0;
	j = 0;
	quote_state = 0;
	while (token[i] && j < max_len)
	{
		if (is_quote(token[i]))
		{
			if (quote_state == 0)
			{
				quote_state = token[i++];
				continue;
			}
			else if (quote_state == token[i])
			{
				quote_state = 0;
				i++;
				continue;
			}
		}
		if (token[i] == '$' && quote_state != '\'')
		{
			i++;
			if (token[i] == '?')
			{
				sprintf(exit_str, "%d", exit_status);
				k = 0;
				while (exit_str[k] && j < max_len)
					result[j++] = exit_str[k++];
				i++;
			}
			else if (process_token_part2(token, &i, result, &j))
				continue ;
			else
				result[j++] = '$';
		}
		else
			result[j++] = token[i++];
	}
	result[j] = '\0';
	return (result);
}

/*
** Process token to handle quotes and expand environment variables
** Returns a processed token string with quotes and env vars handled
*/
char	*process_token(char *token, int exit_status)
{
	char	*result;
	int		max_len;

	if (!token)
		return (NULL);
	max_len = strlen(token) * 2;
	result = (char *)malloc(sizeof(char) * (max_len + 1));
	if (!result)
		return (NULL);
	return (process_token_part1(token, exit_status, result, max_len));
}