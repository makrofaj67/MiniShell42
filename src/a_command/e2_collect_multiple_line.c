/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e2_collect_multiple_line.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 21:30:14 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:35:16 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* Checks if command is syntactically complete */
int	is_command_complete(char *command)
{
	int	paren_status;

	paren_status = parentheses_status(command);
	if (paren_status < 0)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token ')'\n");
		return (1);
	}
	if (paren_status > 0 || has_unclosed_quotes(command)
		|| has_end_backslash(command))
		return (0);
	return (1);
}

/* Handle backslash continuation */
char	*process_backslash_cont(char *cmd, char *next_line)
{
	char	*result;

	trim_trailing_spaces(cmd);
	remove_trailing_backslash(cmd);
	result = ft_strjoin(cmd, next_line);
	free(next_line);
	free(cmd);
	if (result == NULL)
		return (cmd);
	return (result);
}

/* Handle pipe and parenthesis continuation */
char	*handle_pipe_paren(char *cmd, char *next_line)
{
	char	*result;
	char	*temp;

	trim_trailing_spaces(cmd);
	if (next_line[0] != ' ' && next_line[0] != '\t')
	{
		temp = ft_strjoin(cmd, " ");
		if (!temp)
		{
			free(next_line);
			return (cmd);
		}
		result = ft_strjoin(temp, next_line);
		free(temp);
	}
	else
	{
		result = ft_strjoin(cmd, next_line);
	}
	free(next_line);
	free(cmd);
	if (result == NULL)
		return (cmd);
	return (result);
}
