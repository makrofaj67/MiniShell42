#include "../../inc/__minishell.h"
#include <stdlib.h> // For malloc and free
#include <limits.h> // For INT_MIN

// REMOVE count_digits, ft_strcmp, ft_itoa, ft_strlen, ft_strdup, ft_substr, ft_strjoin, ft_strcpy
// Ensure their definitions are in your Libft and declarations in __minishell.h or libft.h

// Implementations of ft_ functions:

size_t ft_strlen(const char *s)
{
    size_t i = 0;
    while (s && s[i])
        i++;
    return (i);
}

char *ft_strcpy(char *dst, const char *src)
{
    size_t i = 0;
    if (!dst || !src)
        return (NULL);
    while (src[i])
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}

char *ft_strdup(const char *s1)
{
    char *dup;
    size_t len;

    if (!s1)
        return (NULL);
    len = ft_strlen(s1);
    dup = (char *)malloc(sizeof(char) * (len + 1));
    if (!dup)
        return (NULL);
    ft_strcpy(dup, s1);
    return (dup);
}

char *ft_substr(char const *s, unsigned int start, size_t len)
{
    char *sub;
    size_t s_len;
    size_t i;

    if (!s)
        return (NULL);
    s_len = ft_strlen(s);
    if (start >= s_len)
        return (ft_strdup(""));
    if (len > s_len - start)
        len = s_len - start;
    sub = (char *)malloc(sizeof(char) * (len + 1));
    if (!sub)
        return (NULL);
    i = 0;
    while (i < len && s[start + i])
    {
        sub[i] = s[start + i];
        i++;
    }
    sub[i] = '\0';
    return (sub);
}

char *ft_strjoin(char const *s1, char const *s2)
{
    char *join;
    size_t len1;
    size_t len2;

    if (!s1 && !s2)
        return (NULL);
    if (!s1)
        return (ft_strdup(s2));
    if (!s2)
        return (ft_strdup(s1));
    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    join = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
    if (!join)
        return (NULL);
    ft_strcpy(join, s1);
    ft_strcpy(join + len1, s2);
    return (join);
}

int ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    if (!s1 || !s2)
    {
        if (s1 == s2) return 0;
        return (s1 ? 1 : -1); // Arbitrary non-zero if one is NULL
    }
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static int count_digits(long n)
{
    int count = 0;
    if (n == 0)
        return (1);
    if (n < 0)
    {
        count++;
        n = -n;
    }
    while (n > 0)
    {
        n /= 10;
        count++;
    }
    return (count);
}

char *ft_itoa(int n)
{
    char *str;
    long num;
    int len;
    int is_negative;

    num = n;
    is_negative = 0;
    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }
    len = count_digits(n);
    str = (char *)malloc(sizeof(char) * (len + 1));
    if (!str)
        return (NULL);
    str[len] = '\0';
    if (num == 0)
        str[0] = '0';
    while (num > 0)
    {
        len--;
        str[len] = (num % 10) + '0';
        num /= 10;
    }
    if (is_negative)
        str[0] = '-';
    return (str);
}

static void handle_zero(char *command, int *index, int *state)
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

static void handle_one(char *command, int *index, int *state)
{
    if (command[*index] == '\'') 
    {
        *state = 0;
    }
}

static void handle_two(char *command, int *index, int *state)
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

int get_quote_state_at_position(char *command, int pos)
{
    int i;
    int state;

    i = 0;
    state = 0;
    if (command == NULL)
        return (0);
    while (i < pos && command[i]) 
    {
        if (state == 0)
            handle_zero(command, &i, &state);
        else if (state == 1)
            handle_one(command, &i, &state);
        else if (state == 2)
            handle_two(command, &i, &state);
        i++;
    }
    return (state);
}

