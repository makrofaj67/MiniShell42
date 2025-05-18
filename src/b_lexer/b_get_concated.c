/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_get_concated.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:36:19 by rakman            #+#    #+#             */
/*   Updated: 2025/05/18 16:04:00 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <stdlib.h> // For malloc

// Assuming ft_strlen is declared in __minishell.h or its includes

char *get_concated(char *command)
{
    if (!command)
        return (NULL);

    size_t command_len = ft_strlen(command);
    char *concated_str = (char *)malloc(sizeof(char) * (command_len + 1));
    if (!concated_str)
        return (NULL);

    int i = 0; // Index for input command string
    int j = 0; // Index for output concated_str
    int quote_state = 0; // 0: general, 1: in_squote, 2: in_dquote

    while (command[i])
    {
        if (quote_state == 0) // General state (not inside any quotes)
        {
            if (command[i] == '\'')
            {
                quote_state = 1; // Enter single quote state
                i++;             // Skip the quote character
            }
            else if (command[i] == '"')
            {
                quote_state = 2; // Enter double quote state
                i++;             // Skip the quote character
            }
            else
            {
                concated_str[j++] = command[i++]; // Copy character
            }
        }
        else if (quote_state == 1) // Inside single quotes ('...')
        {
            if (command[i] == '\'')
            {
                quote_state = 0; // Exit single quote state
                i++;             // Skip the quote character
            }
            else
            {
                concated_str[j++] = command[i++]; // Copy character (even if it's a double quote)
            }
        }
        else // quote_state == 2, Inside double quotes ("...")
        {
            if (command[i] == '"')
            {
                quote_state = 0; // Exit double quote state
                i++;             // Skip the quote character
            }
            else
            {
                concated_str[j++] = command[i++]; // Copy character (even if it's a single quote)
            }
        }
    }
    concated_str[j] = '\0';
    printf("%s", concated_str);
    return (concated_str);
}
