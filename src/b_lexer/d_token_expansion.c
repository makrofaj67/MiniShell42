/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_token_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/18 15:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Processes and expands a token without quotes (state 0).
** In this state, all variables are expanded.
**
** @param token: The token to be processed
** @return: The processed token with all variables expanded
*/
char	*process_and_expand_for_zero(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For zero state (no quotes), expand all variables
	expanded = expand_token_variables(token, 0);
	free(token);
	return (expanded);
}

/*
** Processes a token within single quotes (state 1).
** In this state, no variable expansion occurs - literal values are preserved.
**
** @param token: The token to be processed
** @return: The processed token with literal values preserved
*/
char	*process_and_expand_for_single(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For single quote state, don't expand variables
	expanded = expand_token_variables(token, 1);
	free(token);
	return (expanded);
}

/*
** Processes and expands a token within double quotes (state 2).
** In this state, variables are expanded but some escape sequences are processed.
**
** @param token: The token to be processed
** @return: The processed token with variables expanded and escapes processed
*/
char	*process_and_expand_for_double(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For double quote state, expand variables but handle escape sequences
	expanded = expand_token_variables(token, 2);
	free(token);
	return (expanded);
}


