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

/* 
** Performs syntax validation on the user-entered command
** Checks for blank commands, mismatched parentheses, and unclosed quotes
** Adds valid commands to the command history
** 
** @param command: The command string to validate
** @return: The validated command or NULL if the command contains errors
*/
static char	*validate_command(char *command)
{
	if (is_command_blank(command))
	{
		free(command);
		return (NULL);
	}
	else if (parenthesis_status(command) != 0)
	{
		printf("Parenthesis Error\n");
		free(command);
		return (NULL);
	}
	else if (has_unclosed_quotes(command))
	{
		printf("Unclosed quotes\n");
		free(command);
		return (NULL);
	}
	add_history(command);
	return (command);
}

/*
** Reads a command from the user input and performs initial validation
** Handles EOF (Ctrl+D) by setting the exit flag
** Manages interruption signals (Ctrl+C) through global signal handler
** 
** @param prompt: The prompt string to display to the user
** @param should_exit: Pointer to the shell exit flag, 
** set to 1 if EOF is detected
** @return: A validated command string or NULL on error, EOF, or interruption
*/
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
