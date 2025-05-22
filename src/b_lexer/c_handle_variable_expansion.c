#include "../../inc/__minishell.h"

static char	*get_var_value(char *varname, int *exit_status_ptr, t_variable_list *variables)
{
    char	*value_from_env;
    char	*exit_status_str;

    if (ft_strcmp(varname, "?") == 0)
    {
        if (!exit_status_ptr)
            return (ft_strdup("0")); 
        exit_status_str = ft_itoa(*exit_status_ptr);
        if (!exit_status_str)
            return (ft_strdup("")); // Or handle as error, but "" is consistent for now
        return (exit_status_str);
    }	
    value_from_env = get_variable_value(variables, varname); // This is from 0_env_management
    if (value_from_env == NULL)
        return (ft_strdup("")); // Variable not found or its stored value is NULL
    // If variable is found, always return a duplicate
    return (ft_strdup(value_from_env));
}

char	*handle_variable_expansion(char *raw_command, int *i,
    int *exit_status_ptr, t_variable_list *variables, char *result)
{
    char	*varname;
    char	*value;

    varname = get_varname(raw_command, i);
    if (!varname)
    {
        free(result);
        return (NULL);
    }
    value = get_var_value(varname, exit_status_ptr, variables);
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