/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:30:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/22 12:30:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count;
	int		i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
	}
	return (count);
}

static char	*get_next_word(char const *s, char c, int *i)
{
	char	*word;
	int		word_len;
	int		j;

	word_len = 0;
	while (s[*i + word_len] && s[*i + word_len] != c)
		word_len++;
	word = (char *)malloc(sizeof(char) * (word_len + 1));
	if (!word)
		return (NULL);
	j = 0;
	while (j < word_len)
	{
		word[j] = s[*i];
		j++;
		(*i)++;
	}
	word[j] = '\0';
	return (word);
}

static void	free_split_array(char **arr, int words)
{
	int	i;

	i = 0;
	while (i < words)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

static char	**alloc_and_fill(char const *s, char c, size_t words)
{
	char	**result;
	int		i;
	int		j;

	result = (char **)malloc(sizeof(char *) * (words + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < (int)words)
	{
		while (s[i] == c)
			i++;
		if (s[i])
		{
			result[j] = get_next_word(s, c, &i);
			if (!result[j])
			{
				free_split_array(result, j);
				return (NULL);
			}
			j++;
		}
	}
	result[j] = NULL;
	return (result);
}

char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**result;

	if (!s)
		return (NULL);
	words = count_words(s, c);
	result = alloc_and_fill(s, c, words);
	return (result);
}
