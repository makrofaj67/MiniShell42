#include "../../inc/__minishell.h"

static char	*handle_backslash_no_quotes(char *raw_command, int *i_ptr, 
                                      char *current_result)
{
	char *new_result;

	if (raw_command[*i_ptr + 1])
	{
		new_result = add_char_to_result(current_result, raw_command[*i_ptr + 1]);
		if (new_result == NULL)
			return (NULL);
		*i_ptr = *i_ptr + 1;
	}
	*i_ptr = *i_ptr + 1;
	return (new_result);
}

static char	*handle_backslash_single_quotes(int *i_ptr, char *current_result)
{
    char	*new_result;

    new_result = add_char_to_result(current_result, '\\');
    if (new_result == NULL)
        return (NULL);
	*i_ptr = *i_ptr + 1;
    return (new_result);
}
static char	*handle_backslash_double_quotes(char *raw_command, int *i_ptr,
                                         char *current_result)
{
	char *new_result;
	char char_after_backslash; 

	new_result = current_result;
	char_after_backslash = raw_command[*i_ptr + 1];
	if (char_after_backslash == '$' || char_after_backslash == '"' ||
		char_after_backslash == '\\' || char_after_backslash == '`')
	{
		new_result = add_char_to_result(new_result, char_after_backslash);
		if (new_result == NULL)
			return (NULL);
		*i_ptr = *i_ptr + 1;
	}
	else
	{
		new_result = add_char_to_result(new_result, '\\');
		if (new_result == NULL)
			return (NULL);
	}
	*i_ptr = *i_ptr + 1;
	return (new_result);
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


