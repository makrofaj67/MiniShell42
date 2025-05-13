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

#include "../../inc/f_builtins.h"

void	unset_cmd(char *args, t_env **env)
{
	t_env *tmp;
	t_env *new_node;

	tmp = *env;
	while (tmp != NULL)
	{
		if (mini_strcmp_path(tmp->next->key, args) == 1)
			break;
		else
			return ;
		tmp = tmp->next;
	}
	new_node = tmp->next;
	tmp->next = new_node->next;
	free(new_node);
}
