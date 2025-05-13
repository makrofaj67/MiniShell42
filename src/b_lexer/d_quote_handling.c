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

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

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
