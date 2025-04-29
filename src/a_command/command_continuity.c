/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_continuity.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 15:20:00 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 15:20:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	check_parenthesis_error(char *full_command)
{
	if (has_parenthesis_error(full_command, 0))
	{
		printf("Attention: Parentheses error\n");
		free(full_command);
		return (1);
	}
	return (0);
}

static void	process_next_line(t_line_list *line_list)
{
	char	*next_line;

	next_line = readline("\033[38;5;208m--> ");
	if (next_line)
		add_line_to_line_list(line_list, next_line);
}

void	clear_line_nodes(t_line_list *line_list)
{
	t_line_node	*current;
	t_line_node	*next;

	current = line_list->head;
	while (current != NULL)
	{
		next = current->next;
		free(current->line);
		free(current);
		current = next;
	}
	line_list->head = NULL;
	line_list->tail = NULL;
}

void	get_next_lines_if_needed(t_line_list *line_list)
{
	char	*full_command;

	if (line_list == NULL)
		return ;
	while (1)
	{
		full_command = turn_line_list_to_string(line_list);
		if (full_command == NULL)
			break ;
		if (check_parenthesis_error(full_command))
		{
			clear_line_nodes(line_list);
			return ;
		}
		if (!should_read_more_input(full_command, line_list))
		{
			free(full_command);
			break ;
		}
		free(full_command);
		process_next_line(line_list);
		if (line_list->tail == NULL)
			break ;
	}
}
