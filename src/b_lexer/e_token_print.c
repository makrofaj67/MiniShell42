/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_token_print.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 11:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/01 23:32:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Convert token type to readable string for debug printing
** Returns a string representation of the token type
*/
static char	*get_token_type_str(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	else if (type == PIPE)
		return ("PIPE");
	else if (type == RDRT_IN)
		return ("RDRT_IN");
	else if (type == RDRT_OUT)
		return ("RDRT_OUT");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == HEREDOC)
		return ("HEREDOC");
	return ("UNKNOWN");
}

/*
** Print the token list for debugging
** Displays all tokens in a formatted list
*/
void	print_tokens(t_token_list *list)
{
	t_token_node	*current;
	int				count;

	if (!list || !list->head)
	{
		printf("Token list is empty.\n");
		return ;
	}
	printf("\n=== TOKEN LIST ===\n");
	current = list->head;
	count = 1;
	while (current)
	{
		printf("Token %d: [%s] Value: '%s'\n", 
			count++, 
			get_token_type_str(current->type),
			current->value);
		current = current->next;
	}
	printf("=================\n\n");
}