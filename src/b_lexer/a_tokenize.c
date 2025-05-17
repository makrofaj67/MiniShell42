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


/**
 * Processes tokens to handle quotes correctly:
 * - For fully quoted strings (status 1,2), removes surrounding quotes
 * - For normal tokens, processes interior quotes to produce "clean" tokens
 * 
 * @param token The raw token string
 * @param status The token state (0=normal, 1=single quote, 2=double quote)
 * @return The processed token string
 */
static char	*prepare_token(char *token, int status)
{
	char	*processed;
	char	*result;
	int		i;
	int		j;
	int		len;
	int		in_squote;
	int		in_dquote;
	
	if (!token)
		return (NULL);
	
	// Calculate initial length
	len = 0;
	while (token[len])
		len++;
	
	// Handle fully quoted tokens (remove outer quotes)
	if ((status == 1 || status == 2) && len >= 2)
	{
		processed = ft_substr(token, 1, len - 2);
		free(token);
		token = processed;
		
		// If resulting token is empty, return it
		if (!token[0])
			return (token);
		
		// Recalculate the length after removing quotes
		len = 0;
		while (token[len])
			len++;
	}
	
	// Handle interior quotes in normal tokens
	if (status == 0)
	{
		// Allocate buffer for processed token (same size is enough)
		processed = (char *)malloc(len + 1);
		if (!processed)
		{
			free(token);
			return (NULL);
		}
		
		// Process the token, removing interior quotes
		i = 0;
		j = 0;
		in_squote = 0;
		in_dquote = 0;
		
		while (i < len)
		{
			// Handle opening/closing quotes
			if (token[i] == '\'' && !in_dquote)
			{
				in_squote = !in_squote;
				i++;
				continue;
			}
			else if (token[i] == '"' && !in_squote)
			{
				in_dquote = !in_dquote;
				i++;
				continue;
			}
			
			// Handle escape sequences
			if (token[i] == '\\' && !in_squote)
			{
				if (i + 1 < len)
				{
					// In double quotes, only specific chars can be escaped
					if (in_dquote)
					{
						if (token[i+1] == '$' || token[i+1] == '`' || 
							token[i+1] == '"' || token[i+1] == '\\')
						{
							processed[j++] = token[i+1];
							i += 2;
							continue;
						}
					}
					// Outside quotes, any char can be escaped
					else if (!in_dquote)
					{
						processed[j++] = token[i+1];
						i += 2;
						continue;
					}
				}
			}
			
			// Copy normal character
			processed[j++] = token[i++];
		}
		
		processed[j] = '\0';
		result = ft_strdup(processed);
		
		free(processed);
		free(token);
		return (result);
	}
	
	// For other cases, just return the token as is
	return (token);
}


static char	*handle_double_status(const char *command, int *index)
{
	int		start;
	
	start = *index;
	(*index)++;
//eğer quotelar sonunda boşluk harici başka bi şey varsa joinle	
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
	return (ft_substr(command, start, *index - start));
}

static char	*handle_single_status(const char *command, int *index)
{
	int		start;
	
	start = *index;
	(*index)++;
	//eğer quotelar sonunda boşluk harici başka şeyler varsa join
	while (command[*index] && command[*index] != '\0')
	{
		if (command[*index] == '\'')
		{
			(*index)++;
			break;
		}
		(*index)++;
	}
	return (ft_substr(command, start, *index - start));
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
	int	token_start;
	int	state;
  
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
	return (ft_substr(command, token_start, (*index_ptr) - token_start));
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

t_token_list	*tokenize_command(char *command)
{
	t_token_list	*token_list;
	char			*to_send_to_handler;
	char			*current_token_value;
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
		to_send_to_handler = filter_token_string(command, &i, status);
		current_token_value = prepare_token(to_send_to_handler, status);
		if (current_token_value != NULL)
		{
			add_token(token_list, current_token_value);
			free(current_token_value);
		}
	}
	print_tokens(token_list);
	return (token_list);
}


