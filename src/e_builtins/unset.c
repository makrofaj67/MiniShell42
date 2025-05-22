/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:38 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:38 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	unset_cmd(char *args, t_env **env)
{
    if (args == NULL || env == NULL || *env == NULL)
        return;
    t_env *tmp;
    t_env *new_node;

    tmp = *env;
    if (mini_strcmp_path(tmp->key, args) == 1)
    {
        *env = tmp->next;
        free(tmp);
        return;
    }
    while (tmp != NULL && tmp->next != NULL)
    {
        if (mini_strcmp_path(tmp->next->key, args) == 1)
        {
            new_node = tmp->next;
            tmp->next = new_node->next;
            free(new_node);
            return;
        }
        tmp = tmp->next;
    }
}
