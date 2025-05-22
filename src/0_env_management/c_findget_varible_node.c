/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   c_findget_varible_node.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 20:10:08 by rakman            #+#    #+#             */
/*   Updated: 2025/05/22 20:10:24 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_variable_node *find_variable_node(t_variable_list *list, char *key)
{
    t_variable_node *current;

    if (!list || !key)
        return (NULL);
    current = list->head;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

char *get_variable_value(t_variable_list *list, char *key)
{
    t_variable_node *node;
    char            *value_copy;

    if (!list || !key)
        return (NULL);
    node = find_variable_node(list, key);
    if (node)
    {
        if (node->value)
        {
            value_copy = ft_strdup(node->value);
            return (value_copy);
        }
        else
            return (NULL);
    }
    return (NULL);
}
