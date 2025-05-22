#include "../../inc/__minishell.h"
void free_variable_node(t_variable_node *node)
{
    if (!node)
        return;
    free(node->key);   // Düğümün key string'ini serbest bırak
    node->key = NULL;  // (İyi pratik)
    free(node->value); // Düğümün value string'ini serbest bırak
    node->value = NULL; // (İyi pratik)
    free(node);        // Düğümün kendisini serbest bırak
}

void free_variable_list(t_variable_list *list)
{
    t_variable_node *current;
    t_variable_node *next_node;

    if (!list)
        return;

    current = list->head;
    while (current != NULL)
    {
        next_node = current->next;
        free_variable_node(current); // Her düğümü ve içeriğini serbest bırakmak için yardımcı fonksiyonu kullan
        current = next_node;
    }
    list->head = NULL; // Liste işaretçilerini sıfırla
    list->tail = NULL; // Eğer tail kullanılıyorsa

    free(list); // Listenin kendisini (t_variable_list yapısını) serbest bırak
}
