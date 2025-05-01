/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_quote_handling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:45:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:16:36 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Detects if a character is a quote character (single or double)
** Essential for proper handling of quoted strings in shell commands
** 
** @param c: The character to check
** @return: 1 if the character is a quote, 0 otherwise
*/
int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

/*
** Tracks and manages the current quote state during tokenization
** Changes state when entering/exiting quoted sections based on the character
** 
** @param c: The current character being processed
** @param quote_state: Pointer to the current quote state 
** (0 for none, ' or " for quoted)
** @return: 1 if the quote state changed (quote opened or closed), 0 otherwise
*/
int	update_quote_state(char c, char *quote_state)
{
	if (!*quote_state && is_quote(c))
	{
		*quote_state = c;
		return (1);
	}
	else if (*quote_state == c)
	{
		*quote_state = 0;
		return (1);
	}
	return (0);
}

/*
** Creates and initializes a token processing state structure
** Allocates memory for the result buffer based on token length
** 
** @param token: The token string to process
** @param exit_status: Current exit status for $? expansion
** @return: An initialized t_token_state structure
*/
t_token_state	init_token_state(char *token, int exit_status)
{
	t_token_state	state;

	state.token = token;
	state.max_len = strlen(token) * 2;
	state.result = (char *)malloc(sizeof(char) * (state.max_len + 1));
	state.i = 0;
	state.j = 0;
	state.exit_status = exit_status;
	state.quote_state = 0;
	return (state);
}

/*
** Manages quote characters during token processing
** Handles opening/closing quotes and copying characters inside quotes
** 
** @param state: The token processing state structure
*/
void	handle_quotes(t_token_state *state)
{
	if (state->quote_state == 0)
		state->quote_state = state->token[state->i++];
	else if (state->quote_state == state->token[state->i])
	{
		state->quote_state = 0;
		state->i++;
	}
	else
		state->result[state->j++] = state->token[state->i++];
}
