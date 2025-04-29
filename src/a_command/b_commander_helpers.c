/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commander_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 14:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:12:07 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

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

int	should_read_more_input(char *full_command, t_line_list *line_list)
{
	int	backslash;
	int	unclosed_quotes;
	int	unclosed_parentheses;

	if (!full_command)
		return (0);
	backslash = trim_backslash_if_needed(line_list->tail->line);
	unclosed_quotes = has_unclosed_quotes(full_command);
	unclosed_parentheses = has_unclosed_parentheses(full_command);
	return (backslash || unclosed_quotes || unclosed_parentheses);
}

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
