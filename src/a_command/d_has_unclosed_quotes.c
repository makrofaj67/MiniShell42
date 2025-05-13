/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   d_has_unclosed_quotes.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:51:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/02 01:19:23 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/*
** Detects if a command contains any unclosed quote pairs
** Handles both single and double quotes, respecting escape characters
** Follows shell parsing rules where quotes must be properly paired
** 
** @param command: The command string to check for unclosed quotes
** @return: 1 if there are unclosed quotes, 0 if all quotes are properly closed
*/

int check_quotes_mine_implementation(char *str) 
{
 	int i;
 	int state;

 	i = 0;
 	state = 0;
	if (str == NULL)
		return (0);
	while (str[i]) 
	{
    	if (state == 0)
    	{
			if (str[i] == '\\')
			{
				if (str[i+1] != '\0')
					i++;
			}
			else
			{
				if (str[i] == '\'')
				{
					state = 1;
    			}
				else if (str[i] == '"') 
				{
					state = 2;
    			}
			}
    	} 
		else if (state == 1) 
		{
    		if (str[i] == '\'') 
			{
				state = 0;
    		}
		}
		else if (state == 2) 
		{
			if (str[i] == '\\')
            {
                if (str[i+1] != '\0')
                {
                    if (str[i+1] == '$' || str[i+1] == '`' || \
                        str[i+1] == '"' || str[i+1] == '\\')
                    {
                        i++;
                    }
                }

            }
            else if (str[i] == '"') 
            {
                state = 0;
            }
		}
		i++;
	}
	return (state || 0);
}

