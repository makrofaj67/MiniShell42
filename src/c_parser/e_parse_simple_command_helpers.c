/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   e_parse_simple_command_helpers.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 16:32:30 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_cmdval_list *init_generic_liss(void)
{
	t_cmdval_list *gn_list;
	gn_list = (t_cmdval_list *)malloc(sizeof(t_cmdval_node));
	if (gn_list == NULL)
		return (NULL);
	gn_list->head = NULL;
	gn_list->tail = NULL;
	gn_list->size = 0;
	return (gn_list);
}

int add_node_to_generic_list(t_cmdval_list *list, void *value)
{
    t_cmdval_node *new_node = (t_cmdval_node *)malloc(sizeof(t_cmdval_node));
    if (new_node == NULL) 
        return (0);    
	new_node->value = value; 
    new_node->next = NULL;
	if (list->tail == NULL) 
	{
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
    } 
	else 
	{
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
	list->size++;
	return (1);
}

void add_redir(t_token_node *node, t_cmdval_list *list) 
{
    t_redirection	*new_redir;
	int				success;
	
	new_redir = (t_redirection *)malloc(sizeof(t_redirection));
    if (new_redir == NULL) {
        perror("malloc failed for t_redirection in add_redir");
        return;
    }
	switch (node->type) 
	{
    case RDRT_IN:
        new_redir->type = REDIR_IN;
        break;
    case RDRT_OUT:
        new_redir->type = REDIR_OUT;
        break;
    case APPEND:
        new_redir->type = REDIR_APPEND;
        break;
    case HEREDOC:
        new_redir->type = REDIR_HEREDOC;
        break;
    default:
        break;
	}
    new_redir->filename = strdup(node->next->value); 
    if (new_redir->filename == NULL) {
        perror("strdup failed for filename in add_redir");
        free(new_redir);
        return; 
    }
    success = add_node_to_generic_list(list, new_redir);
    if (!success) 
	{
        perror("add_node_to_generic_list failed in add_redir");
        free(new_redir->filename);
        free(new_redir);
        return;
    }
}

void add_word(t_token_node *node, t_cmdval_list *list)
{
	char *word_copy;
	int success;

	word_copy = strdup(node->value);
    if (word_copy == NULL) 
	{
        perror("strdup failed in add_word"); 
        return;
    }
    success = add_node_to_generic_list(list, word_copy);
    if (success == 0) 
	{
        free(word_copy); 
        perror("add_node_to_generic_list failed in add_word");
        return;
    }
}

