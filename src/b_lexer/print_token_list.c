/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_token_list.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:17:49 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:17:49 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void	print_token_type(t_t_type type)
{
	if (type == WORD)
		printf("WORD");
	else if (type == PIPE)
		printf("PIPE");
	else if (type == RDRT_IN)
		printf("RDRT_IN");
	else if (type == RDRT_OUT)
		printf("RDRT_OUT");
	else if (type == APPEND)
		printf("APPEND");
	else if (type == HEREDOC)
		printf("HEREDOC");
	else if (type == SEMICOLON)
		printf("SEMICOLON");
	else if (type == AND)
		printf("AND");
	else if (type == OR)
		printf("OR");
	else if (type == PRNT_OPEN)
		printf("PRNT_OPEN");
	else if (type == PRNT_CLOSE)
		printf("PRNT_CLOSE");
}

void	print_token_list(t_t_list *list)
{
	t_t_node	*current;
	int			token_count;

	if (!list || !list->head)
	{
		printf("Token list is empty.\n");
		return ;
	}
	current = list->head;
	token_count = 1;
	printf("\n=== TOKEN LIST ===\n");
	while (current)
	{
		printf("Token %d: [", token_count++);
		print_token_type(current->type);
		printf("] Value: '%s'\n", current->value);
		current = current->next;
	}
	printf("=================\n\n");
}
