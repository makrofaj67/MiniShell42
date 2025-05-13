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

int	is_operator(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v');
}

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
