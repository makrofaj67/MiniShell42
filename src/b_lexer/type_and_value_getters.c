/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   type_and_value_getters.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:39:02 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:15:21 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_t_type	get_enum_type(char *unproc_command)
{
	if (strcmp(unproc_command, "|") == 0)
		return (PIPE);
	else if (strcmp(unproc_command, "<") == 0)
		return (RDRT_IN);
	else if (strcmp(unproc_command, ">") == 0)
		return (RDRT_OUT);
	else if (strcmp(unproc_command, ">>") == 0)
		return (APPEND);
	else if (strcmp(unproc_command, "<<") == 0)
		return (HEREDOC);
	else if (strcmp(unproc_command, ";") == 0)
		return (SEMICOLON);
	else if (strcmp(unproc_command, "&&") == 0)
		return (AND);
	else if (strcmp(unproc_command, "||") == 0)
		return (OR);
	else if (strcmp(unproc_command, "(") == 0)
		return (PRNT_OPEN);
	else if (strcmp(unproc_command, ")") == 0)
		return (PRNT_CLOSE);
	else
		return (WORD);
}

char	*get_value(char *unproc_command)
{
	char	*result;

	if (unproc_command == NULL)
		return (NULL);
	result = strdup(unproc_command);
	if (result == NULL)
		return (NULL);
	return (result);
}
