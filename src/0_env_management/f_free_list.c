#include "../../inc/__minishell.h"
void free_variable_list(t_variable_list *list)
{
    t_variable_node *current;
    t_variable_node *next_node;

    if (list == NULL)
        return;
    current = list->head;
    while (current)
    {
        next_node = current->next;
        free(current->key);
        free(current->value);
        free(current);
        current = next_node;
    }
    free(list);
}