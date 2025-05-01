/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_tokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:32:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Extract a single token from the command string
** Returns the extracted token or NULL if end of command
*/
static char	*extract_token(const char *command, int *pos)
{
	int		start;
	int		len;
	char	quote_state;
	char	*token;

	while (command[*pos] && is_whitespace(command[*pos]))
		(*pos)++;
	if (!command[*pos])
		return (NULL);
	start = *pos;
	quote_state = 0;
	if (is_operator(command[*pos]))
	{
		len = get_operator_len(&command[*pos]);
		*pos += len;
	}
	else
	{
		while (command[*pos])
		{
			update_quote_state(command[*pos], &quote_state);
			if (!quote_state && (is_whitespace(command[*pos]) || 
				is_operator(command[*pos])))
				break;
			(*pos)++;
		}
		len = *pos - start;
	}
	token = (char *)malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	strncpy(token, &command[start], len);
	token[len] = '\0';
	return (token);
}

/*
** Tokenize a command string into a list of tokens
** Returns a list of tokens for the given command
*/
t_token_list	*tokenize_command(char *command, int exit_status)
{
	t_token_list	*token_list;
	char			*token_str;
	char			*processed_token;
	int				pos;

	if (!command)
		return (NULL);
	token_list = create_token_list();
	if (!token_list)
		return (NULL);
	pos = 0;
	while (1)
	{
		token_str = extract_token(command, &pos);
		if (!token_str)
			break;
		processed_token = process_token(token_str, exit_status);
		free(token_str);
		if (processed_token)
		{
			add_token(token_list, processed_token);
			free(processed_token);
		}
	}
	return (token_list);
}