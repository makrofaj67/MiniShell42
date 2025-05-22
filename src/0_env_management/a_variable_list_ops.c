#include "../../inc/__minishell.h"

t_variable_node *init_variable_node(void)
{
    t_variable_node *node;

    node = (t_variable_node *)malloc(sizeof(t_variable_node));
    if (!node)
		return (NULL);
	node->is_exported = 0;
    node->key = NULL;
    node->value = NULL;
    node->next = NULL;
	node->prev = NULL;
    return (node);
}

t_variable_node *create_variable_node(char *key, char *value, int is_exported)
{
    t_variable_node *node;

    node = init_variable_node();
    if (!node)
		return (NULL);  
    node->key = ft_strdup(key);
    node->value = ft_strdup(value);
	node->is_exported = is_exported;
	node->next = NULL;
	node->prev = NULL;
    return (node);
}

void list_append_node(t_variable_list *list, t_variable_node *new_node)
{
    if (!list || !new_node)
        return;
    new_node->next = NULL;

    if (list->head == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
        new_node->prev = NULL;
    }
    else
    {
        list->tail->next = new_node;
        new_node->prev = list->tail;
        list->tail = new_node;
    }
}

t_variable_list *init_variable_list(void)
{
    t_variable_list *list;

    list = (t_variable_list *)malloc(sizeof(t_variable_list));
    if (!list)
		return (NULL);  
    list->head = NULL;
    list->tail = NULL;
    return (list);
}