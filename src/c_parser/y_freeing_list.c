/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   y_freeing_list.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 15:23:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/07 11:32:01 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	free_cmdval_list_nodes_only(t_cmdval_list *list)
{
	t_cmdval_node	*next_node;
	t_cmdval_node	*current;

	if (list == NULL)
		return ;
	current = list->head;
	while (current != NULL)
	{
		next_node = current->next;
		free(current);
		current = next_node;
	}
	free(list);
}

static void	free_redir_node(t_cmdval_node *node)
{
	t_redirection	*redir;

	redir = (t_redirection *)node->value;
	if (redir->filename)
		free(redir->filename);
	free(redir);
}

void	free_cmdval_list_with_contents(t_cmdval_list *list, int is_redir_list)
{
	t_cmdval_node	*next_node;
	t_cmdval_node	*current;

	if (list == NULL)
		return ;
	current = list->head;
	while (current != NULL)
	{
		next_node = current->next;
		if (current->value)
		{
			if (is_redir_list)
				free_redir_node(current);
			else
				free(current->value);
		}
		free(current);
		current = next_node;
	}
	free(list);
}
