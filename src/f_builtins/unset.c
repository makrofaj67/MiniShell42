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
	t_env *to_delete;

	if (!env || !*env || !args)
		return;
		
	// Listenin başını kontrol et
	if ((*env) && mini_strcmp_path((*env)->key, args) == 1)
	{
		to_delete = *env;
		*env = (*env)->next;
		free(to_delete->key);
		free(to_delete->value);
		free(to_delete);
		return;
	}
	
	tmp = *env;
	while (tmp && tmp->next)
	{
		if (mini_strcmp_path(tmp->next->key, args) == 1)
		{
			to_delete = tmp->next;
			tmp->next = to_delete->next;
			free(to_delete->key);
			free(to_delete->value);
			free(to_delete);
			return;
		}
		tmp = tmp->next;
	}
}
