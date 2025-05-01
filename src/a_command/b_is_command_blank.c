/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_is_command_blank.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:36:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:23 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Determines if a string consists solely of whitespace characters
** Checks each character against a set of standard whitespace characters
** 
** @param command: The command string to check
** @return: 1 if the string contains only whitespace, 0 otherwise
*/
static int	is_all_white_space(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (!(command[i] == '\t' || command[i] == '\b'
				|| command[i] == '\v' || command[i] == '\n'
				|| command[i] == ' '))
			return (0);
		i++;
	}
	return (1);
}

/*
** Validates if a command string is empty or contains only whitespace
** Prevents processing of meaningless commands, saving system resources
** 
** @param command: The command string to check
** @return: 1 if the command is empty or blank, 0 if it contains content
*/
int	is_command_blank(char *command)
{
	if (command[0] == '\0' || is_all_white_space(command))
		return (1);
	return (0);
}
