/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_tokenize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 10:00:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:14:35 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

// Forward declarations
static char	*handle_zero_status(const char *command, int *index);
static char	*process_quotes_in_token(char *token);

size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

char	*ft_strdup(const char *s1)
{
	char	*dup;
	size_t	len;
	size_t	i;

	len = 0;
	while (s1[len])
		len++;
	dup = (char *)malloc(sizeof(char) * (len + 1));
	if (dup == NULL)
		return (NULL);
	i = 0;
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub;
	size_t	s_len;
	size_t	sub_len;
	size_t	i;

	if (!s)
		return (NULL);
	s_len = 0;
	while (s[s_len])
		s_len++;
	if (start >= s_len)
		return (ft_strdup(""));
	sub_len = len;
	if (start + sub_len > s_len)
		sub_len = s_len - start;
	sub = (char *)malloc(sub_len + 1);
	if (!sub)
		return (NULL);
	i = 0;
	while (i < sub_len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}


static char	*prepare_token(char *token)
{
	char	*processed;
	
	if (!token)
		return (NULL);
	
	processed = ft_strdup(token);
	free(token);
	return (processed);
}


static char	*handle_double_status(const char *command, int *index)
{
	int		start;
	char	*token;
	char	*processed_token;
	char	*next_token;
	char	*joined_token;
	
	start = *index;
	(*index)++;
	while (command[*index] && command[*index] != '\0')
	{
		if (command[*index] == '\\')
		{
			if (command[*index + 1] != '\0' && 
				(command[*index + 1] == '$' || command[*index + 1] == '`' || 
				 command[*index + 1] == '"' || command[*index + 1] == '\\'))
				(*index)++;
		}
		else if (command[*index] == '"')
		{
			(*index)++;
			break;
		}
		(*index)++;
	}
	token = ft_substr(command, start, *index - start);
	
	// Sadece çift tırnakları kaldır, içeriğe dokunma
	if (token && ft_strlen(token) >= 2)
		processed_token = ft_substr(token, 1, ft_strlen(token) - 2);
	else
		processed_token = ft_strdup(token);
	free(token);
	
	// Tırnak sonunda, boşluk gelmeden başka karakterler varsa
	if (command[*index] && !isspace(command[*index]) && 
		command[*index] != '|' && command[*index] != '<' && command[*index] != '>')
	{
		int next_start = *index;
		next_token = handle_zero_status(command, index);
		joined_token = ft_strjoin(processed_token, next_token);
		free(processed_token);
		free(next_token);
		return (joined_token);
	}
	
	return (processed_token);
}

static char	*handle_single_status(const char *command, int *index)
{
	int		start;
	char	*token;
	char	*processed_token;
	char	*next_token;
	char	*joined_token;
	
	start = *index;
	(*index)++;
	while (command[*index] && command[*index] != '\0')
	{
		if (command[*index] == '\'')
		{
			(*index)++;
			break;
		}
		(*index)++;
	}
	token = ft_substr(command, start, *index - start);
	
	// Sadece tek tırnakları kaldır, içeriğe dokunma
	if (token && ft_strlen(token) >= 2)
		processed_token = ft_substr(token, 1, ft_strlen(token) - 2);
	else
		processed_token = ft_strdup(token);
	free(token);
	
	// Tırnak sonunda, boşluk gelmeden başka karakterler varsa
	if (command[*index] && !isspace(command[*index]) && 
		command[*index] != '|' && command[*index] != '<' && command[*index] != '>')
	{
		int next_start = *index;
		next_token = handle_zero_status(command, index);
		joined_token = ft_strjoin(processed_token, next_token);
		free(processed_token);
		free(next_token);
		return (joined_token);
	}
	
	return (processed_token);
}

t_token_type	define_token_type(const char *command, int current_pos)
{
	if (command[current_pos] == '>')
	{
		if (command[current_pos + 1] != '\0')
		{
			if (command[current_pos + 1] == '>')
				return (APPEND);
		}
		return (RDRT_OUT);
	}
	if (command[current_pos] == '<')
	{
		if (command[current_pos + 1] != '\0')
		{
			if (command[current_pos + 1] == '<')
				return (HEREDOC);
		}
		return (RDRT_IN);
	}
	if (command[current_pos] == '|')
		return (PIPE);
	return (WORD);
}

static void	handle_quotes_in_word(const char *command, int *index_ptr, int *state)
{
	if (*state == 0)
	{
		if (command[*index_ptr] == '\\')
		{
			if (command[*index_ptr + 1] != '\0')
				(*index_ptr)++;
		}
		else if (command[*index_ptr] == '\'')
			*state = 1;
		else if (command[*index_ptr] == '"')
			*state = 2;
	}
	else if (*state == 1)
	{
		if (command[*index_ptr] == '\'')
			*state = 0;
	}
	else if (*state == 2)
	{
		if (command[*index_ptr] == '\\')
		{
			if (command[*index_ptr + 1] != '\0' && 
				(command[*index_ptr + 1] == '$' || command[*index_ptr + 1] == '`' || 
				command[*index_ptr + 1] == '"' || command[*index_ptr + 1] == '\\'))
				(*index_ptr)++;
		}
		else if (command[*index_ptr] == '"')
			*state = 0;
	}
}

char	*handle_word_token(const char *command, int *index_ptr)
{
	int		token_start;
	int		state;
	char	*token;
	char	*processed_token;
  
	token_start = *index_ptr;
	state = 0;
	while (command[*index_ptr] != '\0')
	{
		handle_quotes_in_word(command, index_ptr, &state);
		if (state == 0 && (isspace(command[*index_ptr]) || 
			command[*index_ptr] == '|' || command[*index_ptr] == '<' || 
			command[*index_ptr] == '>'))
			break ;
		(*index_ptr)++;
	}
	if (*index_ptr == token_start)
		return (ft_strdup(""));

	// Kelimenin içindeki tırnakları temizleyen bir fonksiyon yazalım
	token = ft_substr(command, token_start, (*index_ptr) - token_start);
	processed_token = process_quotes_in_token(token);
	free(token);
	return (processed_token);
} 

char	*handle_operator_token(const char *command, int *index_ptr, t_token_type type)
{
	int		operator_len;
	char	*op_value;

	operator_len = 0;
	if (type == APPEND || type == HEREDOC)
		operator_len = 2;
	else if (type == RDRT_OUT || type == RDRT_IN || type == PIPE)
		operator_len = 1;
	else
		return (ft_strdup(""));
	op_value = ft_substr(command, *index_ptr, operator_len);
	*index_ptr += operator_len;
	return (op_value);
}

static int	check_next_token_quote(const char *command, int index)
{
	while (command[index] && !isspace(command[index]) &&
		   command[index] != '|' && command[index] != '<' && command[index] != '>')
	{
		// Eğer tırnak bulursak, türünü belirleyip döndürüyoruz
		if (command[index] == '\'')
			return (1);
		else if (command[index] == '"')
			return (2);
		index++;
	}
	return (0);  // Tırnak bulunamadı
}

static char	*handle_zero_status(const char *command, int *index)
{
	t_token_type	type;
	char			*token;
	char			*next_token;
	char			*joined_token;
	int				next_index;
	int				quote_type;

	type = define_token_type(command, *index);
	if (type != WORD)
	{
		token = handle_operator_token(command, index, type);
		return (token);
	}

	// Normal kelime tokenını al
	token = handle_word_token(command, index);
	
	// Kelimeden sonra boşluk olmadan tırnak geliyorsa (örn: abc"def")
	while ((quote_type = check_next_token_quote(command, *index)) != 0)
	{
		// Tırnağı işle
		next_index = *index;
		if (quote_type == 1)
			next_token = handle_single_status(command, &next_index);
		else
			next_token = handle_double_status(command, &next_index);
		
		// Tokenleri birleştir
		joined_token = ft_strjoin(token, next_token);
		free(token);
		free(next_token);
		token = joined_token;
		
		// İndeksi güncelle
		*index = next_index;
	}
	
	return (token);
}

static char	*filter_token_string(const char *command, int *index, int status)
{
	char *filtered;

	if (status == 0)
		return (handle_zero_status(command, index));
	else if (status == 1)
		return (handle_single_status(command, index));
	else if (status == 2)
		return (handle_double_status(command, index));
	return (NULL);
}

static int	define_state_status(const char *command, int pos)
{
	if (command[pos] == '\'')
		return (1);
	else if (command[pos] == '"')
		return (2);
	return (0);
}

static char	*process_quotes_in_token(char *token)
{
	int		i;
	int		j;
	int		len;
	char	*result;
	int		in_quote;
	char	quote_char;

	if (!token)
		return (NULL);
	
	len = ft_strlen(token);
	result = (char *)malloc(len + 1); 
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	in_quote = 0;
	quote_char = 0;
	
	while (i < len)
	{

		if ((token[i] == '\'' || token[i] == '"') && !in_quote)
		{
			in_quote = 1;
			quote_char = token[i];
			i++;
		}
		else if (in_quote && token[i] == quote_char)
		{
			in_quote = 0;
			i++;
		}
		else
		{
			result[j++] = token[i++];
		}
	}
	
	result[j] = '\0'; 
	return (result);
}


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

/*
** Bir token içindeki $ işareti ile başlayan çevre değişkenlerini genişletir
** @param token: Genişletilecek token
** @param state: 0=tırnaksız, 1=tek tırnak, 2=çift tırnak
** @return: Genişletilmiş token
*/
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
		// Kaçış karakteri kontrolü (sadece çift tırnak içinde ve destekleniyorsa)
		if (state == 2 && token[i] == '\\' && token[i+1] != '\0')
		{
			// $ ` " \ karakterlerini kaçırır 
			if (token[i+1] == '$' || token[i+1] == '`' || 
				token[i+1] == '"' || token[i+1] == '\\')
			{
				result[j++] = token[i+1]; // Kaçırılan karakteri al
				i += 2; // İki karakter atla (\ ve kaçırılan karakter)
				continue;
			}
		}
		
		// Değişken genişletme
		if (token[i] == '$' && token[i+1] != '\0')
		{
			int k = 0;
			i++; // $ işaretini atla
			
			// Değişken adını al (alfanumerik + _)
			while ((isalnum(token[i]) || token[i] == '_' || token[i] == '?') 
				  && k < 255)
			{
				var_name[k++] = token[i++];
				// $? özel durumu için hemen çık
				if (var_name[0] == '?' && k == 1)
					break;
			}
			var_name[k] = '\0';
			
			// Boş değişken adı kontrolü
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
		}
		else
		{
			result[j++] = token[i++];
		}
	}
	
	result[j] = '\0';
	return (result);
}
/*
** Processes and expands a token without quotes (state 0).
** In this state, all variables are expanded.
**
** @param token: The token to be processed
** @return: The processed token with all variables expanded
*/
char	*process_and_expand_for_zero(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For zero state (no quotes), expand all variables
	expanded = expand_token_variables(token, 0);
	free(token);
	return (expanded);
}

