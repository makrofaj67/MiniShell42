/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_has_unclosed_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:51:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:23 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Detects if a command contains any unclosed quote pairs
** Handles both single and double quotes, respecting escape characters
** Follows shell parsing rules where quotes must be properly paired
** 
** @param command: The command string to check for unclosed quotes
** @return: 1 if there are unclosed quotes, 0 if all quotes are properly closed
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
