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

void handle_zero(char *command, int *index, int *state)
{
    if (command[*index] == '\\')
    {
        if (command[*index + 1] != '\0')
            (*index)++;
    }
    else
    {
        if (command[*index] == '\'')
        {
            *state = 1;
        }
        else if (command[*index] == '"') 
        {
            *state = 2;
        }
    }
}

void handle_one(char *command, int *index, int *state)
{
    if (command[*index] == '\'') 
    {
        *state = 0;
    }
}

void handle_two(char *command, int *index, int *state)
{
    if (command[*index] == '\\')
    {
        if (command[*index + 1] != '\0')
        {
            if (command[*index + 1] == '$' || command[*index + 1] == '`' || 
                command[*index + 1] == '"' || command[*index + 1] == '\\')
            {
                (*index)++;
            }
        }
    }
    else if (command[*index] == '"') 
    {
        *state = 0;
    }
}

int has_unclosed_quotes(char *command) 
{
    int i;
    int state;

    i = 0;
    state = 0;
    if (command == NULL)
        return (0);
    while (command[i]) 
    {
        if (state == 0)
            handle_zero(command, &i, &state);
        else if (state == 1)
            handle_one(command, &i, &state);
        else if (state == 2)
            handle_two(command, &i, &state);
        i++;
    }
    return (state || 0);
}