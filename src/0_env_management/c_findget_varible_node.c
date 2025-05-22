#include "../../inc/__minishell.h"

t_variable_node *find_variable_node(t_variable_list *list, const char *key)
{
    t_variable_node *current;

    if (!list || !key)
        return (NULL);
    current = list->head;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return (current);
        current = current->next;
    }
    return (NULL);
}

char *get_variable_value(t_variable_list *list, const char *key)
{
    t_variable_node *node;
    char            *value_copy;

    if (!list || !key)
        return (NULL);
    node = find_variable_node(list, key);
    if (node)
    {
        if (node->value)
        {
            value_copy = ft_strdup(node->value);
            return (value_copy);
        }
        else
            return (NULL);
    }
    return (NULL);
}