/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_parse_cmd_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#             */
/*   Updated: 2025/05/07 17:05:10 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_cmdval_list	*init_cmdval_list(void)
{
	t_cmdval_list	*list;

	list = (t_cmdval_list *)malloc(sizeof(t_cmdval_list));
	if (list == NULL)
		return (NULL);
	list->head = NULL;
	list->tail = NULL;
	list->size = 0;
	return (list);
}


int	add_node_to_cmdval_list(t_cmdval_list *list, void *value)
{
	t_cmdval_node	*new_node;

	new_node = (t_cmdval_node *)malloc(sizeof(t_cmdval_node));
	if (new_node == NULL)
		return (0);
	new_node->value = value;
	new_node->next = NULL;
	if (list->tail == NULL)
	{
		new_node->prev = NULL;
		list->head = new_node;
		list->tail = new_node;
	}
	else
	{
		new_node->prev = list->tail;
		list->tail->next = new_node;
		list->tail = new_node;
	}
	list->size++;
	return (1);
}

void	add_word(t_token_node *node, t_cmdval_list *list)
{
	char	*word_copy;
	int		success;

	word_copy = strdup(node->value);
	if (word_copy == NULL)
	{
		perror("strdup failed in add_word");
		return ;
	}
	success = add_node_to_cmdval_list(list, word_copy);
	if (success == 0)
	{
		free(word_copy);
		perror("add_node_to_cmdval_list failed in add_word");
	}
}
