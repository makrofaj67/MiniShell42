/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1b_handle_backslash.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:18:16 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:41:05 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char	*handle_backslash_no_quotes(char *raw_command, int *i_ptr, 
                                      char *current_result)
{
	char *new_result;

	if (raw_command[*i_ptr + 1])
	{
		new_result = add_char_to_result(current_result, raw_command[*i_ptr + 1]);
		if (new_result == NULL)
			return (NULL);
		*i_ptr = *i_ptr + 1;
	}
	*i_ptr = *i_ptr + 1;
	return (new_result);
}

char	*handle_backslash_single_quotes(int *i_ptr, char *current_result)
{
    char	*new_result;

    new_result = add_char_to_result(current_result, '\\');
    if (new_result == NULL)
        return (NULL);
	*i_ptr = *i_ptr + 1;
    return (new_result);
}
char	*handle_backslash_double_quotes(char *raw_command, int *i_ptr,
                                         char *current_result)
{
	char *new_result;
	char char_after_backslash; 

	new_result = current_result;
	char_after_backslash = raw_command[*i_ptr + 1];
	if (char_after_backslash == '$' || char_after_backslash == '"' ||
		char_after_backslash == '\\' || char_after_backslash == '`')
	{
		new_result = add_char_to_result(new_result, char_after_backslash);
		if (new_result == NULL)
			return (NULL);
		*i_ptr = *i_ptr + 1;
	}
	else
	{
		new_result = add_char_to_result(new_result, '\\');
		if (new_result == NULL)
			return (NULL);
	}
	*i_ptr = *i_ptr + 1;
	return (new_result);
}


