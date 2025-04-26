/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:05:06 by rakman            #+#    #+#             */
/*   Updated: 2025/04/26 18:58:09 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"

void	free_token_list(t_token_list *list)
{
	t_token_node	*current;
	t_token_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

void	print_tokens(t_token_list *list)
{
	t_token_node	*current;

	printf("Tokens:\n");
	current = list->head;
	while (current)
	{
		printf("Type: %s, Value: '%s'\n",
			token_type_to_string(current->type), current->value);
		current = current->next;
	}
}
t_token_list	*tokenize_command(char *command)
{
	char			**tokens;
	t_token_list	*token_list;

	tokens = extract_tokens(command);
	if (!tokens)
		return (NULL);
	token_list = create_token_list_from_array(tokens);
	free_tokens_array(tokens);
	return (token_list);
}

int	main(int argc, char **argv)
{
	t_token_list	*tokens;

	if (argc != 2)
	{
		printf("Usage: %s \"command\"\n", argv[0]);
		return (1);
	}
	tokens = tokenize_command(argv[1]);
	if (!tokens)
	{
		printf("Error: Failed to tokenize command\n");
		return (1);
	}
	print_tokens(tokens);
	free_token_list(tokens);
	return (0);
}
