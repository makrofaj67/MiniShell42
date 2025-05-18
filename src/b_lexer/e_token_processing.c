/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_token_processing.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 10:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/18 10:00:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

// Exit status tutmak için global değişken
extern int g_exit_status;

static char	*get_env_value(const char *var_name)
{
	char	*value;
	
	if (var_name[0] == '?' && var_name[1] == '\0')
	{
		// $? için exit status değerini string olarak dön
		char buffer[12]; // Yeterince büyük: int değeri için
		sprintf(buffer, "%d", g_exit_status);
		return (ft_strdup(buffer));
	}
	
	value = getenv(var_name);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup("")); // Tanımlı değilse boş string döndür
}


char	*expand_token_variables(const char *token, int state)
{
	int		i;
	int		j;
	char	*result;
	char	var_name[256];
	char	*var_value;
	int		max_size;
	
	if (!token)
		return (NULL);
		
	// Tek tırnak içinde genişletme yapma (state=1)
	if (state == 1)
		return (ft_strdup(token));
		
	// Maksimum sonuç boyutunu tahmin et (kabaca)
	max_size = ft_strlen(token) * 10; // Bazı değişkenler uzun olabilir
	result = (char *)malloc(max_size); 
	if (!result)
		return (NULL);
		
	i = 0;
	j = 0;
	
	while (token[i] && j < max_size - 1)
	{
		if (state == 2 && token[i] == '\\' && token[i+1] != '\0')
		{
			if (token[i+1] == '$' || token[i+1] == '`' || 
				token[i+1] == '"' || token[i+1] == '\\')
			{
				result[j++] = token[i+1];
				i += 2;
				continue;
			}
		}
		if (token[i] == '$' && token[i+1] != '\0')
		{
			int k = 0;
			int dollar_pos = i;
			i++;
			while ((isalnum(token[i]) || token[i] == '_' || token[i] == '?') 
				  && k < 255)
			{
				var_name[k++] = token[i++];
				if (var_name[0] == '?' && k == 1)
					break;
			}
			var_name[k] = '\0';
			if (k == 0)
			{
				result[j++] = '$'; // Tek $ işareti
				continue;
			}
			
			// Değişkeni genişlet ve sonuca ekle
			var_value = get_env_value(var_name);
			if (var_value)
			{
				int v = 0;
				while (var_value[v] && j < max_size - 1)
					result[j++] = var_value[v++];
				free(var_value);
			}
			// NOT: i zaten doğru pozisyonda, değişken adı sonrasında devam ediyor
			// Bu şekilde $USERa gibi ifadelerde a karakteri korunmuş olacak
		}
		else
		{
			result[j++] = token[i++];
		}
	}
	
	result[j] = '\0';
	return (result);
}
