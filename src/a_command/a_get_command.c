/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:24:07 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 21:10:20 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* 
** Validate command for syntax errors
** Returns NULL if command has errors, otherwise returns the command
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
** Main input function called from shell_loop in main.c
** Reads a command from the user and ensures it is valid
** Acts like a gatekeeper, only allowing complete commands to pass
** through to the lexer and parser
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
