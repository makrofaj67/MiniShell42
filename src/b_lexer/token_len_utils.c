/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_len_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 12:18:17 by rakman            #+#    #+#             */
/*   Updated: 2025/04/29 12:35:10 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

int	get_token_len(char *str)
{
	int		i;
	int		inc;
	char	quote;

	i = 0;
	quote = 0;
	while (str[i])
	{
		inc = 0;
		if (handle_backslash(str, i, quote, &inc))
			i++;
		else if (check_quote_state(str[i], &quote))
			;
		else if (!quote)
		{
			if (i == 0 && is_special_char(str[i]))
				return (handle_special_chars(str));
			else if (i > 0 && (is_special_char(str[i]) || is_space(str[i])))
				break ;
		}
		i++;
	}
	return (i);
}
