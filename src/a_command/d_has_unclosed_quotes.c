/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_has_unclosed_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:51:36 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 21:17:17 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	process_double_quote(t_pstack *stack, int *inside, int i, char *cmd)
{
	if (cmd[i] == '"')
	{
		if (*inside)
		{
			pop_pstack(stack);
			*inside = 0;
		}
		else
		{
			push_pstack(stack);
			*inside = 1;
		}
	}
}

/*
** Check double quote status in command
*/
static int	double_quote_status(char *command)
{
	int			i;
	t_pstack	*stack;
	int			result;
	int			inside;

	i = 0;
	inside = 0;
	stack = init_pstack();
	if (!stack)
		return (-1);
	while (command[i])
	{
		process_double_quote(stack, &inside, i, command);
		i++;
	}
	result = stack->size;
	free_pstack(stack);
	return (result);
}

/*
** Process single quote for tracking quote status
*/
static void	process_single_quotes(t_pstack *stack,
			int *inside, int i, char *cmd)
{
	if (cmd[i] == '\'')
	{
		if (*inside)
		{
			pop_pstack(stack);
			*inside = 0;
		}
		else
		{
			push_pstack(stack);
			*inside = 1;
		}
	}
}

/*
** Check single quote status in command
*/
static int	single_quote_status(char *command)
{
	int			i;
	t_pstack	*stack;
	int			result;
	int			inside;

	i = 0;
	inside = 0;
	stack = init_pstack();
	if (!stack)
		return (-1);
	while (command[i])
	{
		process_single_quotes(stack, &inside, i, command);
		i++;
	}
	result = stack->size;
	free_pstack(stack);
	return (result);
}

/*
** Check if command has any unclosed quotes
*/
int	has_unclosed_quotes(char *command)
{
	return (single_quote_status(command) > 0
		|| double_quote_status(command) > 0);
}
