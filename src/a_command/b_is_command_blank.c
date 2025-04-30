/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_is_command_blank.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:36:22 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 20:37:26 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

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

int	is_command_blank(char *command)
{
	if (command[0] == '\0' || is_all_white_space(command))
		return (1);
	return (0);
}
