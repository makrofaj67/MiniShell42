/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parentheses_status_backslash.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:41:53 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 21:11:22 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	has_end_backslash(char *command)
{
	int	len;

	if (!command)
		return (0);
	len = strlen(command);
	if (len == 0)
		return (0);
	if (command[len - 1] == '\\')
		return (1);
	return (0);
}

/*
** Check for unmatched closing parenthesis
*/
static int	check_closing_paren(t_pstack *stack, char c)
{
	if (c == ')' && stack->head == NULL)
		return (1);
	if (c == ')')
		pop_pstack(stack);
	return (0);
}

/*
** Count unclosed parentheses in a command
** Returns: >0 (unclosed), 0 (balanced), <0 (unexpected closing)
*/
int	parentheses_status(char *command)
{
	int			i;
	t_pstack	*stack;
	int			result;

	i = 0;
	stack = init_pstack();
	if (stack == NULL)
		perror_exit("Malloc failed");
	while (command[i])
	{
		if (command[i] == '(')
			push_pstack(stack);
		else if (check_closing_paren(stack, command[i]))
		{
			free_pstack(stack);
			return (-1);
		}
		i++;
	}
	result = stack->size;
	free_pstack(stack);
	return (result);
}
