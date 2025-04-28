/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commander_helpers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 21:59:14 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 21:59:14 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	is_all_white_space(char *command)
{
	int	i;

	i = 0;
	while (command[i])
	{
		if (!(command[i] == '\t' || command[i] == '\b'
				|| command[i] == '\v' || command[i] == '\n'
				|| command[i] == ' '))
			return (0);
		i++;
	}
	return (1);
}

int	calculate_total_length(t_line_list *line)
{
	int			total_len;
	t_line_node	*node;

	total_len = 0;
	node = line->head;
	while (node != NULL)
	{
		total_len += strlen(node->line) + 2;
		node = node->next;
	}
	return (total_len);
}
