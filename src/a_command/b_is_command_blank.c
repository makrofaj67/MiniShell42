/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_is_command_blank.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:36:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 11:07:53 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Helper function used by is_command_blank
** Checks if a string contains only whitespace characters
** Like a scanner that examines each character looking for any substance
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
** Used in get_command to validate user input
** Checks if a command string contains only whitespace or is empty
** Works like a filter that prevents empty commands from being processed
*/
int	is_command_blank(char *command)
{
	if (command[0] == '\0' || is_all_white_space(command))
		return (1);
	return (0);
}
