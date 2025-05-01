/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_stack_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:42:50 by rakman            #+#    #+#             */
/*   Updated: 2025/04/30 21:38:33 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Used in parentheses_status to track opening and closing parentheses
** Initializes a new empty stack for tracking parentheses
** Works like creating a clean ledger before tracking transactions
*/
t_pstack	*init_pstack(void)
{
	t_pstack	*stack;

	stack = (t_pstack *)malloc(sizeof(t_pstack));
	if (stack == NULL)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
	return (stack);
}

/*
** Used by push_pstack when adding a new element
** Creates a new empty node to represent an opening parenthesis
** Like creating a blank record card ready to be filed
*/
t_pnode	*init_psnode(void)
{
	t_pnode	*node;

	node = (t_pnode *)malloc(sizeof(t_pnode));
	if (node == NULL)
		return (NULL);
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

/*
** Called in parentheses_status when an opening parenthesis is found
** Adds a new node to the stack to track an opening parenthesis
** Works like adding a marker on a trail, to be picked up on the way back
*/
void	push_pstack(t_pstack *stack)
{
	t_pnode	*node;

	node = init_psnode();
	if (stack->head == NULL)
	{
		stack->head = node;
		stack->tail = node;
	}
	else
	{
		stack->head->next = node;
		node->prev = stack->head;
		stack->head = node;
	}
	stack->size++;
}

/*
** Called in check_closing_paren when a closing parenthesis is found
** Removes a node from the stack to balance with a closing parenthesis
** Like crossing off an item from a checklist when it's completed
*/
void	pop_pstack(t_pstack *stack)
{
	t_pnode	*temp;

	if (!stack || !stack->head)
		return ;
	temp = stack->head;
	if (stack->head == stack->tail)
	{
		stack->head = NULL;
		stack->tail = NULL;
	}
	else if (stack->head->prev)
	{
		stack->head = stack->head->prev;
		stack->head->next = NULL;
	}
	free(temp);
	stack->size--;
}

/*
** Called in parentheses_status when finished analyzing parentheses
** Cleans up all allocated memory for the parentheses tracking stack
** Works like a janitor cleaning up after a job is completed
*/
void	free_pstack(t_pstack *stack)
{
	t_pnode	*current;
	t_pnode	*temp;

	if (!stack)
		return ;
	current = stack->head;
	while (current)
	{
		temp = current;
		current = current->prev;
		free(temp);
	}
	free(stack);
}
