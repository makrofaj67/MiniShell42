/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:09 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/12 12:00:09 by nakbas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/f_builtins.h"

int		mini_strcmp_path(char *path, char *cmd)
{
	int i = 0;
	
	if (path == NULL || cmd == NULL)
		return (0);
	
	while (path[i] != '\0' && cmd[i] != '\0')
	{
		if (path[i] != cmd[i])
			return (0);
		i++;
	}
	if (path[i] == '\0' && cmd[i] == '\0')
		return (1);
	else
		return (0);
}

char	*mini_strcat_p(char *dest, char *src)
{
	char *result_path;
	int i = 0;
	int j = 0;

	result_path = malloc(mini_strlen(dest) + mini_strlen(src));
	if (result_path == NULL)
	return NULL;
	while (dest[i] != '\0')
	{
		result_path[i] = dest[i];
		i++;
	}
	while (src[j] != '\0')
	{
		result_path[i] = src[j];
		i++;
		j++;
	}
	result_path[i] = '\0';
	return result_path;
}

char	*mini_strcat(char *dest, char *src)
{
	char *result_path;
	int i = 0;
	int j = 0;

	result_path = malloc(mini_strlen(dest) + mini_strlen(src) + 2);
	result_path[i] = '\0';
	if (result_path == NULL)
		return NULL;
	while (dest[i] != '\0')
	{
		result_path[i] = dest[i];
		i++;
	}
	result_path[i] = '/';
	i++;
	while (src[j] != '\0')
	{
		result_path[i] = src[j];
		i++;
		j++;
	}
	return result_path;
}

int		mini_strlen(char *s)
{
	int i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

t_env	*mini_lstlast(t_env *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}
