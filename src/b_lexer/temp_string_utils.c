#include "../../inc/__minishell.h" // For libft functions like ft_strlen, ft_strcpy, ft_strjoin, ft_substr, ft_strdup
#include <stdlib.h> // For malloc, free

// Adds a single character to a dynamically allocated string.
// The original 'result' string is freed.
char	*add_char_to_result(char *result, char c)
{
	char	*new_result;
	size_t	old_len;

	old_len = 0;
	if (result)
		old_len = ft_strlen(result);
	new_result = (char *)malloc(sizeof(char) * (old_len + 2));
	if (new_result == NULL)
	{
		if (result)
			free(result);
		return (NULL); // Allocation failure
	}
	if (result)
		ft_strcpy(new_result, result);
	else
		new_result[0] = '\0'; // Initialize if result was NULL
	new_result[old_len] = c;
	new_result[old_len + 1] = '\0';
	if (result)
		free(result);
	return (new_result);
}

// Appends a string 'str' to a dynamically allocated string 'result'.
// The original 'result' string is freed.
char	*add_str_to_result(char *result, char *str_to_add)
{
	char	*new_result;

	if (!str_to_add)
	{
		// If str_to_add is NULL, arguably result should not change.
		// If result itself is NULL, and str_to_add is NULL, return empty string.
		if (!result)
			return (ft_strdup(""));
		return (result);
	}
	if (!result)
	{
		// If result is NULL, new_result is a duplicate of str_to_add.
		return (ft_strdup(str_to_add));
	}
	new_result = ft_strjoin(result, str_to_add);
	free(result);
	// ft_strjoin handles allocation failure by returning NULL.
	// If new_result is NULL, it means ft_strjoin failed.
	if (new_result == NULL)
	    return (ft_strdup("")); // Or handle error more explicitly
	return (new_result);
}

// Extracts a variable name from raw_command starting at the character after '$'.
// Updates 'i_ptr' to point to the character *after* the end of the variable name.
char	*get_varname(char *raw_command, int *i_ptr)
{
	int		start_index;
	int		current_pos;

	current_pos = *i_ptr + 1; // Move past '$'
	start_index = current_pos;
	if (raw_command[current_pos] == '?')
	{
		*i_ptr = current_pos + 1; // Update i_ptr to point after '?'
		return (ft_strdup("?"));
	}
	// Bash variable names can contain alphanumeric characters and underscores.
	while (raw_command[current_pos] &&
	       ((raw_command[current_pos] >= 'a' && raw_command[current_pos] <= 'z') ||
	        (raw_command[current_pos] >= 'A' && raw_command[current_pos] <= 'Z') ||
	        (raw_command[current_pos] >= '0' && raw_command[current_pos] <= '9') ||
	        raw_command[current_pos] == '_'))
	{
		current_pos++;
	}
	*i_ptr = current_pos; // Update i_ptr to point after the varname
	if (current_pos == start_index) // No valid variable name characters found (e.g., "$$" or "$ ")
		return (ft_strdup("")); 

	return (ft_substr(raw_command, start_index, current_pos - start_index));
}
