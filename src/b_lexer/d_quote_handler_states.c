/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_quote_handler_states.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 19:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/13 19:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	handle_zero_state(t_token_state *state)
{
	if (state->token[state->i] == '\\')
	{
		if (state->token[state->i + 1] != '\0')
		{
			state->result[state->j++] = state->token[state->i + 1];
			state->i += 2;
			return ;
		}
	}
	else if (state->token[state->i] == '\'')
	{
		state->quote_state = 1;
		state->i++;
	}
	else if (state->token[state->i] == '"')
	{
		state->quote_state = 2;
		state->i++;
	}
	else
	{
		state->result[state->j++] = state->token[state->i++];
	}
}

void	handle_single_quote_state(t_token_state *state)
{
	if (state->token[state->i] == '\'')
	{
		state->quote_state = 0;
		state->i++;
	}
	else
	{
		state->result[state->j++] = state->token[state->i++];
	}
}

void	handle_double_quote_state(t_token_state *state)
{
	if (state->token[state->i] == '\\')
	{
		if (state->token[state->i + 1] != '\0')
		{
			if (state->token[state->i + 1] == '$' 
				|| state->token[state->i + 1] == '`' 
				|| state->token[state->i + 1] == '"' 
				|| state->token[state->i + 1] == '\\')
			{
				state->result[state->j++] = state->token[state->i + 1];
				state->i += 2;
				return ;
			}
		}
		state->result[state->j++] = state->token[state->i++];
	}
	else if (state->token[state->i] == '"')
	{
		state->quote_state = 0;
		state->i++;
	}
	else
	{
		state->result[state->j++] = state->token[state->i++];
	}
}
