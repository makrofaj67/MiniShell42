/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_tokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:14:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static char	*extract_token_part(const char *command, int start, int len)
{
	char	*token;

	token = (char *)malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	strncpy(token, &command[start], len);
	token[len] = '\0';
	return (token);
}

static char	*process_operator(const char *command, int *pos, int start)
{
	int	len;

	len = get_operator_len(&command[*pos]);
	*pos += len;
	return (extract_token_part(command, start, len));
}

static char	*process_word(const char *command, int *pos, int start)
{
	int		len;
	char	quote_state;

	quote_state = 0;
	while (command[*pos])
	{
		update_quote_state(command[*pos], &quote_state);
		if (!quote_state && (is_whitespace(command[*pos])
				|| is_operator(command[*pos])))
			break ;
		(*pos)++;
	}
	len = *pos - start;
	return (extract_token_part(command, start, len));
}

static char	*extract_token(const char *command, int *pos)
{
	int		start;

	while (command[*pos] && is_whitespace(command[*pos]))
		(*pos)++;
	if (!command[*pos])
		return (NULL);
	start = *pos;
	if (is_operator(command[*pos]))
		return (process_operator(command, pos, start));
	else
		return (process_word(command, pos, start));
}

t_token_list	*tokenize_command(char *command, int exit_status)
{
	t_token_list	*token_list;
	char			*token_str;
	char			*processed_token;
	int				pos;

	if (command == NULL)
		return (NULL);
	token_list = create_token_list();
	if (token_list == NULL)
		return (NULL);
	pos = 0;
	while (1)
	{
		token_str = extract_token(command, &pos);
		if (!token_str)
			break ;
		processed_token = process_token(token_str, exit_status);
		free(token_str);
		if (processed_token)
		{
			add_token(token_list, processed_token);
			free(processed_token);
		}
	}
	print_tokens(token_list);
	return (token_list);
}
