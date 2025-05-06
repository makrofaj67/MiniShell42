/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:26:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 17:03:05 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>

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
	{
		fprintf(stderr, "Error creating sub token list\n");
		free_token_list(left_tokens);
		free_token_list(right_tokens);
		return (NULL);
	}
	left_child = parse_tokens(left_tokens);
	right_child = parse_tokens(right_tokens);
	if (left_tokens == NULL || right_tokens == NULL)
	{		
		fprintf(stderr, "Error parsing sub-commands for pipe\n");
		free_token_list(left_tokens);
		free_token_list(right_tokens);
		return (NULL);
	}
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
        if (pipe_node == NULL) {
            fprintf(stderr, "Error creating pipe node\n");
            free_ast(childs[0]);
            free_ast(childs[1]);
            return NULL;
        }
        return pipe_node;
    }
    else
    {
        command_value *cmd_details = parse_simple_command(tokens);
        if (cmd_details == NULL) {
            return NULL;
        }
        ast_node *command_node = create_command_node(cmd_details);
        if (command_node == NULL) {
             fprintf(stderr, "Error creating command node\n");
             free_command_value(cmd_details);
             return NULL;
        }
        return command_node;
    }
}

