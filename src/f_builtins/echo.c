/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 11:47:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:25:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*env_value(const char *name, t_env *env_list)
{
	t_env *tmp;

	if (!name || !env_list)
		return (NULL);

	tmp = env_list;
	while (tmp != NULL)
	{
		if (mini_strcmp_path(name, tmp->key))
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

int	echo_cmd(char **args)
{
	int	i;
	int n_flag;

	if (!args || !args[0])
		return (1);

	i = 1;
	n_flag = 0;

	// "-n" bayrağını kontrol et
	if (args[i] && mini_strcmp_path(args[i], "-n") == 1)
	{
		n_flag = 1;
		i++;
	}

	// Argümanları yazdır
	while (args[i])
	{
		printf("%s", args[i]);
		
		// Son argüman değilse boşluk ekle
		if (args[i + 1])
			printf(" ");
		
		i++;
	}

	// -n bayrağı yoksa yeni satır ekle
	if (!n_flag)
		printf("\n");

	return (0);
}
