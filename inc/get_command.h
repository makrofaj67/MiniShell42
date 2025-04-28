/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:13:44 by rakman            #+#    #+#             */
/*   Updated: 2025/04/28 16:15:41 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_COMMAND_H
# define GET_COMMAND_H

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

t_line_list		*init_line(void);
void			add_line_to_line_list(t_line_list *l_list, char *line);
t_line_node		*create_node(char *line);
void			print_line(t_line_list *line);
void			free_line_list(t_line_list *list);

char			*get_command(char *prompt);
void			get_next_lines_if_needed(t_line_list *line_list);
char			*turn_line_list_to_string(t_line_list *line_list);
int				trim_backslash_if_needed(char *str);
void			update_quote_state(const char *str, int *i, char *quote_char);
int				has_unclosed_quotes(const char *str);

#endif
