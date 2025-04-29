/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:00:46 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:15:21 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_t_list	*init_token_list(void)
{
	t_t_list	*list;

	list = (t_t_list *)malloc(sizeof(t_t_list));
	if (list == NULL)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	return (list);
}

t_t_node	*create_token_node(char *value, t_t_type type)
{
	t_t_node	*node;

	node = (t_t_node *)malloc(sizeof(t_t_node));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->value = value;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	add_token_to_list(t_t_list *list, char *unprocessed_token)
{
	t_t_node	*node;
	t_t_type	type;
	char		*value;

	type = get_enum_type(unprocessed_token);
	value = get_value(unprocessed_token);
	if (value == NULL)
		return ;
	node = create_token_node(value, type);
	if (node == NULL)
	{
		free(value);
		return ;
	}
	if (list->head == NULL)
	{
		list->head = node;
		list->tail = node;
	}
	else
	{
		list->tail->next = node;
		node->prev = list->tail;
		list->tail = node;
	}
}

void	free_token_list(t_t_list *list)
{
	t_t_node	*current;
	t_t_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}
