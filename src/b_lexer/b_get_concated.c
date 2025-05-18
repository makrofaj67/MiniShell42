/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_get_concated.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 15:36:19 by rakman            #+#    #+#             */
/*   Updated: 2025/05/18 15:45:01 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *get_concated(char *command)
{
    if (!command)
        return (NULL);

    size_t command_len = ft_strlen(command);
    char *concated_str = (char *)malloc(sizeof(char) * (command_len + 1));
    if (!concated_str)
        return (NULL);

    int i = 0;
    int j = 0;
    int quote_state = 0;

    while (command[i])
    {
        if (command[i] == '\'' && quote_state != 2)
        {
            if (quote_state == 1)
                quote_state = 0;
            else 
                quote_state = 1;
            i++; 
        }
        else if (command[i] == '"' && quote_state != 1)
        {
            if (quote_state == 2)
                quote_state = 0;
            else
                quote_state = 2;
            i++;
        }
        else 
        {
            concated_str[j++] = command[i++];
        }
    }
    concated_str[j] = '\0';
	//printf("%s", concated_str);
    return (concated_str);
}
