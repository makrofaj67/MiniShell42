#include "../../inc/__minishell.h"

char	*init_expanded_str(void)
{
	char	*result;

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

char	*get_expanded(char *raw_command, int *exit_status_ptr, t_variable_list *variables)
{
	int				i;
	char			*result;

	i = 0;
	result = init_expanded_str();
	if (!result)
		return (NULL);
	while (raw_command[i])
	{
		if (raw_command[i] == '$' && is_need_for_expanding(raw_command, i))
            result = handle_variable_expansion(raw_command, &i, exit_status_ptr, variables, result);
		else
			result = handle_regular_char(raw_command, &i, result);
		if (result == NULL)
			return (NULL);
	}
	return (result);
}
