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

/*
** Extracts a substring from the command and creates a new token
** 
** @param command: The original command string
** @param start: Starting index of the token in the command
** @param len: Length of the token to extract
** @return: A newly allocated string containing the token or NULL on failure
*/
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

/*
** Processes shell operators like pipes and redirections
** 
** @param command: The original command string
** @param pos: Pointer to the current position in command (updated by function)
** @param start: Starting index of the operator in the command
** @return: A newly allocated string containing the operator token
*/
static char	*process_operator(const char *command, int *pos, int start)
{
	int	len;

	len = get_operator_len(&command[*pos]);
	*pos += len;
	return (extract_token_part(command, start, len));
}

/*
** Processes a word token, handling quotes and advancing position
** Handles complex quoted words by tracking quote state
** 
** @param command: The original command string
** @param pos: Pointer to the current position in command (updated by function)
** @param start: Starting index of the word token in the command
** @return: A newly allocated string containing the word token
*/
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

/*
** Extracts the next token from the command string
** Skips whitespace and determines token type (operator or word)
** 
** @param command: The original command string
** @param pos: Pointer to the current position in command (updated by function)
** @return: A newly allocated string containing the next token or NULL if at end
*/
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

/*
** Main tokenization function that breaks a command into a linked list of tokens
** Processes quotes and environment variables in the tokens
** 
** @param command: The command string to tokenize
** @param exit_status: The current exit status for $? expansion
** @return: A linked list of tokens or NULL if tokenization fails
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
			break ;
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
