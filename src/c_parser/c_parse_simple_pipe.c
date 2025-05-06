/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_parse_simple_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 18:46:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 18:47:02 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	*free_tokens_wrapper(t_token_list *left_tokens,
		t_token_list *right_tokens, ast_node **childs, char *message)
{
	free_token_list(left_tokens);
	free_token_list(right_tokens);
	if (childs != NULL)
		free(childs);
	write(2, message, strlen(message));
	return (NULL);
}

void	*free_ast_node_wrapper(ast_node *left_node,
		ast_node *right_node, char *message)
{
	free_ast(left_node);
	free_ast(right_node);
	write(2, message, strlen(message));
	return (NULL);
}

ast_node	**init_pipe_children(t_token_list *tokens,
		t_token_node *pipe_location, t_token_list **left_tokens,
		t_token_list **right_tokens)
{
	ast_node	**childs;

	childs = (ast_node **)malloc(sizeof(ast_node *) * 2);
	if (childs == NULL)
		return (NULL);
	*left_tokens = get_tokens_before(tokens, pipe_location);
	*right_tokens = get_tokens_after(tokens, pipe_location);
	if (*left_tokens == NULL || *right_tokens == NULL)
	{
		free(childs);
		return (NULL);
	}
	return (childs);
}

ast_node	**parse_simple_pipe(t_token_list *tokens,
				t_token_node *pipe_location)
{
	ast_node		**childs;
	t_token_list	*left_toks;
	t_token_list	*right_toks;
	ast_node		*left_child;
	ast_node		*right_child;

	childs = init_pipe_children(tokens, pipe_location, &left_toks, &right_toks);
	if (childs == NULL)
		return (free_tokens_wrapper(left_toks, right_toks, NULL,
				"Error creating sub token list\n"));
	left_child = parse_tokens(left_toks);
	right_child = parse_tokens(right_toks);
	if (left_child == NULL || right_child == NULL)
	{
		free_ast(left_child);
		free_ast(right_child);
		return (free_tokens_wrapper(left_toks, right_toks, childs,
				"Error parsing sub-commands for pipe\n"));
	}
	childs[0] = left_child;
	childs[1] = right_child;
	free_token_list(left_toks);
	free_token_list(right_toks);
	return (childs);
}
