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
