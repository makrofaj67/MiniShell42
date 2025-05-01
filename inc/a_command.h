/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_command.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:13:44 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:34:06 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef A_COMMAND_H
# define A_COMMAND_H

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
char		*get_command(char *prompt, int *should_exit);
int			is_command_blank(char *command);
int			has_unclosed_quotes(char *command);
void		free_pstack(t_pstack *stack);
void		pop_pstack(t_pstack *stack);
void		push_pstack(t_pstack *stack);
t_pstack	*init_pstack(void);
int			parenthesis_status(char *command);

#endif
