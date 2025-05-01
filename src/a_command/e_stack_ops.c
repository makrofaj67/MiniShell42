/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_stack_ops.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:42:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:23 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Creates and initializes a new parenthesis tracking stack
** Allocates memory for the stack structure and initializes its properties
** Used for tracking parenthesis balance in command validation
** 
** @return: A pointer to the newly created stack, or NULL if allocation fails
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
** Creates and initializes a new node for the parenthesis stack
** Allocates memory for a node that represents a single parenthesis
** Used by push_pstack to add parenthesis markers to the stack
** 
** @return: A pointer to the newly created node, or NULL if allocation fails
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
** Adds a new node to the parenthesis stack
** Represents tracking an opening parenthesis during command parsing
** Handles proper linking of nodes in the doubly-linked stack
** 
** @param stack: The stack to which the new node should be added
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
** Removes the top node from the parenthesis stack
** Represents matching a closing parenthesis with an opening parenthesis
** Maintains proper stack structure after node removal
** 
** @param stack: The stack from which the top node should be removed
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
** Deallocates all memory used by the parenthesis stack
** Frees each node in the stack and then the stack itself
** Called after parenthesis checking is complete to prevent memory leaks
** 
** @param stack: The stack to be deallocated
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
