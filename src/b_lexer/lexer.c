/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 11:52:03 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:33:50 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/* Bu fonksiyonlar token_len_utils.c'de tanımlandı */
int	is_special_char(char c);
int	is_space(char c);

static char	*extract_token(char *command, int *index)
{
	int		len;
	char	*token;
	int		i;

	while (is_space(command[*index]))
		(*index)++;
	if (command[*index] == '\0')
		return (NULL);
	len = get_token_len(&command[*index]);
	token = (char *)malloc(sizeof(char) * (len + 1));
	if (!token)
		return (NULL);
	i = 0;
	while (i < len)
	{
		token[i] = command[*index + i];
		i++;
	}
	token[i] = '\0';
	*index += len;
	return (token);
}

t_t_list	*get_lexed(char *command)
{
	int			i;
	char		*token_string;
	t_t_list	*list;

	list = init_token_list();
	if (!list)
		return (NULL);
	i = 0;
	while (command[i])
	{
		token_string = extract_token(command, &i);
		if (token_string)
		{
			add_token_to_list(list, token_string);
			free(token_string);
		}
	}
	return (list);
}
