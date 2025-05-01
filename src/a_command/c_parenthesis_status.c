/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parenthesis_status.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:41:53 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 00:52:47 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Used in parentheses_status to handle closing parentheses
** Detects unmatched closing parentheses and updates the stack
** Works like a bouncer who checks if someone is 
** leaving a party they never entered
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
** Used in get_command to validate parentheses balance
** Counts unclosed parentheses using a stack data structure
** Works like a bookkeeper tracking opening and closing symbols
*/
int	parenthesis_status(char *command)
{
	int			i;
	t_pstack	*stack;
	int			result;

	i = 0;
	stack = init_pstack();
	if (stack == NULL)
	{
		exit(EXIT_SUCCESS);
	}
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
