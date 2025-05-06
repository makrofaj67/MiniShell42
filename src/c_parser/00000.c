/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00000.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/06 16:24:47 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_generic_list *init_generic_list(void)
{
	t_generic_list *gn_list;
	gn_list = (t_generic_list *)malloc(sizeof(t_generic_list));
	if (gn_list == NULL)
		return (NULL);
	gn_list->head = NULL;
	gn_list->tail = NULL;
	gn_list->size = 0;
	return (gn_list);
}

int add_node_to_generic_list(t_generic_list *list, void *value)
{
    t_gl_node *new_node = (t_gl_node *)malloc(sizeof(t_gl_node));
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

void add_redir(t_token_node *node, t_generic_list *list) 
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

void add_word(t_token_node *node, t_generic_list *list)
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

command_value *parse_simple_command(t_token_list *tokens)
{
	command_value		*cmd_details;
	t_generic_list		*redir_list;
	t_generic_list		*word_list;
	t_token_node		*current_token;
	int					i;
	int					j;
	t_gl_node			*current_word;
	t_gl_node			*current_redir;

	cmd_details = (command_value *)malloc(sizeof(command_value));
	if (cmd_details == NULL)
		return (NULL);
	redir_list = init_generic_list();
	word_list = init_generic_list();
	if (!redir_list || !word_list) {
		free(cmd_details);
		free_generic_list_with_contents(redir_list, 1);
		free_generic_list_with_contents(word_list, 0);
		return (NULL);
	}
	current_token = tokens->head;
	while(current_token != NULL)
	{
		if (current_token->type == RDRT_IN || current_token->type == RDRT_OUT || current_token->type == HEREDOC || current_token->type == APPEND)
		{
			if (current_token->next == NULL || current_token->next->type != WORD)
			{
				printf("Syntax Error");
				free(cmd_details);
				free_generic_list_with_contents(redir_list, 1);
				free_generic_list_with_contents(word_list, 0);
				return (NULL);
			}
			else
			{
				add_redir(current_token, redir_list);
				if (current_token->next != NULL)
					current_token = current_token->next->next;
			}
		}
		else if (current_token->type == WORD)
		{
			add_word(current_token, word_list);
			current_token = current_token->next;
		}
		else
		{
			printf("Unknown error");
			free(cmd_details);
			free_generic_list_with_contents(redir_list, 1);
			free_generic_list_with_contents(word_list, 0);
			return (NULL);
		}
	}

	cmd_details->arg_array = (char **)malloc(sizeof(char *) * (word_list->size + 1));
	if (cmd_details->arg_array == NULL) {
		free(cmd_details);
		free_generic_list_with_contents(redir_list, 1);
		free_generic_list_with_contents(word_list, 0);
		return (NULL);
	}
	
	i = 0;
	current_word = word_list->head;
	while(current_word != NULL)
	{
		cmd_details->arg_array[i++] = current_word->value;
		current_word = current_word->next;
	}
	cmd_details->arg_array[i] = NULL;

	cmd_details->redirections = (t_redirection **)malloc(sizeof(t_redirection *) * (redir_list->size + 1));
	if (cmd_details->redirections == NULL) {
		free(cmd_details->arg_array);
		free(cmd_details);
		free_generic_list_with_contents(redir_list, 1);
		free_generic_list_with_contents(word_list, 0);
		return (NULL);
	}
	
	j = 0;
	current_redir = redir_list->head;
	while(current_redir != NULL)
	{	
		cmd_details->redirections[j++] = (t_redirection *)current_redir->value;
		current_redir = current_redir->next;
	}
	cmd_details->redirections[j] = NULL; 
	free_generic_list_nodes_only(redir_list);
	free_generic_list_nodes_only(word_list);
	return (cmd_details);
}
