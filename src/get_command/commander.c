/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commander.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:06:48 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 17:01:55 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

size_t	calculate_total_length(t_line_list *line)
{
	size_t		total_len;
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

char	*turn_line_list_to_string(t_line_list *line)
{
	t_line_node		*node;
	char			*full_command;
	size_t			total_len;

	if (line == NULL || line->head == NULL)
		return (strdup(""));
	total_len = calculate_total_length(line);
	full_command = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!full_command)
		return (NULL);
	full_command[0] = '\0';
	node = line->head;
	while (node != NULL)
	{
		strcat(full_command, node->line);
		if (node->next != NULL)
			strcat(full_command, "\n");
		node = node->next;
	}
	return (full_command);
}

void	get_next_lines_if_needed(t_line_list *line_list)
{
	char	*next_line;
	char	*full_command;

	if (!line_list)
		return ;
	while (true)
	{
		full_command = turn_line_list_to_string(line_list);
		if (full_command == NULL)
			break ;
		if (!trim_backslash_if_needed(line_list->tail->line)
			&& !has_unclosed_quotes(full_command))
		{
			free(full_command);
			break ;
		}
		free(full_command);
		next_line = readline("\033[38;5;208m--->");
		if (!next_line)
			break ;
		add_line_to_line_list(line_list, next_line);
	}
}

char	*get_command(char *prompt)
{
	char		*current_line;
	char		*command_string;
	t_line_list	*line_list;

	line_list = init_line();
	if (!line_list)
		return (NULL);
	current_line = readline(prompt);
	if (current_line == NULL)
	{
		free_line_list(line_list);
		exit(1);
	}
	if (current_line[0] == '\0' || is_all_white_space(current_line))
	{
		free(current_line);
		free_line_list(line_list);
		return (NULL);
	}
	add_line_to_line_list(line_list, current_line);
	get_next_lines_if_needed(line_list);
	command_string = turn_line_list_to_string(line_list);
	free_line_list(line_list);
	return (command_string);
}
