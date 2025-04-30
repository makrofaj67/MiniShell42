/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_collect_multiple_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:54:26 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 23:26:29 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static char	*combine_cmd_with_newline(char *command, char *next_line)
{
	char	*temp;
	char	*combined;
	char	*result;

	temp = command;
	combined = ft_strjoin(command, "\n");
	if (!combined)
	{
		free(next_line);
		return (temp);
	}
	result = ft_strjoin(combined, next_line);
	free(combined);
	free(next_line);
	if (!result)
		return (temp);
	free(temp);
	return (result);
}

static int	is_command_complete(char *command)
{
	int	paren_status;

	paren_status = parentheses_status(command);
	if (paren_status < 0)
	{
		perror_rnull("You can not close what you have not open");
		return (1);
	}
	if (!(paren_status > 0 || has_unclosed_quotes(command)
			|| has_end_backslash(command)))
		return (1);
	return (0);
}

/* Collect multiple lines of input until the command is complete */
char	*collect_multiple_line(char *command, char *prompt)
{
	char	*next_line;

	while (1)
	{
		next_line = readline("> ");
		if (!next_line)
			break ;
		if (is_command_blank(next_line))
		{
			free(next_line);
			continue ;
		}
		command = combine_cmd_with_newline(command, next_line);
		if (is_command_complete(command))
			break ;
	}
	return (command);
}