int is_need_for_expanding(char *raw_command, int i)
{
    int quote_state;
    if (i > 0 && raw_command[i - 1] == '\\')
    {
        quote_state = get_quote_state_at_position(raw_command, i - 1);
        if (quote_state == 0 || quote_state == 2)
            return (0);
    }
    if (raw_command[i + 1] == '\0')
        return (0);
    if (raw_command[i + 1] == '$')
        return (0);
    if (raw_command[i + 1] == ' ' || raw_command[i + 1] == '\t' || 
        raw_command[i + 1] == '/' || raw_command[i + 1] == '=' ||
        raw_command[i + 1] == ':' || raw_command[i + 1] == ';' ||
        raw_command[i + 1] == ',' || raw_command[i + 1] == '.')
        return (0);
    quote_state = get_quote_state_at_position(raw_command, i);
    if (quote_state == 1)
        return (0);
    if (raw_command[i + 1] == '?')
        return (1);
    if ((raw_command[i + 1] >= 'a' && raw_command[i + 1] <= 'z') ||
        (raw_command[i + 1] >= 'A' && raw_command[i + 1] <= 'Z') ||
        raw_command[i + 1] == '_')
        return (1);
    return (0);
}

static char *init_expanded_str(void)
{
    char *result;

    result = (char *)malloc(sizeof(char) * 1);
    if (result == NULL)
        return (NULL);
    result[0] = '\0';
    return (result);
}

static char *add_char_to_result(char *result, char c)
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

static char *add_str_to_result(char *result, char *str)
{
    char *new_result;

    new_result = ft_strjoin(result, str);
    free(result);
    return (new_result);
}

static char *get_varname(char *raw_command, int *i)
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

static char *get_var_value(char *varname, int exit_status, t_env *env_list)
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

// (ft_strlen, ft_strcpy, ft_strdup, ft_substr, ft_strjoin, ft_strcmp, count_digits, ft_itoa)
// (handle_zero, handle_one, handle_two, get_quote_state_at_position)
// (is_need_for_expanding, init_expanded_str, add_char_to_result, add_str_to_result)
// (get_varname, get_var_value)

static char *handle_backslash_expansion(char *raw_command, int *i_ptr, char *current_result)
{
    char *new_result = current_result;
    int current_i = *i_ptr;
    int quote_state_at_backslash;

    quote_state_at_backslash = get_quote_state_at_position(raw_command, current_i);
    current_i++; // Move past the backslash itself

    if (quote_state_at_backslash == 0) // General State (Unquoted)
    {
        if (raw_command[current_i]) // If there's a character after backslash
        {
            new_result = add_char_to_result(new_result, raw_command[current_i]);
            if (!new_result) return (NULL);
            current_i++; // Move past the escaped character
        }
        // If backslash is at the end of the string, it's consumed.
    }
    else if (quote_state_at_backslash == 1) // Inside Single Quotes ('...')
    {
        // Backslash is literal. Add the original backslash.
        new_result = add_char_to_result(new_result, '\\');
        if (!new_result) return (NULL);
        // The character raw_command[current_i] (that was after the backslash)
        // will be processed in the next iteration of the main loop in get_expanded.
    }
    else if (quote_state_at_backslash == 2) // Inside Double Quotes ("...")
    {
        if (raw_command[current_i] == '$' || raw_command[current_i] == '"' ||
            raw_command[current_i] == '\\' || raw_command[current_i] == '`')
        {
            // Backslash is skipped, the special char is written literally.
            new_result = add_char_to_result(new_result, raw_command[current_i]);
            if (!new_result) return (NULL);
            current_i++; // Move past the escaped character
        }
        else if (raw_command[current_i]) // Backslash followed by a non-special character
        {
            // Treat backslash as literal.
            new_result = add_char_to_result(new_result, '\\');
            if (!new_result) return (NULL);
            // The character raw_command[current_i] (that was after the backslash)
            // will be processed in the next iteration.
        }
        else // Backslash is the last character within double quotes (e.g., "abc\")
        {
            // Add literal backslash
            new_result = add_char_to_result(new_result, '\\');
            if (!new_result) return (NULL);
        }
    }
    *i_ptr = current_i; // Update the original index
    return new_result;
}

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
            {
                return (NULL); // Error occurred in backslash handling
            }
            continue; // Index 'i' and result are updated, continue loop
        }
        else if (raw_command[i] == '$' && is_need_for_expanding(raw_command, i))
        {
            varname = get_varname(raw_command, &i); // get_varname advances i
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
            {
                return (NULL); 
            }
        }
        else
        {
            result = add_char_to_result(result, raw_command[i]);
            if (!result) 
            {
                return (NULL);
            }
            i++;
        }
    }
    // The printf below is likely for debugging.
    // You might want to remove it if this function is part of a larger system.
   // printf("%s\n", result); // Consider removing for final version
    return (result);
}

