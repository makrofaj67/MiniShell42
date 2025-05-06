/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:26:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 17:17:58 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void *free_tokens_wrapper(t_token_list *left_tokens, t_token_list *right_tokens, char *message)
{
	free_token_list(left_tokens);
	free_token_list(right_tokens);
	write(2, message, strlen(message));
	return (NULL);
}

void *free_ast_node_wrapper(ast_node *left_node, ast_node *right_node, char *message)
{
	free_ast(left_node);
	free_ast(right_node);
	write(2, message, strlen(message));
	return (NULL);
}

ast_node **parse_simple_node(t_token_list *tokens, t_token_node *pipe_location)
{
	ast_node	**childs;
	t_token_list *left_tokens;
	t_token_list *right_tokens;
	ast_node *left_child;
	ast_node *right_child;

	childs = (ast_node **)malloc(sizeof(ast_node*) * 2);
	if (childs == NULL)
		return (NULL);
	left_tokens = get_tokens_before(tokens, pipe_location);
	right_tokens = get_tokens_after(tokens, pipe_location);
	if (left_tokens == NULL || right_tokens == NULL)
		return (free_tokens_wrapper(left_tokens, right_tokens,
					"Error creating sub token list\n"));
	left_child = parse_tokens(left_tokens);
	right_child = parse_tokens(right_tokens);
	if (left_tokens == NULL || right_tokens == NULL)
		return (free_tokens_wrapper(left_tokens, right_tokens,
					"Error parsing sub-commands for pipe\n"));
	childs[0] = left_child;
	childs[1] = right_child;
	return (childs);
}


ast_node *parse_tokens(t_token_list *tokens) 
{
	t_token_node *pipe_location;
	ast_node **childs;

	if (tokens == NULL || tokens->head == NULL)
	    return (NULL);
	pipe_location = find_last_pipe(tokens);
    if (pipe_location != NULL)
    {
		childs = parse_simple_node(tokens, pipe_location);	
        ast_node *pipe_node = create_pipe_node(childs[0], childs[1]);
        if (pipe_node == NULL) 
			return (free_ast_node_wrapper(childs[0], childs[1], "Error creating pipe node\n"));
        return pipe_node;
    }
    else
    {
        command_value *cmd_details = parse_simple_command(tokens);
        if (cmd_details == NULL) {
            return NULL;
        }
        ast_node *command_node = create_command_node(cmd_details);
        if (command_node == NULL)
			return (free_ast_node_wrapper(command_node, NULL, "Error creating command node\n"));
        return command_node;
    }
}

