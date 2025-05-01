/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e3_collect_multiple_line.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 22:15:47 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:35:16 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* Handle quoted string continuation */
char	*handle_quote(char *cmd, char *next_line)
{
	char	*result;
	char	*temp;

	temp = ft_strjoin(cmd, "\n");
	if (!temp)
	{
		free(next_line);
		return (cmd);
	}
	result = ft_strjoin(temp, next_line);
	free(temp);
	free(next_line);
	free(cmd);
	if (result == NULL)
		return (cmd);
	return (result);
}

/* Process cmd based on continuation type */
static char	*process_continuation(char *cmd, char *next_line, int cont_char)
{
	if (cont_char == '\\')
		return (process_backslash_cont(cmd, next_line));
	else if (cont_char == '|' || cont_char == '(')
		return (handle_pipe_paren(cmd, next_line));
	return (NULL);
}

/* Combines multi-line input according to shell rules */
char	*combine_cmd_with_newline(char *cmd, char *next_line)
{
	char	*result;
	int		cont_char;

	if (!cmd || !next_line)
		return (cmd);
	if (check_quotes(cmd, strlen(cmd)))
		return (handle_quote(cmd, next_line));
	cont_char = get_continuation_char(cmd);
	if (cont_char == '\\' || cont_char == '|' || cont_char == '(')
		return (process_continuation(cmd, next_line, cont_char));
	else if (is_quote_start(next_line))
		return (handle_quote(cmd, next_line));
	result = ft_strjoin(cmd, next_line);
	free(next_line);
	free(cmd);
	if (result == NULL)
		return (cmd);
	return (result);
}

/* Collects multi-line input until the command is complete */
char	*collect_multiple_line(char *command, char *prompt, int backslash)
{
	char	*next_line;

	(void)prompt;
	(void)backslash;
	while (1)
	{
		next_line = readline("> ");
		if (next_line == NULL)
		{
			free(command);
			return (NULL);
		}
		command = combine_cmd_with_newline(command, next_line);
		if (!command)
			return (NULL);
		if (is_command_complete(command))
			break ;
	}
	return (command);
}
