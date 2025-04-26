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

void	free_token_list(t_token_list *list)
{
	t_token_node	*current;
	t_token_node	*next;

	if (!list)
		return ;
	current = list->head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
	free(list);
}

char	*extract_token(char *command, int start, int end)
{
	char	*token;
	int		i;

	token = (char *)malloc(sizeof(char) * (end - start + 2));
	if (!token)
		return (NULL);
	i = 0;
	while (start + i <= end)
	{
		token[i] = command[start + i];
		i++;
	}
	token[i] = '\0';
	return (token);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

void	skip_spaces(char *command, int *i)
{
	while (command[*i] && isspace(command[*i]))
		(*i)++;
}

int	get_token_end(char *command, int start)
{
	int	i;

	i = start;
	if (is_special_char(command[i]))
		i++;
	else
		while (command[i] && !isspace(command[i]) && !is_special_char(command[i]))
			i++;
	return (i);
}

t_token_node	*process_token(char *command, int start, int end, t_token_list *stack)
{
	t_token_node	*node;

	node = create_token_node(extract_token(command, start, end - 1));
	if (!node)
	{
		free_token_list(stack);
		return (NULL);
	}
	add_token(node, stack);
	return (node);
}

t_token_list	*tokenize_command(char *command)
{
	t_token_list	*stack;
	int				i;
	int				start;
	int				end;

	stack = init_token_stack();
	if (!stack)
		return (NULL);
	i = 0;
	while (command[i])
	{
		skip_spaces(command, &i);
		if (!command[i])
			break;
		start = i;
		end = get_token_end(command, start);
		if (!process_token(command, start, end, stack))
			return (NULL);
		i = end;
	}
	return (stack);
}
