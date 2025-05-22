/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   f_free_list.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:10:34 by rakman            #+#    #+#             */
/*   Updated: 2025/05/22 20:10:45 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void free_variable_node(t_variable_node *node)
{
    if (!node)
        return;
    free(node->key);   
    node->key = NULL;  
    free(node->value); 
    node->value = NULL; 
    free(node);        
}

void free_variable_list(t_variable_list *list)
{
    t_variable_node *current;
    t_variable_node *next_node;

    if (!list)
        return;

    current = list->head;
    while (current != NULL)
    {
        next_node = current->next;
        free_variable_node(current); 
        current = next_node;
    }
    list->head = NULL; 
    list->tail = NULL; 

    free(list); 
}
