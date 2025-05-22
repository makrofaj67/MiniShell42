/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:23 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdio.h> // For printf

// Function to print only exported variables for the 'env' command
void print_exported_vars(t_variable_list *variables)
{
	t_variable_node *current;

	if (!variables)
		return;

	current = variables->head;
	while (current != NULL)
	{
		if (current->is_exported && current->value != NULL) // env only prints if value is not NULL
		{
			printf("%s=%s\n", current->key, current->value);
		}
		current = current->next;
	}
	// env command usually has an exit status of 0 if successful.
	// Error cases (like invalid options, not supported here) would change this.
}