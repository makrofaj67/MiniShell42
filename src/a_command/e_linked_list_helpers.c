/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_helpers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 12:02:16 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 11:15:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h>

t_line_list	*init_line(void)
{
	t_line_list	*line;

	line = (t_line_list *)malloc(sizeof(t_line_list));
	if (line == NULL)
		return (NULL);
	line->head = NULL;
	line->tail = NULL;
	return (line);
}

t_line_node	*create_node(char *line)
{
	t_line_node	*node;

	node = (t_line_node *)malloc(sizeof(t_line_node));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	node->line = line;
	return (node);
}

void	add_line_to_line_list(t_line_list *l_list, char *line)
{
	t_line_node		*node_line;

	node_line = create_node(line);
	if (l_list->head == NULL)
	{
		l_list->head = node_line;
		l_list->tail = node_line;
	}
	else
	{
		l_list->tail->next = node_line;
		node_line->prev = l_list->tail;
		l_list->tail = node_line;
	}
}

void	print_line(t_line_list *line)
{
	t_line_node	*node;

	if (!line || !line->head)
		return ;
	node = line->head;
	printf("\n");
	while (node != NULL)
	{
		printf("%s", node->line);
		if (node->next != NULL)
			printf("\n");
		node = node->next;
	}
	printf("\n");
}

void	free_line_list(t_line_list *list)
{
	t_line_node	*current;
	t_line_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->line);
		free(current);
		current = next;
	}
	free(list);
}
