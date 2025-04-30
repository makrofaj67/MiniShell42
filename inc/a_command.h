/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:13:44 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 21:07:10 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_COMMAND_H
# define A_COMMAND_H

/*
 * Struct and node for multiple line commands
*/
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

/*
 *	Struct and node for parenthesis and quote
 *	operations
*/
typedef struct s_pnode
{
	struct s_pnode *prev;
	struct s_pnode *next;
}	t_pnode;

typedef struct s_pstack
{
	int				size;
	struct s_pnode *head;
	struct s_pnode *tail;
}	t_pstack;


char *get_command(char *prompt);
/*
 * Error control in command string
*/
int is_command_blank(char *command);

char *collect_multiple_line(char *command, char *prompt);

/*
 *list utils for multiple commands
 */

void	print_line(t_line_list *line);
void	free_line_list(t_line_list *list);
void	add_line_to_line_list(t_line_list *l_list, char *line);;
t_line_node	*create_node(char *line);
t_line_list	*init_line(void);

/*
* stack utils for parentheses operations
*/

int parentheses_status(char *command);
void free_pstack(t_pstack *stack);
void pop_pstack(t_pstack *stack);
void push_pstack(t_pstack *stack);
t_pnode *init_psnode(void);
t_pstack *init_pstack(void);

//quotes and slashes

int has_unclosed_quotes(char *command);
int has_end_backslash(char *command);

#endif
