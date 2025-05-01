/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_token_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:30:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:32:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Create a new token list
** Returns a newly allocated empty token list or NULL if allocation fails
*/
t_token_list	*create_token_list(void)
{
	t_token_list	*list;

	list = (t_token_list *)malloc(sizeof(t_token_list));
	if (!list)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	return (list);
}

/*
** Create a new token node with given value and type
** Returns a newly allocated token node or NULL if allocation fails
*/
t_token_node	*create_token(char *value, t_token_type type)
{
	t_token_node	*node;

	if (!value)
		return (NULL);
	node = (t_token_node *)malloc(sizeof(t_token_node));
	if (!node)
		return (NULL);
	node->value = strdup(value);
	if (!node->value)
	{
		free(node);
		return (NULL);
	}
	node->type = type;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

/*
** Add a token to the end of the token list
** Updates the list with the new token
*/
void	add_token(t_token_list *list, char *value)
{
	t_token_node	*new_node;
	t_token_type	type;

	if (!list || !value)
		return ;
	type = get_token_type(value);
	new_node = create_token(value, type);
	if (!new_node)
		return ;
	if (!list->head)
	{
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		list->tail->next = new_node;
		new_node->prev = list->tail;
		list->tail = new_node;
	}
}

/*
** Free the entire token list and its contents
** Properly deallocates all memory used by the token list
*/
void	free_token_list(t_token_list *list)
{
	t_token_node	*current;
	t_token_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
	free(list);
}