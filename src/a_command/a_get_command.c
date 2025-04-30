/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:24:07 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 20:24:54 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*get_command(char *prompt)
{
	char	*command;

	command = readline(prompt);
	if (command == NULL)
		perror_exit("exit");
	else if (is_command_blank(command))
		perror_rnull("Expected string, but found nothing");
	else if (parentheses_status(command) < 0)
		perror_rnull("You can not close what you have not open");
	if (parentheses_status(command) > 0
		|| has_unclosed_quotes(command) || has_end_backslash(command))
		command = collect_multiple_line(command, prompt);
	return (command);
}
