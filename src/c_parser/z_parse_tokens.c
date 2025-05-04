/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/04 15:22:41 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"

t_token_list *get_tokens_after(t_token_list *tokens, t_token_node *pipe_location)
{
	t_token_list *new_list;
    t_token_node *current_original;
	
	new_list = create_token_list();
    if (new_list == NULL) 
	{
        perror("Failed to create new token list in get_tokens_before");
        return NULL;
    }
	current_original = pipe_location->next;
    while (current_original != NULL && current_original != pipe_location) 
	{
        add_token(new_list, current_original->value);
        current_original = current_original->next;
    }
    return new_list;
}

t_token_list *get_tokens_before(t_token_list *tokens, t_token_node *pipe_location) 
{
	t_token_list *new_list;
    t_token_node *current_original;
	
	new_list = create_token_list();
    if (new_list == NULL) 
	{
        perror("Failed to create new token list in get_tokens_before");
        return NULL;
    }
	current_original = tokens->head;
    while (current_original != NULL && current_original != pipe_location) 
	{
        add_token(new_list, current_original->value);
        current_original = current_original->next;
    }
    return new_list;
}

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

ast_node *create_command_node(command_value *cmd_details)
{
	ast_node *command_node;
	command_node = (ast_node *)malloc(sizeof(ast_node));
	if (command_node == NULL)
		return (NULL);
	command_node->type = COMMAND_NODE;
	command_node->left = NULL;
	command_node->right = NULL;
	command_node->value = cmd_details;
	return(command_node);
}

t_token_node *find_last_pipe(t_token_list *tokens)
{
	t_token_node *current;
	if (tokens == NULL || tokens->head == NULL || tokens->tail == NULL)
		return (NULL);
	current = tokens->tail;
	while (current != NULL)
	{
		if (current->type == PIPE)
			return (current);
		current = current->prev;
	}
	return (NULL);
}

ast_node *create_pipe_node(ast_node *left_child, ast_node *right_child)
{
	ast_node *pipe_node;
	pipe_node = (ast_node *)malloc(sizeof(ast_node));
	if (pipe_node == NULL)
		return (NULL);
	pipe_node->type = PIPE_NODE;
	pipe_node->left = left_child;
	pipe_node->right = right_child;
	pipe_node->value = NULL;
	return(pipe_node);
}

ast_node *parse_tokens(t_token_list *tokens) {

	if (tokens == NULL || tokens->head == NULL)
	    return NULL;

	t_token_node *pipe_location = find_last_pipe(tokens);
    if (pipe_location != NULL)
    {
        t_token_list *left_tokens = get_tokens_before(tokens, pipe_location);
        t_token_list *right_tokens = get_tokens_after(tokens, pipe_location);
        if (left_tokens == NULL || right_tokens == NULL) {
             fprintf(stderr, "Error creating sub token lists\n");
             free_token_list(left_tokens);
             free_token_list(right_tokens);
             return NULL;
        }
        ast_node *left_child = parse_tokens(left_tokens);
        ast_node *right_child = parse_tokens(right_tokens);
        free_token_list(left_tokens);
        free_token_list(right_tokens);
        if (left_child == NULL || right_child == NULL) {
             fprintf(stderr, "Error parsing sub-commands for pipe\n");
             free_ast(left_child);
             free_ast(right_child);
             return NULL;
        }
        ast_node *pipe_node = create_pipe_node(left_child, right_child);
        if (pipe_node == NULL) {
            fprintf(stderr, "Error creating pipe node\n");
            free_ast(left_child);
            free_ast(right_child);
            return NULL;
        }
        return pipe_node;
    }
    else
    {
        command_value *cmd_details = parse_simple_command(tokens);
        if (cmd_details == NULL) {
            return NULL;
        }
        ast_node *command_node = create_command_node(cmd_details);
        if (command_node == NULL) {
             fprintf(stderr, "Error creating command node\n");
             free_command_value(cmd_details);
             return NULL;
        }
        return command_node;
    }
}

