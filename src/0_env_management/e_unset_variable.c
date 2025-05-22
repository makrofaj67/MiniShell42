/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_unset_variable.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:10:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/22 20:10:24 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static void list_remove_and_free_node(t_variable_list *list, t_variable_node *node_to_remove)
{
    if (!list || !node_to_remove)
        return;

    if (node_to_remove == list->head)
        list->head = node_to_remove->next;
    if (node_to_remove == list->tail)
        list->tail = node_to_remove->prev;
    if (node_to_remove->prev)
        node_to_remove->prev->next = node_to_remove->next;
    if (node_to_remove->next)
        node_to_remove->next->prev = node_to_remove->prev;
    free(node_to_remove->key);
    free(node_to_remove->value);
    free(node_to_remove);
}

void unset_variable(t_variable_list *list, char *key)
{
    t_variable_node *node_to_unset;

    if (!list || !key)
        return;
    node_to_unset = find_variable_node(list, key);
    if (node_to_unset)
        list_remove_and_free_node(list, node_to_unset);
}
