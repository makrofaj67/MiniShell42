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



char	*expand_token_variables(const char *token, int state)
{
	int		i;
	int		j;
	char	*result;
	char	var_name_buffer[256];
	char	*var_value;
	size_t	token_len;
	size_t	max_result_size;

	if (!token)
		return (NULL);
	if (state == 1) // Tek tırnak içinde değişken genişletme olmaz
		return (ft_strdup(token));

	token_len = ft_strlen(token);
	max_result_size = token_len * 10 + 256;
	if (max_result_size <= token_len)
		max_result_size = token_len + 256;
	result = (char *)malloc(max_result_size);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;
	while (token[i] != '\0' && j < max_result_size - 1)
	{
		if (token[i] == '$' && token[i + 1] != '\0') // $ ve ardından en az bir karakter var
		{
			int k = 0;
			int var_char_idx = i + 1; // $ işaretinden sonraki ilk karakterin indeksi

			// $? özel durumu
			if (token[var_char_idx] == '?') 
			{
				var_name_buffer[k++] = token[var_char_idx++];
				var_name_buffer[k] = '\0';
			}
			// Değişken adı harf veya _ ile başlamalı
			else if (isalpha(token[var_char_idx]) || token[var_char_idx] == '_') 
			{
				var_name_buffer[k++] = token[var_char_idx++]; // İlk karakteri al
				// Sonraki karakterler alfanumerik veya _ olabilir
				while ((isalnum(token[var_char_idx]) || token[var_char_idx] == '_') && k < 255)
				{
					var_name_buffer[k++] = token[var_char_idx++];
				}
				var_name_buffer[k] = '\0';
			}
			else // $ sonrası '?' veya (harf|_) değilse, bu geçerli bir değişken değil
			{
				// Sadece '$' karakterini result'a ekle ve devam et
				if (j < max_result_size - 1) 
					result[j++] = token[i++];
				else 
					break; // Buffer dolu
				continue; 
			}

			// Eğer var_name_buffer'a bir şey yazıldıysa (k > 0), genişletmeyi yap
			if (k > 0) {
				var_value = get_env_value(var_name_buffer);
				if (var_value)
				{
					int v_idx = 0;
					while (var_value[v_idx] != '\0' && j < max_result_size - 1)
					{
						result[j++] = var_value[v_idx++];
					}
					free(var_value);
				}
				i = var_char_idx; // Ana 'i'yi, değişken adının okunduğu son pozisyona ayarla
			}
		}
		else // Normal karakter veya tek başına '$' (satır sonu veya $ sonrası geçersiz)
		{
			if (j < max_result_size - 1) 
				result[j++] = token[i++];
			else 
				break;
		}
	}
	result[j] = '\0';
	return (result);
}

/*
** Bir token içindeki $ işareti ile başlayan çevre değişkenlerini genişletir
** @param token: Genişletilecek token
** @param state: 0=tırnaksız, 1=tek tırnak, 2=çift tırnak
** @return: Genişletilmiş token
*/


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


