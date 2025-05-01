/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_parenthesis_status.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:41:53 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:23 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Verifies that a closing parenthesis has a matching opening parenthesis
** Detects syntax errors where closing parentheses appear without matching opens
** Uses a stack to track parenthesis pairs
** 
** @param stack: The parenthesis tracking stack
** @param c: The current character (checked if it's a closing parenthesis)
** @return: 1 if an unmatched closing parenthesis is found, 0 otherwise
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
** Validates that all parentheses in a command are properly matched
** Ensures command has syntactically correct parenthesis pairs
** Uses a stack-based algorithm to track opening and closing pairs
** 
** @param command: The command string to check for balanced parentheses
** @return: 0 if all parentheses are matched, positive count of unclosed
**          parentheses, or -1 if there are unmatched closing parentheses
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
