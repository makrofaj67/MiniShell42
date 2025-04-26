/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/26 18:23:31 by rakman            #+#    #+#             */
/*   Updated: 2023/04/26 19:16:25 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include <stdio.h>

/*
 * Command string'indeki token sayısını sayar
 */
static int	count_tokens(char *command)
{
	int	i;
	int	count;
	int	end;

	i = 0;
	count = 0;
	while (command[i])
	{
		while (command[i] && isspace(command[i]))
			i++;
		if (!command[i])
			break ;
		end = get_token_end(command, i);
		if (end == -1)
			return (-1);
		count++;
		i = end;
	}
	return (count);
}

/*
 * Token array'i için bellek ayırır
 */
char	**init_tokens_array(int size)
{
	char	**tokens;

	tokens = (char **)malloc(sizeof(char *) * (size + 1));
	if (!tokens)
		return (NULL);
	tokens[size] = NULL;
	return (tokens);
}

/*
 * Tek bir token'ı işler ve array'e ekler
 */
static int	process_token(char *command, int *pos, char **tokens, int i)
{
	int	start;
	int	end;

	while (command[*pos] && isspace(command[*pos]))
		(*pos)++;
	start = *pos;
	end = get_token_end(command, start);
	if (end == -1)
		return (-1);
	tokens[i] = get_token_value(command, start, end);
	if (!tokens[i])
		return (-1);
	*pos = end;
	return (0);
}

/*
 * Token array'ini temizler
 */
void	free_tokens_array(char **tokens)
{
	int	i;

	if (!tokens)
		return ;
	i = 0;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}

/*
 * Command string'inden token array'i çıkarır
 */
char	**extract_tokens(char *command)
{
	char	**tokens;
	int		token_count;
	int		i;
	int		pos;
	int		result;

	token_count = count_tokens(command);
	if (token_count == -1)
		return (NULL);
	tokens = init_tokens_array(token_count);
	if (!tokens)
	{
		printf("ERROR");
		return (NULL);
	}
	i = 0;
	pos = 0;
	while (i < token_count)
	{
		result = process_token(command, &pos, tokens, i);
		if (result == -1)
		{
			free_tokens_array(tokens);
			return (NULL);
		}
		i++;
	}
	return (tokens);
}
