/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:13:44 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 20:35:16 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_COMMAND_H
# define A_COMMAND_H

/**
** Struct and node for multiple line commands
** Used to store and manage multi-line input from the user
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

/**
** Struct and node for parenthesis tracking
** Used to maintain a stack of opening parentheses for validation
*/
typedef struct s_pnode
{
	struct s_pnode	*prev;
	struct s_pnode	*next;
}	t_pnode;

typedef struct s_pstack
{
	int				size;
	struct s_pnode	*head;
	struct s_pnode	*tail;
}	t_pstack;

/**
** Main command processing functions
*/
char	*get_command(char *prompt);
int		is_command_blank(char *command);
char	*collect_multiple_line(char *command, char *prompt, int backslash);

/**
** Command syntax validation functions
*/
int		has_unclosed_quotes(char *command);
int		has_end_backslash(char *command);
int		parentheses_status(char *command);

/**
** Line list utility functions for multiple commands
*/
t_line_list	*init_line(void);
t_line_node	*create_node(char *line);
void		add_line_to_line_list(t_line_list *l_list, char *line);
void		print_line(t_line_list *line);
void		free_line_list(t_line_list *list);

/**
** Stack utility functions for parentheses tracking
*/
t_pstack	*init_pstack(void);
t_pnode		*init_psnode(void);
void		push_pstack(t_pstack *stack);
void		pop_pstack(t_pstack *stack);
void		free_pstack(t_pstack *stack);

/**
** Multi-line command processing helper functions 
*/
int		check_quotes(char *str, int pos);
char	*trim_trailing_spaces(char *str);
int		get_continuation_char(const char *str);
int		is_quote_start(const char *str);
char	*remove_trailing_backslash(char *cmd);
int		is_command_complete(char *command);
char	*process_backslash_cont(char *cmd, char *next_line);
char	*handle_pipe_paren(char *cmd, char *next_line);
char	*handle_quote(char *cmd, char *next_line);
char	*combine_cmd_with_newline(char *cmd, char *next_line);

#endif
