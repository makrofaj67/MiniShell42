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

static char	*validate_command(char *command)
{
	if (is_command_blank(command))
	{
		free(command);
		return (NULL);
	}
	else if (parenthesis_status(command) == -1)
	{
		printf("Unopened parenthesis error --> ')'\n");
		free(command);
		return (NULL);
	}
	else if (parenthesis_status(command) == 1)
	{
		printf("Unclosed parenthesis error --> '('\n");
		free(command);
		return(NULL);
	}
	else if (has_unclosed_quotes(command) == 1)
	{
		printf("There is unclosed single quote on your command --> '\n");
		free(command);
		return (NULL);
	}
	else if (has_unclosed_quotes(command) == 2)
	{
		printf("There is unclosed double quote on your command --> '\n");
		free(command);
		return (NULL);
	}
	add_history(command);
	return (command);
}

char	*get_command(char *prompt, int *should_exit)
{
	char	*command;

	if (g_signal_received == SIGINT)
	{
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
