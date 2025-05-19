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

char *get_varname(char *raw_command, int *i)
{
    int dollar_pos = *i; // Original position of '$'
    int start_of_name;   // Index of the first character of the variable name (after '$')
    int len_of_name;     // Length of the variable name part (e.g., 1 for '?', N for 'VAR')

    start_of_name = dollar_pos + 1;
    len_of_name = 0;

    if (raw_command[start_of_name] == '?')
    {
        len_of_name = 1;
        // Update *i to point to the character AFTER '?'
        *i = start_of_name + len_of_name; 
        return (ft_strdup("?"));
    }
    
    // For regular variable names
    while (raw_command[start_of_name + len_of_name] && 
           ((raw_command[start_of_name + len_of_name] >= 'a' && raw_command[start_of_name + len_of_name] <= 'z') ||
            (raw_command[start_of_name + len_of_name] >= 'A' && raw_command[start_of_name + len_of_name] <= 'Z') ||
            (raw_command[start_of_name + len_of_name] >= '0' && raw_command[start_of_name + len_of_name] <= '9') ||
            raw_command[start_of_name + len_of_name] == '_'))
    {
        len_of_name++;
    }
    
    // Update *i to point to the character AFTER the variable name
    *i = start_of_name + len_of_name; 
    
    return (ft_substr(raw_command, start_of_name, len_of_name));
}

char *get_var_value(char *varname, int exit_status, t_env *env_list)
{
    t_env *current;
    char *exit_status_str; // Declare a pointer for the result of ft_itoa

    if (ft_strcmp(varname, "?") == 0)
    {
        // Convert exit_status to string
        // char exit_str[12]; // Sufficient to hold any int value // OLD LINE
        // ft_itoa(exit_status, exit_str); // OLD LINE
        exit_status_str = ft_itoa(exit_status); // Assign the returned string
        if (!exit_status_str) // Check if ft_itoa failed
            return (ft_strdup("")); // Return empty string or handle error
        // return (ft_strdup(exit_str)); // OLD LINE
        char *result = ft_strdup(exit_status_str); // Duplicate the string from ft_itoa
        free(exit_status_str); // Free the string returned by ft_itoa
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
