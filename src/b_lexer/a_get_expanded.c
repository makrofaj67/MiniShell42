#include "../../inc/__minishell.h"

char *get_expanded(char *raw_command, int exit_status, t_env *env_list)
{
    int i;
    char *result;
    char *varname;
    char *value;

    i = 0;
    result = init_expanded_str();
    if (!result)
        return (NULL);
    while (raw_command[i])
    {
        if (raw_command[i] == '\\')
        {
            result = handle_backslash_expansion(raw_command, &i, result);
            if (!result)
                return (NULL); 
            continue; 
        }
        else if (raw_command[i] == '$' && is_need_for_expanding(raw_command, i))
        {
            varname = get_varname(raw_command, &i); 
            if (!varname)
            {
                free(result);
                return (NULL);
            }
            value = get_var_value(varname, exit_status, env_list);
            if (!value) 
            {
                free(varname);
                free(result);
                return (NULL);
            }
            result = add_str_to_result(result, value);
            free(varname);
            free(value);
            if (!result) 
                return (NULL); 
        }
        else
        {
            result = add_char_to_result(result, raw_command[i]);
            if (!result) 
                return (NULL);
            i++;
        }
    }
    return (result);
}

