/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   g_tokenizer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 10:15:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/20 10:15:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

static int	is_operator_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

static int	is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
        || c == '\f' || c == '\r');
}

static int	handle_operator(t_token_list *token_list, char *input, int *i)
{
    char	*token_str;

    if ((input[*i] == '>' && input[*i + 1] == '>') ||
        (input[*i] == '<' && input[*i + 1] == '<'))
    {
        token_str = ft_substr(input, *i, 2);
        *i += 2;
    }
    else
    {
        token_str = ft_substr(input, *i, 1);
        (*i)++;
    }
    if (token_str)
    {
        add_token(token_list, token_str);
        free(token_str);
        return (1);
    }
    return (0);
}

static int	extract_token(t_token_list *token_list, char *input,
        int start, int end)
{
    char	*token_str;

    token_str = ft_substr(input, start, end - start);
    if (!token_str)
        return (0);
    add_token(token_list, token_str);
    free(token_str);
    return (1);
}

t_token_list	*create_tokens(char *input)
{
    t_token_list	*token_list;
    int				i;
    int				start;

    if (!input || !(token_list = create_token_list()))
        return (NULL);
    i = 0;
    while (input[i])
    {
        while (input[i] && is_whitespace(input[i]))
            i++;
        if (!input[i])
            break ;
        if (is_operator_char(input[i]))
        {
            if (!handle_operator(token_list, input, &i))
                return (token_list);
            continue ;
        }
        start = i;
        while (input[i] && !is_whitespace(input[i]) 
            && !is_operator_char(input[i]))
            i++;
        if (!extract_token(token_list, input, start, i))
            return (token_list);
    }
    return (token_list);
}

