/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commander.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:06:48 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:12:07 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*finalize_command(t_line_list *line_list)
{
	char	*command_string;

	command_string = turn_line_list_to_string(line_list);
	if (!command_string)
	{
		free_line_list(line_list);
		return (NULL);
	}
	add_history(command_string);
	free_line_list(line_list);
	return (command_string);
}

static int	check_initial_errors(char *current_line, t_line_list *line_list)
{
	if (current_line == NULL)
	{
		free_line_list(line_list);
		exit(1);
	}
	if (current_line[0] == '\0' || is_all_white_space(current_line))
	{
		free(current_line);
		free_line_list(line_list);
		return (1);
	}
	return (0);
}

t_line_list	*initialize_and_get_lines(char *prompt)
{
	char		*current_line;
	t_line_list	*line_list;

	line_list = init_line();
	if (line_list == NULL)
		return (NULL);
	current_line = readline(prompt);
	if (check_initial_errors(current_line, line_list))
		return (NULL);
	add_line_to_line_list(line_list, current_line);
	get_next_lines_if_needed(line_list); // parantez kontrolünü ayıramadım, çünkü loop içerisinde olmalı
	if (line_list->head == NULL)
	{
		free_line_list(line_list);
		return (NULL);
	}
	return (line_list);
}

char	*get_command(char *prompt)
{
	char		*command_string;
	t_line_list	*line_list;

	line_list = initialize_and_get_lines(prompt);
	if (!line_list)
		return (NULL);
	command_string = finalize_command(line_list);
	return (command_string);
}
