/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:01:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:01:00 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdio.h> // For printf in print_all_vars_sorted
#include <stdlib.h> // For qsort in print_all_vars_sorted
#include <string.h> // For strcmp in print_all_vars_sorted

// Helper function to parse argument for export (e.g., "KEY=value" or "KEY")
static void parse_export_argument(const char *arg, char **key, char **value)
{
	char *equal_sign;

	*key = NULL;
	*value = NULL;
	equal_sign = ft_strchr(arg, '=');
	if (equal_sign != NULL)
	{
		*key = ft_substr(arg, 0, equal_sign - arg);
		*value = ft_strdup(equal_sign + 1);
	}
	else
	{
		*key = ft_strdup(arg);
	}
}

// Function to print all variables, sorted, for 'export' without arguments
void print_all_vars_sorted(t_variable_list *variables)
{
	t_variable_node *current;
	int count = 0;
	t_variable_node **nodes_array;
	int i;

	if (!variables || !variables->head)
		return;

	current = variables->head;
	while (current)
	{
		count++;
		current = current->next;
	}

	nodes_array = (t_variable_node **)malloc(sizeof(t_variable_node *) * count);
	if (!nodes_array)
	{
		perror("malloc error in print_all_vars_sorted");
		return;
	}

	current = variables->head;
	i = 0;
	while (current)
	{
		nodes_array[i++] = current;
		current = current->next;
	}

	// Simple bubble sort for demonstration; consider qsort for larger lists
	for (i = 0; i < count - 1; i++)
	{
		for (int j = 0; j < count - i - 1; j++)
		{
			if (ft_strcmp(nodes_array[j]->key, nodes_array[j + 1]->key) > 0)
			{
				t_variable_node *temp = nodes_array[j];
				nodes_array[j] = nodes_array[j + 1];
				nodes_array[j + 1] = temp;
			}
		}
	}

	for (i = 0; i < count; i++)
	{
		if (nodes_array[i]->is_exported)
		{
			printf("declare -x %s", nodes_array[i]->key);
			if (nodes_array[i]->value)
				printf("=\\"%s\\"", nodes_array[i]->value);
			printf("\\n");
		}
	}
	free(nodes_array);
}

void export_cmd(char **args, t_variable_list *variables)
{
	char *key;
	char *value;
	int i;

	if (!args || !args[0]) // Should not happen if called from selector
		return;

	if (args[1] == NULL)
	{
		print_all_vars_sorted(variables);
		return;
	}

	i = 1;
	while (args[i])
	{
		parse_export_argument(args[i], &key, &value);
		if (key)
		{
			// TODO: Add validation for key (e.g., valid identifier)
			// For now, directly set/update the variable and mark as exported.
			// If value is NULL and key exists, it's just marked as exported.
			// If key does not exist and value is NULL, it's added with NULL value and exported.
			set_variable(variables, key, value, 1); // 1 for is_exported
			free(key);
			if (value)
				free(value);
		}
		i++;
	}
	// Note: exit status for export should be 0 unless an invalid identifier is given.
	// This basic version doesn't implement identifier validation yet.
}
