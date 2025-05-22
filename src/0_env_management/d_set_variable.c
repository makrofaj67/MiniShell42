#include "../../inc/__minishell.h"

void update_existing_variable_node(t_variable_node *node, char *value, int is_exported)
{
    char *new_value_dup;

    if (!node)
        return;
    free(node->value);
    node->value = NULL;
    if (value)
    {
        new_value_dup = ft_strdup(value);
        if (!new_value_dup)
            perror("minishell: update_existing_variable_node: ft_strdup failed for value");
        node->value = new_value_dup;
    }
    node->is_exported = is_exported;
}

static void add_new_variable_node(t_variable_list *list, char *key, char *value, int is_exported)
{
    t_variable_node *new_node;

    if (!list || !key)
    new_node = create_variable_node(key, value, is_exported);
    if (new_node)
        list_append_node(list, new_node);
    else
        perror("minishell: add_new_variable_node: create_variable_node failed");
}

void set_variable(t_variable_list *list, char *key, char *value, int is_exported)
{
    t_variable_node *existing_node;

    if (!list || !key)
        return;
    existing_node = find_variable_node(list, key);
    if (existing_node)
        update_existing_variable_node(existing_node, value, is_exported);
    else 
        add_new_variable_node(list, key, value, is_exported);
}