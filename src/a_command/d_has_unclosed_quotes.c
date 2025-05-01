/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_has_unclosed_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:51:36 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 23:35:01 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Used in get_command to detect incomplete commands with quote issues
** Checks if a command has any unclosed quotes (both single and double)
** Works like a doorman keeping track of who entered and left the building,
** ensuring that every opening quote has a matching closing quote
*/
int	has_unclosed_quotes(char *command)
{
	int	i;
	int	in_single_quote;
	int	in_double_quote;
	int	escaped;

	if (!command)
		return (0);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	escaped = 0;
	while (command[i])
	{
		if (command[i] == '\\' && !escaped && !in_single_quote)
			escaped = 1;
		else if (command[i] == '\'' && !escaped && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (command[i] == '"' && !escaped && !in_single_quote)
			in_double_quote = !in_double_quote;
		else
			escaped = 0;
		i++;
	}
	return (in_single_quote || in_double_quote);
}
