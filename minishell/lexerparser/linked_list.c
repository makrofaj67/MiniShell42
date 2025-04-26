#include "cell.h"
#include <stdio.h>

typedef enum e_token_type {
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT
} t_token_type;

typedef struct s_token_node 
{
	t_token_type type;
	char *value;
	struct s_token_node *next;
	struct s_token_node *prev;
} t_token_node;

typedef struct s_token_list 
{
	int		size;
	t_token_node *head;
	t_token_node *tail;
} t_token_list ;

t_token_list *init_token_stack()
{
	t_token_list *stack;

	stack = (t_token_list *)malloc(sizeof(t_token_list));
	if (stack == NULL)
		return (NULL);
	stack->head = NULL;
	stack->tail = NULL;
	stack->size = 0;
	return (stack);
}

t_token_node *create_token_node(char *token)
{
	t_token_node* node;

	node = (t_token_node *)malloc(sizeof(t_token_node));
	if (node == NULL)
		return (NULL);
	if (strcmp(token, "|") == 0)
		node->type = PIPE;
	else if (strcmp(token, ">") == 0)
		node->type = REDIR_IN;
	else if (strcmp(token, "<") == 0)
		node->type = REDIR_OUT;
	else
		node->type = WORD;
	node->value = token;
	node->prev = NULL;
	node->next = NULL;
	return (node);
}

void add_token(t_token_node *token_node, t_token_list *token_list)
{
	if(token_list->size == 0)
	{
		token_list->head = token_node;
		token_list->tail = token_node;
	}
	else
	{
		token_list->tail->next = token_node;
		token_node->prev = token_list->tail;
		token_list->tail = token_node;
	}
	token_list->size++;
}


