/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nakbas <nakbas@stundent.42istanbul.com.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/12 12:00:23 by nakbas            #+#    #+#             */
/*   Updated: 2025/05/13 17:30:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

void	print_nodes(t_env *env_list)
{
    if (env_list == NULL)
    {
        printf("Environment list is empty.\n");
        return;
    }
	
	while (env_list != NULL)
	{
		printf("%s=%s\n", env_list->key, env_list->value);
		env_list = env_list->next;
	}
}

void	env_lengths_2(t_env **len, char *args)
{
	int	i;
	int j;

	i = 0;
	*len = malloc(sizeof(t_env));
	if (*len == NULL)
        exit(1);
		
	// Anahtarın uzunluğunu bul
	while (args[i] != '=' && args[i] != '\0')
		i++;
		
	j = i;
	
	// '=' karakteri varsa, değerin uzunluğunu hesapla
	if (args[i] == '=')
	{
		i++;
		while (args[i] != '\0')
			i++;
		i = i - j - 1; // Değerin uzunluğu
	}
	else
	{
		i = 0; // Değer yok
	}
	
	// Bellek tahsisi
	(*len)->key = malloc((j + 1) * sizeof(char));
	(*len)->value = malloc((i + 1) * sizeof(char));
	if (!(*len)->key || !(*len)->value)
		exit(1);
}

void	add_variable_2(char *args, t_env **env)
{
	int i;
	int j;

	if (!args || !env)
		return;
		
	j = 0;
	i = 0;
	env_lengths_2(env, args);
	
	// Anahtarı kopyala
	while (args[i] != '=' && args[i] != '\0')
	{
		(*env)->key[i] = args[i];
		i++;
	}
	(*env)->key[i] = '\0';
	
	// Değer varsa kopyala
	if (args[i] == '=')
	{
		i++;
		while (args[i] != '\0')
		{
			(*env)->value[j] = args[i];
			i++;
			j++;
		}
	}
	(*env)->value[j] = '\0';
	(*env)->next = NULL;
}

void	create_env(t_env **env)
{
	extern char **environ;
	char	**env_var;
	int 	i;

	i = 0;
	env_var = environ;
	
	// Çevre değişkenlerini işle
	while (env_var[i] != NULL)
	{
		if (*env != NULL)
   		{
       		t_env *last = mini_lstlast(*env);
        	t_env *new_node = NULL;
        	add_variable_2(env_var[i], &new_node);
        	last->next = new_node;
    	}
    	else
        	add_variable_2(env_var[i], env);
		i++;
	}
}
