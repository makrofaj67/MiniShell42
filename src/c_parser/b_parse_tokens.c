/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b_parse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rakman <rakman@student.42istanbul.com.tr>  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 15:39:17 by rakman            #+#    #+#             */
/*   Updated: 2025/05/03 18:28:50 by rakman           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/__minishell.h"
#include <new>
#include <stdlib.h>

t_token_list *get_tokens_after(t_token_list *tokens, t_token_node *pipe_location)
{
	t_token_list *t_tokens;
	return (tokens);
}

t_token_list *get_tokens_before(t_token_list *tokens, t_token_node *pipe_location)
{
	t_token_list *t_tokens;
	return (tokens);
}

typedef struct s_generic_list_node {
    void *value;
    struct s_generic_list_node *prev;
    struct s_generic_list_node *next;
} t_gl_node;

typedef struct s_generic_list {
	t_gl_node *head;
	t_gl_node *tail;
} t_generic_list;

t_generic_list *init_generic_list(void)
{
	t_generic_list *gn_list;
	gn_list = (t_generic_list *)malloc(sizeof(t_generic_list));
	if (gn_list == NULL)
		return (NULL);
	gn_list->head = NULL;
	gn_list->tail = NULL;
	return (gn_list);
}

int add_node_to_generic_list(t_generic_list *list, void *value)
{
    t_gl_node *new_node = (t_gl_node *)malloc(sizeof(t_gl_node));
    if (new_node == NULL) {
        return (0);
    }    new_node->value = value; 
    new_node->next = NULL;
    if (list->tail == NULL) {
        new_node->prev = NULL;
        list->head = new_node;
        list->tail = new_node;
    } else {
        new_node->prev = list->tail;
        list->tail->next = new_node;
        list->tail = new_node;
    }
    return (1); // Başarılı oldu
}


void add_redir(t_token_node *node, t_generic_list *list)
{
		//adding
}

void add_word(t_token_node *node, t_generic_list *list)
{

}

command_value *parse_simple_command(t_token_list *tokens)
{
	command_value		*cmd_details;
	t_generic_list		*redir_list;
	t_generic_list		*word_list;
	t_token_node		*current_token;

	redir_list = init_generic_list();
	word_list = init_generic_list();
	current_token = tokens->head;
	while(current_token != NULL)
	{
		if (current_token->type == RDRT_IN || current_token->type == RDRT_OUT || current_token->type == HEREDOC || current_token->type == APPEND)
		{
			if (current_token->next == NULL || current_token->next->type != WORD)
			{
				printf("Syntax Error");
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
			return (NULL);
		}
	}
	return (cmd_details);
}

/*typedef enum e_ast_type
{
	PIPE_NODE,
	COMMAND_NODE
}	ast_type;

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
} t_redir_type;

typedef struct s_redirection
{
    t_redir_type type;
    char         *filename;
} t_redirection;

typedef struct t_command_value 
{
	char **arg_array;
	t_redirection **redirections;
} command_value;

typedef struct t_ast_node
{
	ast_type			type;
	struct t_ast_node	*left;
	struct t_ast_node	*right;
	command_value		*value;	
}	ast_node;

*/
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
	t_token_node *pipe_location = find_last_pipe(tokens);
	

	/*    if (pipe_location != NULL)
	{
        t_token_list *left_tokens = get_tokens_before(tokens, pipe_location);
        t_token_list *right_tokens = get_tokens_after(tokens, pipe_location);
        ast_node *left_child = parse_tokens(left_tokens);
        ast_node *right_child = parse_tokens(right_tokens);
        ast_node *pipe_node = create_pipe_node(left_child, right_child); 
        return pipe_node;
    }
    else 
    {
        command_value *cmd_details = parse_simple_command(tokens); 
        ast_node *command_node = create_command_node(cmd_details); 
        return command_node;
    }*/
}



