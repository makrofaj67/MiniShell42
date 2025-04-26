/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:23:28 by rakman            #+#    #+#             */
/*   Updated: 2025/04/26 18:39:31 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

/**
 * Token listesi başlatma
 */
t_token_list	*init_token_list(void)
{
	t_token_list	*list;

	list = (t_token_list *)malloc(sizeof(t_token_list));
	if (list == NULL)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}

/**
 * Yeni token düğümü oluşturma
 */
t_token_node	*create_token_node(t_token_type type, char *value)
{
	t_token_node	*node;

	node = (t_token_node *)malloc(sizeof(t_token_node));
	if (node == NULL)
		return (NULL);
	node->type = type;
	node->value = value;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

/**
 * Token düğümünü listeye ekleme
 */
void	add_token(t_token_node *node, t_token_list *list)
{
	if (list->size == 0)
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
	list->size++;
}

/**
 * Token tipini string'e dönüştürme
 */
const char	*token_type_to_string(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == REDIR_IN)
		return ("REDIR_IN");
	else if (type == REDIR_OUT)
		return ("REDIR_OUT");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == HEREDOC)
		return ("HEREDOC");
	else if (type == ERROR)
		return ("ERROR");
	else
		return ("UNKNOWN");
}
