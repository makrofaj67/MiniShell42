/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:24:07 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:35:16 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Main input function called from shell_loop in main.c
** Reads a command from the user and ensures it is valid
** Acts like a gatekeeper, only allowing complete commands to pass
** through to the lexer and parser
*/
char	*get_command(char *prompt, int *should_exit)
{
	char	*command;

	command = readline(prompt);
	if (command == NULL)
	{
		*should_exit = 1;
		printf("exit");
		return (NULL);
	}
	else if (is_command_blank(command))
		return (NULL);
	else if (parentheses_status(command) != 0)
	{
		printf("Parenthesis Error\n");
		return (NULL);
	}
	else if (has_unclosed_quotes(command))
	{
		printf("Unclosed quotes\n");
		return (NULL);
	}
	return (command);
}
