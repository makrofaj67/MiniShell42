/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:26:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 18:48:25 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

ast_node	*parse_tokens(t_token_list *tokens)
{
	t_token_node	*pipe_location;
	ast_node		*pipe_node;
	ast_node		**childs;
	command_value	*cmd_details;
	ast_node		*command_node;

	if (tokens == NULL || tokens->head == NULL)
		return (NULL);
	pipe_location = find_last_pipe(tokens);
	if (pipe_location != NULL)
	{
		childs = parse_simple_pipe(tokens, pipe_location);
		if (childs == NULL)
			return (NULL);
		pipe_node = create_pipe_node(childs[0], childs[1]);
		free(childs);
		return (pipe_node);
	}
	cmd_details = parse_simple_command(tokens);
	if (cmd_details == NULL)
		return (NULL);
	command_node = create_command_node(cmd_details);
	return (command_node);
}
