/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_token_list.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:30:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:14:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Initializes a new doubly-linked token list data structure
** Used as the main container for all tokens during lexical analysis
** 
** @return: A newly allocated empty token list or NULL if allocation fails
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
** Creates a new token node in memory with specified value and type
** Makes a deep copy of the token value for memory safety
** 
** @param value: The string value of the token
** @param type: The token's type (WORD, PIPE, etc.)
** @return: A new token node or NULL if allocation fails
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
** Adds a new token to the end of the doubly-linked token list
** Automatically determines token type based on the token value
** Maintains prev/next pointers for bi-directional traversal
** 
** @param list: The token list to add to
** @param value: The string value of the token to add
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
** Deallocates the entire token list and all contained token nodes
** Properly frees all memory to prevent leaks, including token values
** Should be called when the token list is no longer needed
** 
** @param list: The token list to free
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
