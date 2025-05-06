/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_split_tokens_by_pipe.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/06 16:00:16 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 18:16:33 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_token_node	*find_last_pipe(t_token_list *tokens)
{
	t_token_node	*current;

	if (tokens == NULL || tokens->head == NULL || tokens->tail == NULL)
		return (NULL);
	current = tokens->tail;
	while (current != NULL)
	{
		if (current->type == PIPE)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

t_token_list	*get_tokens_after(t_token_list *tokens,
				t_token_node *pipe_location)
{
	t_token_list	*new_list;
	t_token_node	*current_original;

	new_list = create_token_list();
	if (new_list == NULL)
	{
		perror("Failed to create new token list in get_tokens_before");
		return (NULL);
	}
	current_original = pipe_location->next;
	while (current_original != NULL && current_original != pipe_location)
	{
		add_token(new_list, current_original->value);
		current_original = current_original->next;
	}
	return (new_list);
}

t_token_list	*get_tokens_before(t_token_list *tokens,
					t_token_node *pipe_location)
{
	t_token_list	*new_list;
	t_token_node	*current_original;

	new_list = create_token_list();
	if (new_list == NULL)
	{
		perror("Failed to create new token list in get_tokens_before");
		return (NULL);
	}
	current_original = tokens->head;
	while (current_original != NULL && current_original != pipe_location)
	{
		add_token(new_list, current_original->value);
		current_original = current_original->next;
	}
	return (new_list);
}
