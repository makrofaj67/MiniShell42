/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   a_get_expanded.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 01:35:50 by rakman            #+#    #+#             */
/*   Updated: 2025/05/19 23:38:44 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

char *init_expanded_str(void)
{
    char *result;

    result = (char *)malloc(sizeof(char));
    if (result == NULL)
        return (NULL);
    result[0] = '\0';
    return (result);
}

static char	*handle_regular_char(char *raw_command, int *i, char *result)
{
    result = add_char_to_result(result, raw_command[*i]);
    if (!result)
        return (NULL);
    (*i)++;
    return (result);
}

char	*handle_variable_expansion(char *raw_command, int *i,
    t_env_and_exit *envx, char *result)
{
    char	*varname;
    char	*value;

    varname = get_varname(raw_command, i);
    if (!varname)
    {
        free(result);
        return (NULL);
    }
    value = get_var_value(varname, envx->exit_status, envx->env_list);
    if (!value)
    {
        free(varname);
        free(result);
        return (NULL);
    }
    result = add_str_to_result(result, value);
    free(varname);
    free(value);
    return (result);
}

char	*handle_backslash(char *raw_command, int *i_ptr,
                                 char *current_result)
{
    int	quote_state_at_backslash;

    quote_state_at_backslash = get_quote_state_at_position(raw_command, *i_ptr);
    if (quote_state_at_backslash == 0)
        return (handle_backslash_no_quotes(raw_command, i_ptr, current_result));
    else if (quote_state_at_backslash == 1)
        return (handle_backslash_single_quotes(i_ptr, current_result));
    else if (quote_state_at_backslash == 2)
        return (handle_backslash_double_quotes(raw_command, i_ptr, current_result));
    return (current_result);
}

char	*get_expanded(char *raw_command, int *exit_status, t_env *env_list)
{
    int				i;
    char			*result;
    t_env_and_exit	envx;

    i = 0;
    envx.exit_status = *exit_status;
    envx.env_list = env_list;
    envx.export_list = NULL;
    result = init_expanded_str();
    if (!result)
        return (NULL);
    while (raw_command[i])
    {
        if (raw_command[i] == '\\')
            result = handle_backslash(raw_command, &i, result);
        else if (raw_command[i] == '$' && is_need_for_expanding(raw_command, i))
            result = handle_variable_expansion(raw_command, &i, &envx, result);
        else
            result = handle_regular_char(raw_command, &i, result);
        if (result == NULL)
            return (NULL);
    }
    *exit_status = envx.exit_status;
    return (result);
}
