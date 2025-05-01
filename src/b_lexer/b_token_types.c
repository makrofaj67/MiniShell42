/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_token_types.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:15:01 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Identifies shell special operator characters (|, <, >)
** Used during tokenization to distinguish operators from regular words
** 
** @param c: The character to check
** @return: 1 if character is a shell operator, 0 otherwise
*/
int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

/*
** Detects whitespace characters in the command string
** Used to separate tokens and ignore spaces in the command
** 
** @param c: The character to check
** @return: 1 if character is whitespace, 0 otherwise
*/
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

/*
** Calculates the length of shell operators including compound operators
** Handles both single-char operators (|,<,>) and double-char operators (<<,>>)
** 
** @param str: Pointer to the operator in the command string
** @return: 2 for compound operators (<<,>>),
** 1 for simple operators, 0 if not an operator
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
** Maps token strings to their corresponding token type enumerations
** Used after tokenization to classify each token for parsing
** 
** @param token_str: The token string to classify
** @return: Appropriate token type enum value 
** (PIPE, RDRT_IN, etc. or WORD by default)
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
