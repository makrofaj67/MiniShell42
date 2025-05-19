/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   1a_get_var_val.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 23:20:00 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:42:36 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *add_char_to_result(char *result, char c)
{
    char *new_result;
    size_t len;

    len = ft_strlen(result);
    new_result = (char *)malloc(sizeof(char) * (len + 2));
    if (new_result == NULL)
    {
        free(result);
        return (NULL);
    }
    ft_strcpy(new_result, result);
    new_result[len] = c;
    new_result[len + 1] = '\0';
    free(result);
    return (new_result);
}

char *add_str_to_result(char *result, char *str)
{
    char *new_result;

    new_result = ft_strjoin(result, str);
    free(result);
    return (new_result);
}



char *get_var_value(char *varname, int exit_status, t_env *env_list)
{
    t_env *current;
    char *exit_status_str;

    if (ft_strcmp(varname, "?") == 0)
    {

        exit_status_str = ft_itoa(exit_status); 
        if (!exit_status_str)
            return (ft_strdup("0"));
        char *result = ft_strdup(exit_status_str); 
        free(exit_status_str);
        return (result);
    }
    current = env_list;
    while (current)
    {
        if (ft_strcmp(current->key, varname) == 0)
            return (ft_strdup(current->value));
        current = current->next;
    }
    return (ft_strdup(""));
}

char *get_varname(char *raw_command, int *i)
{
    int dollar_pos = *i; 
    int start_of_name;   
    int len_of_name;     

    start_of_name = dollar_pos + 1;
    len_of_name = 0;
    if (raw_command[start_of_name] == '?')
    {
        len_of_name = 1;
        *i = start_of_name + len_of_name; 
        return (ft_strdup("?"));
    }
    while (raw_command[start_of_name + len_of_name] && 
           ((raw_command[start_of_name + len_of_name] >= 'a' && raw_command[start_of_name + len_of_name] <= 'z') ||
            (raw_command[start_of_name + len_of_name] >= 'A' && raw_command[start_of_name + len_of_name] <= 'Z') ||
            (raw_command[start_of_name + len_of_name] >= '0' && raw_command[start_of_name + len_of_name] <= '9') ||
            raw_command[start_of_name + len_of_name] == '_'))
    {
        len_of_name++;
    }
    *i = start_of_name + len_of_name; 
    return (ft_substr(raw_command, start_of_name, len_of_name));
}
