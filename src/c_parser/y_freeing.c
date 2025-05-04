/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   y_freeing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:40:36 by rakman            #+#    #+#             */
/*   Updated: 2025/05/04 14:41:40 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

/**
 * @brief Free only the list structure without freeing the contained values
 * 
 * @param list Generic list to free
 */
void free_generic_list_nodes_only(t_generic_list *list) {
    
	t_gl_node *next_node;
	t_gl_node *current; 
	
	if (list == NULL) 
		return;
	current = list->head;
	while (current != NULL) 
	{
		next_node = current->next; 
		free(current); 
		current = next_node; 
	}
	free(list);
}

/**
 * @brief Free a generic list and its contents depending on the list type
 * 
 * @param list Generic list to free
 * @param is_redir_list Whether this is a redirection list (true) or word list (false)
 */
void free_generic_list_with_contents(t_generic_list *list, int is_redir_list) {
    t_gl_node *next_node;
    t_gl_node *current;
    
    if (list == NULL)
        return;
        
    current = list->head;
    while (current != NULL) {
        next_node = current->next;
        
        if (current->value) {
            if (is_redir_list) {
                t_redirection *redir = (t_redirection *)current->value;
                if (redir->filename) {
                    free(redir->filename);
                }
                free(redir);
            } else {
                free(current->value);
            }
        }
        
        free(current);
        current = next_node;
    }
    
    free(list);
}

void free_command_value(command_value *details) {
    int i;
    if (details == NULL) {
        return;
    }
    if (details->arg_array != NULL) {
        i = 0;
        while (details->arg_array[i] != NULL) {
            free(details->arg_array[i]);
            i++;
        }
        free(details->arg_array);
    }
    if (details->redirections != NULL) {
        i = 0;
        while (details->redirections[i] != NULL) {
            if (details->redirections[i]->filename != NULL) {
                free(details->redirections[i]->filename);
            }
            free(details->redirections[i]);
            i++;
        }
        free(details->redirections);
    }
    free(details);
}




void free_ast(ast_node *node) {
    if (node == NULL) {
        return;
    }
    switch (node->type) {
        case COMMAND_NODE:
            free_command_value(node->value);
            break;

        case PIPE_NODE:
            free_ast(node->left);
            free_ast(node->right);
            break;
    }
    free(node);
}
