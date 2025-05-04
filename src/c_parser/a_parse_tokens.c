/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 15:26:22 by rakman            #+#    #+#             */
/*   Updated: 2025/05/04 15:26:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

ast_node *parse_tokens(t_token_list *tokens) {

	if (tokens == NULL || tokens->head == NULL)
	    return NULL;

	t_token_node *pipe_location = find_last_pipe(tokens);
    if (pipe_location != NULL)
    {
        t_token_list *left_tokens = get_tokens_before(tokens, pipe_location);
        t_token_list *right_tokens = get_tokens_after(tokens, pipe_location);
        if (left_tokens == NULL || right_tokens == NULL) {
             fprintf(stderr, "Error creating sub token lists\n");
             free_token_list(left_tokens);
             free_token_list(right_tokens);
             return NULL;
        }
        ast_node *left_child = parse_tokens(left_tokens);
        ast_node *right_child = parse_tokens(right_tokens);
        free_token_list(left_tokens);
        free_token_list(right_tokens);
        if (left_child == NULL || right_child == NULL) {
             fprintf(stderr, "Error parsing sub-commands for pipe\n");
             free_ast(left_child);
             free_ast(right_child);
             return NULL;
        }
        ast_node *pipe_node = create_pipe_node(left_child, right_child);
        if (pipe_node == NULL) {
            fprintf(stderr, "Error creating pipe node\n");
            free_ast(left_child);
            free_ast(right_child);
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

