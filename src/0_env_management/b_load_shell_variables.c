#include "../../inc/__minishell.h"

int extract_key(const char *env_str, const char *equal_sign_pos, char **key_out)
{
    size_t key_len;

    key_len = equal_sign_pos - env_str;
    if (key_len == 0)
        return (0);
    *key_out = ft_substr(env_str, 0, key_len);
    if (!(*key_out))
        return (0);
    return (1);
}

int extract_value(const char *equal_sign_pos, char **value_out)
{
    if (*(equal_sign_pos + 1) == '\0')
        *value_out = ft_strdup("");
    else
        *value_out = ft_strdup(equal_sign_pos + 1);
    if (!(*value_out))
        return (0);
    return (1);
}

int parse_env_string(char *env_str, char **key_out, char **value_out)
{
    const char *equal_sign_pos;

    if (!env_str || !key_out || !value_out)
        return (0);
    *key_out = NULL;
    *value_out = NULL;
    equal_sign_pos = ft_strchr(env_str, '=');
    if (equal_sign_pos == NULL)
        return (0);
    if (!extract_key(env_str, equal_sign_pos, key_out))
    {
        return (0);
    }
    if (!extract_value(equal_sign_pos, value_out))
    {
        free(*key_out);
        *key_out = NULL;
        return (0);
    }
    return (1);
}

void load_initial_env(t_variable_list *list, char **envp)
{
    char            *key;
    char            *value;
    t_variable_node *new_node;
    int             i;

    if (!list || !envp)
        return;
    i = 0;
    while (envp[i])
    {
        key = NULL;
        value = NULL;
        if (parse_env_string(envp[i], &key, &value)) // key & value are allocated by parse_env_string
        {
            new_node = create_variable_node(key, value, 1);
            if (new_node)
            {
                list_append_node(list, new_node);
                // Key and value are now owned by new_node and the list.
                // DO NOT free them here. They will be freed when the list/node is destroyed.
            }
            else
            {
                // create_variable_node failed, but parse_env_string allocated key and value.
                // We must free them to prevent a leak.
                free(key);
                free(value);
            }
        }
        // If parse_env_string returned 0, it should have handled freeing
        // any memory it allocated for key/value before returning.
        i++;
    }
}