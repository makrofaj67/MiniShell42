/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:38 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:50:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	unset_cmd(char *args, t_env **env)
{
	t_env *tmp;
	t_env *to_delete;

	if (!args || !env || !*env)
		return (1);
	
	// Listenin başındaki öğeyi kontrol et
	if (mini_strcmp_path((*env)->key, args) == 1)
	{
		to_delete = *env;
		*env = (*env)->next;
		free(to_delete->key);
		free(to_delete->value);
		free(to_delete);
		return (0);
	}
	
	// Listedeki diğer öğeleri kontrol et
	tmp = *env;
	while (tmp->next != NULL)
	{
		if (mini_strcmp_path(tmp->next->key, args) == 1)
		{
			to_delete = tmp->next;
			tmp->next = to_delete->next;
			free(to_delete->key);
			free(to_delete->value);
			free(to_delete);
			return (0);
		}
		tmp = tmp->next;
	}
	
	// Değişken bulunamadı
	return (1);
}
