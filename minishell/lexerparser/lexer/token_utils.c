/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:23:31 by rakman            #+#    #+#             */
/*   Updated: 2025/04/26 19:26:46 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

int	handle_quotes(char *command, int start)
{
	int	i;
	int	quote;

	i = start;
	quote = command[i];
	i++;
	while (command[i] && command[i] != quote)
		i++;
	if (command[i] == quote)
		return (i + 1);
	return (-1);
}

int	get_token_end(char *command, int start)
{
	int	i;

	i = start;
	if (command[i] == '"' || command[i] == '\'')
	{
		i = handle_quotes(command, i);
		if (i == -1)
			return (-1);
		return (i);
	}
	if (command[i] == '|' || command[i] == '<' || command[i] == '>')
	{
		if ((command[i] == '<' && command[i + 1] == '<')
			|| (command[i] == '>' && command[i + 1] == '>'))
			return (i + 2);
		return (i + 1);
	}
	while (command[i] && !isspace(command[i])
		&& !(command[i] == '|' || command[i] == '<' || command[i] == '>'))
		i++;
	return (i);
}

char	*extract_token(char *command, int start, int end)
{
	char	*token;
	int		i;

	token = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (start + i < end)
	{
		token[i] = command[start + i];
		i++;
	}
	token[i] = '\0';
	return (token);
}

char	*get_token_value(char *cmd, int start, int end)
{
	int	is_quoted;

	is_quoted = (cmd[start] == '"' || cmd[start] == '\'');
	if (is_quoted)
		return (extract_token(cmd, start + 1, end - 1));
	return (extract_token(cmd, start, end));
}
