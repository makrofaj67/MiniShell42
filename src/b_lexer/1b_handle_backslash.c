#include "../../inc/__minishell.h"

static char	*handle_backslash_no_quotes(char *raw_command, int *i_ptr, 
                                      char *current_result)
{
    char	*new_result;
    int		current_i;

    new_result = current_result;
    current_i = *i_ptr;
    current_i++;
    if (raw_command[current_i])
    {
        new_result = add_char_to_result(new_result, raw_command[current_i]);
        if (!new_result)
            return (NULL);
        current_i++;
    }
    *i_ptr = current_i;
    return (new_result);
}

static char	*handle_backslash_single_quotes(int *i_ptr, char *current_result)
{
    char	*new_result;

    new_result = add_char_to_result(current_result, '\\');
    if (!new_result)
        return (NULL);
    (*i_ptr)++;
    return (new_result);
}

static char	*handle_backslash_double_quotes(char *raw_command, int *i_ptr,
                                         char *current_result)
{
    char	*new_result;
    int		current_i;

    new_result = current_result;
    current_i = *i_ptr;
    current_i++;
    if (raw_command[current_i] == '$' || raw_command[current_i] == '"' ||
        raw_command[current_i] == '\\' || raw_command[current_i] == '`')
    {
        new_result = add_char_to_result(new_result, raw_command[current_i]);
        if (!new_result)
            return (NULL);
        current_i++;
    }
    else
    {
        new_result = add_char_to_result(new_result, '\\');
        if (!new_result)
            return (NULL);
    }
    *i_ptr = current_i;
    return (new_result);
}

char	*handle_backslash_expansion(char *raw_command, int *i_ptr,
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


