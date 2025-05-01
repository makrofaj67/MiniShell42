/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_token_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 00:24:47 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Check if a character is a shell operator
** Returns 1 if it's an operator, 0 otherwise
*/
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
** Check if a character is whitespace
** Returns 1 if it's whitespace, 0 otherwise
*/
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

/*
** Get the length of an operator token
** Returns the length (1 or 2) of the operator
*/
int	get_operator_len(const char *str)
{
	if (!str || !*str)
		return (0);
	if ((str[0] == '<' && str[1] == '<')
		|| (str[0] == '>' && str[1] == '>'))
		return (2);
	if (is_operator(str[0]))
		return (1);
	return (0);
}

/*
** Determine the token type from the token string
** Returns the corresponding token type enum value
*/
t_token_type	get_token_type(char *token_str)
{
	if (!token_str || !*token_str)
		return (WORD);
	if (strcmp(token_str, "|") == 0)
		return (PIPE);
	else if (strcmp(token_str, "<") == 0)
		return (RDRT_IN);
	else if (strcmp(token_str, ">") == 0)
		return (RDRT_OUT);
	else if (strcmp(token_str, ">>") == 0)
		return (APPEND);
	else if (strcmp(token_str, "<<") == 0)
		return (HEREDOC);
	return (WORD);
}
