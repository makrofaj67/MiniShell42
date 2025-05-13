/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:01:00 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:35:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	add_env_var(char *key, char *value, t_env **env)
{
	int	i;

	if (!key || !env)
		return;
		
	// Tahsis edilecek bellek için uzunlukları hesapla
	i = 0;
	while (key && key[i])
		i++;
	(*env)->key = malloc((i + 1) * sizeof(char));
	if (!(*env)->key)
		exit(1);
		
	i = 0;
	while (value && value[i])
		i++;
	(*env)->value = malloc((i + 1) * sizeof(char));
	if (!(*env)->value)
		exit(1);
	
	// Key kopyalama
	i = 0;
	while (key && key[i])
	{
		(*env)->key[i] = key[i];
		i++;
	}
	(*env)->key[i] = '\0';
	
	// Value kopyalama
	i = 0;
	while (value && value[i])
	{
		(*env)->value[i] = value[i];
		i++;
	}
	(*env)->value[i] = '\0';
	(*env)->next = NULL;
}

int	export_cmd(char *arg, t_env **env, t_env **env_var)
{
	t_env	*tmp;
	t_env   *new_node;
	int     found;

	if (!arg || !env || !env_var)
		return (1);
		
	new_node = NULL;
	tmp = *env_var;
	found = 0;
	
	// Değişkenin env_var listesinde var olup olmadığını kontrol et
	while (tmp != NULL)
	{
		if (mini_strcmp_path(tmp->key, arg) == 1)
		{
        	t_env *last = mini_lstlast(*env);
        	new_node = malloc(sizeof(t_env));
        	if (!new_node)
        		return (1);
        		
        	add_env_var(tmp->key, tmp->value, &new_node);
        	last->next = new_node;
			found = 1;
			break;
		}
		tmp = tmp->next;
	}
	
	// Değişken bulunamadıysa hata geri dön
	if (!found)
	{
		printf("export: '%s': not found\n", arg);
		return (1);
	}
	
	return (0);
}
