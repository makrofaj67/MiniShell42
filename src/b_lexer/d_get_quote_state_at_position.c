/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1d_get_quote_state.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:19:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:19:40 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	xhandle_zero(char *command, int *index, int *state)
{
	if (command[*index] == '\\')
	{
		if (command[*index + 1] != '\0')
			(*index)++;
	}
	else
	{
		if (command[*index] == '\'')
		{
			*state = 1;
		}
		else if (command[*index] == '"')
		{
			*state = 2;
		}
	}
}

void	xhandle_one(char *command, int *index, int *state)
{
	if (command[*index] == '\'')
	{
		*state = 0;
	}
}

void	xhandle_two(char *command, int *index, int *state)
{
	if (command[*index] == '\\')
	{
		if (command[*index + 1] != '\0')
		{
			if (command[*index + 1] == '$' || command[*index + 1] == '`'
				|| command[*index + 1] == '"' || command[*index + 1] == '\\')
			{
				(*index)++;
			}
		}
	}
	else if (command[*index] == '"')
	{
		*state = 0;
	}
}

int	get_quote_state_at_position(char *command, int pos)
{
	int		i;
	int		state;

	i = 0;
	state = 0;
	if (command == NULL)
		return (0);
	while (i < pos && command[i])
	{
		if (state == 0)
			xhandle_zero(command, &i, &state);
		else if (state == 1)
			xhandle_one(command, &i, &state);
		else if (state == 2)
			xhandle_two(command, &i, &state);
		i++;
	}
	return (state);
}
