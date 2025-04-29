/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:13:44 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 13:12:07 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_COMMAND_H
# define A_COMMAND_H

typedef struct s_line_node
{
	char				*line;
	struct s_line_node	*next;
	struct s_line_node	*prev;
}	t_line_node;

typedef struct s_line_list
{
	t_line_node	*head;
	t_line_node	*tail;
}	t_line_list;

/* Initialize and manage line input */
t_line_list		*init_line(void);
void			add_line_to_line_list(t_line_list *l_list, char *line);
t_line_node		*create_node(char *line);
void			print_line(t_line_list *line);
void			free_line_list(t_line_list *list);
int				calculate_total_length(t_line_list *line);
void			clear_line_nodes(t_line_list *line_list);

/* Command processing */
char			*get_command(char *prompt);
int				is_all_white_space(char *command);
void			get_next_lines_if_needed(t_line_list *line_list);
char			*turn_line_list_to_string(t_line_list *line_list);
int				should_read_more_input(char *full_command,
					t_line_list *line_list);
t_line_list		*initialize_and_get_lines(char *prompt);
char			*finalize_command(t_line_list *line_list);

/* Quote and backslash handling */
int				trim_backslash_if_needed(char *str);
int				has_unclosed_quotes(const char *str);
int				has_unclosed_parentheses(const char *str);
int				has_parenthesis_error(const char *str, int silent);

#endif
