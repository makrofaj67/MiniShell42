/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:23:31 by rakman            #+#    #+#             */
/*   Updated: 2025/04/26 19:26:10 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

t_token_type	determine_token_type(char *token)
{
	if (strcmp(token, "|") == 0)
		return (PIPE);
	else if (strcmp(token, "<") == 0)
		return (REDIR_IN);
	else if (strcmp(token, ">") == 0)
		return (REDIR_OUT);
	else if (strcmp(token, "<<") == 0)
		return (HEREDOC);
	else if (strcmp(token, ">>") == 0)
		return (APPEND);
	return (WORD);
}

t_token_node	*token_to_node(char *token)
{
	t_token_type	type;
	char			*value;

	type = determine_token_type(token);
	value = strdup(token);
	if (!value)
		return (NULL);
	return (create_token_node(type, value));
}

t_token_list	*create_token_list_from_array(char **tokens)
{
	t_token_list	*list;
	t_token_node	*node;
	int				i;

	list = init_token_list();
	if (!list)
		return (NULL);
	i = 0;
	while (tokens[i])
	{
		node = token_to_node(tokens[i]);
		if (!node)
		{
			free_token_list(list);
			return (NULL);
		}
		add_token(node, list);
		i++;
	}
	return (list);
}
