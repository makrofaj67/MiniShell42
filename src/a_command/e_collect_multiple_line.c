/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_collect_multiple_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:54:26 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 21:44:42 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	calculate_total_length(t_line_list *line)
{
	int			total_len;
	t_line_node	*node;

	total_len = 0;
	if (!line || !line->head)
		return (0);
	node = line->head;
	while (node != NULL)
	{
		total_len += strlen(node->line);
		if (node->next != NULL)
			total_len += 1;
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

/* Check if command is complete and ready to be processed */
int	process_line_and_check(t_line_list *list, char *next_line, char **result)
{
	char	*full_command;

	add_line_to_line_list(list, strdup(next_line));
	full_command = turn_line_list_to_string(list);
	if (!full_command)
		return (0);
	if (has_unclosed_quotes(full_command)
		|| parentheses_status(full_command) > 0
		|| has_end_backslash(full_command))
	{
		free(full_command);
		return (0);
	}
	*result = full_command;
	return (1);
}

/* Handle line input errors and checks */
int	handle_line_input(char *next_line, t_line_list *list, char *command)
{
	if (is_command_blank(next_line))
	{
		free(next_line);
		return (0);
	}
	else if (parentheses_status(next_line) < 0)
	{
		free(next_line);
		perror_rnull("You can not close what you have not open");
		free_line_list(list);
		return (1);
	}
	return (0);
}

/* Collect multiple lines of input until the command is complete */
char	*collect_multiple_line(char *command, char *prompt)
{
	t_line_list	*list;
	char		*next_line;
	char		*result;

	list = init_line();
	if (!list)
		return (command);
	add_line_to_line_list(list, strdup(command));
	while (1)
	{
		next_line = readline("> ");
		if (!next_line)
			break ;
		if (handle_line_input(next_line, list, command))
			return (command);
		if (process_line_and_check(list, next_line, &result))
		{
			free(command);
			free_line_list(list);
			return (result);
		}
		free(next_line);
	}
	free_line_list(list);
	return (command);
}
