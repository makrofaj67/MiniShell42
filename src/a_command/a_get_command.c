/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:24:07 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static char	*validate_parentheses(char *command)
{
	if (parenthesis_status(command) == -1)
	{
		printf("Unopened parenthesis error --> ')'\n");
		free(command);
		return (NULL);
	}
	else if (parenthesis_status(command) == 1)
	{
		printf("Unclosed parenthesis error --> '('\n");
		free(command);
		return (NULL);
	}
	return (command);
}

static char	*validate_quotes(char *command)
{
	if (quote_state(command) == 1)
	{
		printf("There is unclosed single quote on your command --> '\n");
		free(command);
		return (NULL);
	}
	else if (quote_state(command) == 2)
	{
		printf("There is unclosed double quote on your command --> \"\n");
		free(command);
		return (NULL);
	}
	return (command);
}

static char	*validate_command(char *command)
{
	if (is_command_blank(command))
	{
		free(command);
		return (NULL);
	}
	command = validate_parentheses(command);
	if (!command)
		return (NULL);
	command = validate_quotes(command);
	if (!command)
		return (NULL);
	return (command);
}

char	*get_command(char *prompt, int *should_exit)
{
	char	*command;

	if (g_signal_received == SIGINT)
	{
		*should_exit = 1;
		reset_signal_flag();
		return (NULL);
	}
	command = readline(prompt);
	if (command == NULL)
	{
		*should_exit = 1;
		printf("exit");
		return (NULL);
	}
	return (validate_command(command));
}
