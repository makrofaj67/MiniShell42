/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_token_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:50:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:14:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	process_env_var(t_token_state *state)
{
	char	env_name[256];
	char	*env_value;
	int		k;

	k = 0;
	while ((isalnum(state->token[state->i])
			|| state->token[state->i] == '_') && k < 255)
		env_name[k++] = state->token[state->i++];
	env_name[k] = '\0';
	env_value = getenv(env_name);
	if (env_value)
	{
		k = 0;
		while (env_value[k] && state->j < state->max_len)
			state->result[state->j++] = env_value[k++];
	}
}

static void	handle_exit_status(t_token_state *state)
{
	char	exit_str[12];
	int		k;

	sprintf(exit_str, "%d", state->exit_status);
	k = 0;
	while (exit_str[k] && state->j < state->max_len)
		state->result[state->j++] = exit_str[k++];
	state->i++;
}

/*
** Handles all $ expansions (variables and special parameters)
** Routes to appropriate handler based on what follows the $ character
** Special cases: $? for exit status, alphanumeric for environment variables
** 
** @param state: The token processing state structure
*/
static void	handle_dollar_sign(t_token_state *state)
{
	state->i++;
	if (state->token[state->i] == '?')
		handle_exit_status(state);
	else if (isalnum(state->token[state->i])
		|| state->token[state->i] == '_')
		process_env_var(state);
	else
		state->result[state->j++] = '$';
}

/*
** Main token processing function that handles:
** 1. Quote removal (preserving quoted content)
** 2. Environment variable expansion
** 3. Exit status expansion
** 
** Respects shell quote rules: variables aren't expanded in single quotes
** 
** @param token: The raw token string to process
** @param exit_status: Current exit status for $? expansion
** @return: A newly allocated string with processed token content
*/
char	*process_token(char *token, int exit_status)
{
	t_token_state	state;

	if (!token)
		return (NULL);
	state = init_token_state(token, exit_status);
	if (!state.result)
		return (NULL);
	while (token[state.i] && state.j < state.max_len)
	{
		if (is_quote(token[state.i]))
			handle_quotes(&state);
		else if (token[state.i] == '$' && state.quote_state != '\'')
			handle_dollar_sign(&state);
		else
			state.result[state.j++] = token[state.i++];
	}
	state.result[state.j] = '\0';
	return (state.result);
}
