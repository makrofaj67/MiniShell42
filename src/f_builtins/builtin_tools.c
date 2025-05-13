/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:09 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:15:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int		mini_strcmp_path(const char *s1, const char *s2)
{
	int i = 0;
	
	if (s1 == NULL || s2 == NULL)
		return (0);
	
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	if (s1[i] == '\0' && s2[i] == '\0')
		return (1);
	else
		return (0);
}

char	*mini_strcat_p(char *dest, const char *src_path_segment, const char *src_command)
{
	char *result_path;
	int i = 0;
	int j = 0;

	result_path = malloc(mini_strlen(dest) + mini_strlen(src_path_segment) + mini_strlen(src_command) + 2);
	if (result_path == NULL)
		return NULL;
		
	// Temel yol kopyalanıyor
	while (dest[i] != '\0')
	{
		result_path[i] = dest[i];
		i++;
	}
	
	// Path segmenti kopyalanıyor (gerekirse)
	if (src_path_segment && *src_path_segment)
	{
		if (result_path[i-1] != '/')
			result_path[i++] = '/';
			
		j = 0;
		while (src_path_segment[j] != '\0')
		{
			result_path[i] = src_path_segment[j];
			i++;
			j++;
		}
	}
	
	// Komut adı kopyalanıyor
	if (result_path[i-1] != '/')
		result_path[i++] = '/';
		
	j = 0;
	while (src_command[j] != '\0')
	{
		result_path[i] = src_command[j];
		i++;
		j++;
	}
	
	result_path[i] = '\0';
	return result_path;
}

char	*mini_strcat(char *dest, const char *src)
{
	char *result;
	int i = 0;
	int j = 0;

	result = malloc(mini_strlen(dest) + mini_strlen(src) + 1);
	if (result == NULL)
		return NULL;
		
	while (dest[i] != '\0')
	{
		result[i] = dest[i];
		i++;
	}
	
	while (src[j] != '\0')
	{
		result[i] = src[j];
		i++;
		j++;
	}
	
	result[i] = '\0';
	return result;
}

size_t	mini_strlen(const char *s)
{
	size_t i;

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