/*
** Processes a token within single quotes (state 1).
** In this state, no variable expansion occurs - literal values are preserved.
**
** @param token: The token to be processed
** @return: The processed token with literal values preserved
*/
char	*process_and_expand_for_single(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For single quote state, don't expand variables
	expanded = expand_token_variables(token, 1);
	free(token);
	return (expanded);
}

/*
** Processes and expands a token within double quotes (state 2).
** In this state, variables are expanded but some escape sequences are processed.
**
** @param token: The token to be processed
** @return: The processed token with variables expanded and escapes processed
*/
char	*process_and_expand_for_double(char *token)
{
	char	*expanded;

	if (token == NULL)
		return (NULL);
		
	// For double quote state, expand variables but handle escape sequences
	expanded = expand_token_variables(token, 2);
	free(token);
	return (expanded);
}


t_token_list	*tokenize_command(char *command)
{
	t_token_list	*token_list;
	char			*filtered_token;
	char			*raw_token;
	int				i;
	int				status;

	token_list = create_token_list();
	if (token_list == NULL)
		return (NULL);
	i = 0;
	while (command[i])
	{
		while (command[i] != '\0' && isspace(command[i]))
			i++;
		if (command[i] == '\0')
			break ;
		status = define_state_status(command, i);
		filtered_token = filter_token_string(command, &i, status);
		if (status == 0)
			raw_token = process_and_expand_for_zero(filtered_token);
		else if (status == 1)
			raw_token = process_and_expand_for_single(filtered_token);
		else if (status == 2)
			raw_token = process_and_expand_for_double(filtered_token);
		if (raw_token != NULL)
		{
			add_token(token_list, raw_token);
			free(raw_token);
		}
	}
	print_tokens(token_list);
	return (token_list);
}


